#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>


using namespace std;

vector<string> readExistingIds(const string& filePath) {
	vector<string> ids;
	ifstream file(filePath);

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			stringstream ss(line);
			string currentId;
			if (getline(ss, currentId, ',')) { // Assuming IDs are in the first column
				ids.push_back(currentId);
			}
		}
		file.close();
	}
	return ids;
}

struct Client {
	string id, fname, lname, password;

	string generateId() {
		const string filePath = "C:\\Users\\Administrator\\Desktop\\Clients.csv";
		const string filePath2 = "C:\\Users\\Administrator\\Desktop\\Employees.csv";
		vector<string> existingIds = readExistingIds(filePath);
		vector<string> existingIds2 = readExistingIds(filePath2);


		srand(time(0));
		do {
			int randomNum = std::rand() % 8999 + 1000; 
			id = fname.substr(0, 1) + lname.substr(0, 1) + to_string(randomNum);
		} while (find(existingIds.begin(), existingIds.end(), id) != existingIds.end()
			&& find(existingIds2.begin(), existingIds2.end(), id) != existingIds2.end());

		cout << "Generated unique ID: " << id << endl;

		return id;
	}

	string createPassword() {
		do {
			cout << "\nEnter a password composed of 8 characters:";
			cin >> password;
		} while (password.size() != 8);
		return password;
	}



};

struct ClientNode {
	Client client;
	ClientNode* next;
};
void addClient(ClientNode*& head, Client* newClient) {
	ClientNode* newNode = new ClientNode;
	newNode->client = *newClient; 
	newNode->next = nullptr;

	if (head == nullptr) {
		head = newNode;
	}
	else {
		ClientNode* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newNode;
	}

	delete newClient;
}

void displayClients(ClientNode* head) {
	if (head == nullptr) {
		cout << "No clients in the list." << endl;
		return;
	}

	ClientNode* current = head;
	while (current != nullptr) {
		cout << "\nClient ID: " << current->client.id << endl;
		cout << "First Name: " << current->client.fname << endl;
		cout << "Last Name: " << current->client.lname << endl;
		cout << "Last Name: " << current->client.password << endl;
		cout << "------------------------" << endl;
		current = current->next;
	}
}

ClientNode* searchClientNode(ClientNode* head, string clientId, string password) {
	ClientNode* current = head; 

	while (current != nullptr) {
		if (current->client.id == clientId && current->client.password == password) { 

			return current; 
		}
		current = current->next; 
	}
	return nullptr; 

}

void writeClientsToCSV(ClientNode* head) {
	if (head == nullptr) {
		cout << "The client list is empty. Nothing to write to the CSV file." << endl;
		return;
	}

	string filename = "C:\\Users\\Administrator\\Desktop\\Clients.csv";
	ofstream outFile(filename);
	if (!outFile) {
		cerr << "Error: Unable to open file for writing." << endl;
		return;
	}

	outFile << "ID,First Name,Last Name,Password\n";

	ClientNode* current = head;
	while (current != nullptr) {
		outFile << current->client.id << ","
			<< current->client.fname << ","
			<< current->client.lname << ","
			<< current->client.password << "\n";
		current = current->next;
	}

	outFile.close();
	cout << "Clients successfully written to " << filename << endl;
}
void readClientsFromCSV(ClientNode*& head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\Clients.csv";
	ifstream file(filename); 

	if (!file.is_open()) {
		cerr << "Error: Unable to open file " << filename << " for reading." << endl;
		return;
	}

	string line;
	getline(file, line); 

	while (getline(file, line)) {
		istringstream ss(line);
		string fname, lname, id, password;

		if (getline(ss, id, ',') &&
			getline(ss, fname, ',') &&
			getline(ss, lname, ',') &&
			getline(ss, password)) {

			Client* newClient = new Client;
			newClient->fname = fname;
			newClient->lname = lname;
			newClient->id = id;
			newClient->password = password;

			addClient(head, newClient);
		}
	}

	file.close();
}
Client* createClient() {
	Client* c = new Client;
	cout << "Enter first name:";
	cin >> c->fname;
	cout << "Enter last name:";
	cin >> c->lname;
	c->generateId();
	c->createPassword();
	return c;
}

struct CurrentAccount {
	string password;
	double balance;               
	double overdraftLimit = 200;        
	bool overdraft = false;       
	double minimumBalance = 600;  
	bool isFrozen = false;        
	time_t overdraftTime = 0;    
	double interestRate = 0.02;   
	double transactionFee = 5.0;  
	Client* C1;
	string createPassword() {
		do {
			cout << "\nCreating a Current Account\nEnter a password composed of 8 characters:";
			cin >> password;
		} while (password.size() != 8);
		return password;
	}

	void baseBalance() {

		do {
			cout << "Enter the amount you want to put in your balance: ";
			cin >> balance;
			if (balance < minimumBalance) {
				cout << "Minimum balance required is $" << minimumBalance << ". Please try again.\n";
			}
		} while (balance < minimumBalance);

	}


	void checkOverdraft() {
		if (balance < 0) {
			if (!overdraft) {
				overdraft = true;
				overdraftTime = time(nullptr);

				char buffer[26];
				ctime_s(buffer, sizeof(buffer), &overdraftTime);
				cout << "Overdraft activated on " << buffer;
			}
		}
		else {
			overdraft = false;
		}
	}

	void checkFrozen() {
		if (overdraft) {
			time_t now = time(nullptr);
			double daysSinceOverdraft = difftime(now, overdraftTime) / (60 * 60 * 24); 
			if (daysSinceOverdraft > 30) {
				isFrozen = true;
				cout << "Account has been frozen due to overdraft exceeding 30 days.\n";
			}
		}

		else {
			isFrozen = false;
		}
	}

	void applyOverdraftCharges() {
		if (overdraft) {
			double interest = -balance * interestRate; 
			balance -= (interest + transactionFee);   
			cout << "Overdraft charges applied: $" << interest + transactionFee << "\n"
				<< "New balance: $" << balance << "\n";
		}
	}

	void withdraw() {

		checkOverdraft(); 
		checkFrozen();   
		double amount;
		cout << "\nEnter the amount you want to withdraw:";
		cin >> amount;
		if (isFrozen) {
			cout << "Your account is frozen. Withdrawals are not allowed until the issue is resolved.\n";
			return;
		}

		if (amount <= 0) {
			cout << "Invalid withdrawal amount. Please enter a positive amount.\n";
			return;
		}

		if (amount > balance + overdraftLimit) {
			cout << "Transaction denied. Insufficient funds.\n";
			return;
		}

		if (overdraft && amount > 50) {
			cout << "Your account is currently overdrafted.\n"
				<< "You can only withdraw amounts up to $50 while overdrafted.\n";
			return;
		}

		balance -= amount;
		cout << "Transaction successful! Withdrawn: $" << amount << "\n"
			<< "Remaining balance: $" << balance << "\n";

		checkOverdraft();        
		applyOverdraftCharges(); 
	}

	void deposit() {
		double amount = 0;
		cout << "Enter deposit amout";
		cin >> amount;
		if (amount <= 0) {
			cout << "Deposit amount must be greater than 0.\n";
			return;
		}

		balance += amount;
		cout << "Deposit successful! New balance: $" << balance << "\n";

		checkOverdraft();
	}

	void displayAccountDetails() const {
		cout << "Account Details:\n"
			<< "Id:" << C1->id << endl
			<< "First Name:" << C1->fname << endl
			<< "Last Name:" << C1->lname << endl
			<< "Balance: $" << balance << "\n"
			<< "Overdraft Limit: $" << overdraftLimit << "\n"
			<< "Minimum Balance: $" << minimumBalance << "\n"
			<< "Overdraft Status: " << (overdraft ? "Active" : "Not Active") << "\n"
			<< "Account Frozen: " << (isFrozen ? "Yes" : "No") << "\n";

		if (overdraft) {
			char buffer[26];
			ctime_s(buffer, sizeof(buffer), &overdraftTime);
			cout << "Overdraft Started On: " << buffer;
		}
	}
};
struct CurrentAccountNode {
	CurrentAccount ca;
	CurrentAccountNode* next;
};
void addCurrentAccount(CurrentAccountNode*& head, CurrentAccount* newAccount) {
	CurrentAccountNode* newNode = new CurrentAccountNode;
	newNode->ca = *newAccount;  
	newNode->next = nullptr;

	if (head == nullptr) {
		head = newNode;  
	}
	else {
		CurrentAccountNode* temp = head;
		while (temp->next != nullptr) {  
			temp = temp->next;
		}
		temp->next = newNode;  
	}

}

void displayCurrentAccounts(CurrentAccountNode* head) {
	if (head == nullptr) {
		cout << "No current accounts in the list." << endl;
		return;
	}

	CurrentAccountNode* current = head;
	while (current != nullptr) {
		current->ca.displayAccountDetails();
		cout << "-----------------------------------" << endl;
		current = current->next;
	}
}


void writeCurrentAccountsToCSV(CurrentAccountNode* head) {
	if (head == nullptr) {
		cout << "The Current Account list is empty. Nothing to write to the CSV file." << endl;
		return;
	}

	string filename = "C:\\Users\\Administrator\\Desktop\\CurrentAccounts.csv";
	ofstream outFile(filename);
	if (!outFile) {
		cerr << "Error: Unable to open file for writing." << endl;
		return;
	}

	outFile << "ID,fname,lname,c_Password,a_Password,Balance,OverdraftLimit,Overdraft,MinimumBalance,IsFrozen,OverdraftTime,InterestRate,TransactionFee\n";

	CurrentAccountNode* current = head;
	while (current != nullptr) {
		outFile << current->ca.C1->id << ","
			<< current->ca.C1->fname << ","
			<< current->ca.C1->lname << ","
			<< current->ca.C1->password << "," 
			<< current->ca.password << "," 
			<< current->ca.balance << ","
			<< current->ca.overdraftLimit << ","
			<< (current->ca.overdraft ? "Yes" : "No") << ","
			<< current->ca.minimumBalance << ","
			<< (current->ca.isFrozen ? "Yes" : "No") << ",";

		if (current->ca.overdraft && current->ca.overdraftTime != 0) {
			char buffer[26];
			ctime_s(buffer, sizeof(buffer), &current->ca.overdraftTime);
			string overdraftTime(buffer);
			overdraftTime.erase(overdraftTime.find_last_not_of(" \n\r\t") + 1); 
			outFile << overdraftTime;
		}
		else {
			outFile << "N/A";
		}

		outFile << "," << current->ca.interestRate << ","
			<< current->ca.transactionFee << "\n";

		current = current->next;
	}

	outFile.close(); 
	cout << "\nCurrent Accounts successfully written to " << filename << endl;
}

void readCurrentAccountsFromCSV(CurrentAccountNode*& head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\CurrentAccounts.csv";
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error: Unable to open file " << filename << " for reading." << endl;
		return;
	}

	string line;
	getline(file, line); 

	while (getline(file, line)) {
		istringstream ss(line);
		string id, fname, lname, c_password, a_password, balanceStr, overdraftLimitStr, overdraftStr,
			minimumBalanceStr, frozenStr, overdraftTimeStr, interestRateStr, transactionFeeStr;

		if (getline(ss, id, ',') &&
			getline(ss, fname, ',') &&
			getline(ss, lname, ',') &&
			getline(ss, c_password, ',') &&
			getline(ss, a_password, ',') &&
			getline(ss, balanceStr, ',') &&
			getline(ss, overdraftLimitStr, ',') &&
			getline(ss, overdraftStr, ',') &&
			getline(ss, minimumBalanceStr, ',') &&
			getline(ss, frozenStr, ',') &&
			getline(ss, overdraftTimeStr, ',') &&
			getline(ss, interestRateStr, ',') &&
			getline(ss, transactionFeeStr)) {

			CurrentAccount* newAccount = new CurrentAccount;
			newAccount->C1 = new Client{ id, fname, lname, c_password }; 
			newAccount->password = a_password;
			newAccount->balance = stod(balanceStr);
			newAccount->overdraftLimit = stod(overdraftLimitStr);
			newAccount->minimumBalance = stod(minimumBalanceStr);
			newAccount->overdraft = (overdraftStr == "Yes");
			newAccount->isFrozen = (frozenStr == "Yes");

			if (newAccount->overdraft && overdraftTimeStr != "N/A") {
				std::tm tm = {};
				std::istringstream ss(overdraftTimeStr);
				ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
				if (ss.fail()) {
					cerr << "Error: Failed to parse overdraft time." << endl;
				}
				else {
					newAccount->overdraftTime = mktime(&tm);
				}
			}

			newAccount->interestRate = stod(interestRateStr);
			newAccount->transactionFee = stod(transactionFeeStr);

			addCurrentAccount(head, newAccount);
		}
	}
	file.close();
}
CurrentAccountNode* accessCurrentAcountNode(CurrentAccountNode* head) {

	CurrentAccountNode* current = head; 
	string password,id;

	cout << "\nEnter the account's password:";
	cin >> password;
	cout << "\nEnter the client's id:";
	cin >> id;
	while (current != nullptr) {
		if (current->ca.password == password&&current->ca.C1->id==id) { 
			return current; 
		}
		current = current->next; 
	}
	cout << "\nInvalid password";
	return nullptr; 

}
bool checkCurrentAccount(CurrentAccountNode* head, ClientNode* c) {
	CurrentAccountNode* current = head;
	while (current != nullptr) {
		if (current->ca.C1->id == c->client.id && current->ca.C1->password == c->client.password) {
			return true;
		}
		current = current->next; 
	}
	return false;
}
CurrentAccount* createCurrentAccount(ClientNode* c, CurrentAccountNode* head) {
	if (checkCurrentAccount(head, c)) {
		cout << "\nThis client has already an account";
		return nullptr;
	}
	else {
		CurrentAccount* ca = new CurrentAccount;
		ca->C1 = &c->client;
		ca->createPassword();
		ca->baseBalance();
		return ca;
	}

}
void transferToCurrentAccount(CurrentAccountNode* head, CurrentAccountNode* ca) {
	string id;
	CurrentAccountNode* current = head;
	double amount;
	if (ca->ca.isFrozen) {
		cout << "Your account is frozen. Withdrawals are not allowed until the issue is resolved.\n";
		return;
	}
	else {
		cout << "\nEnter the id of the current acount you want to transfer money to: ";
		cin >> id;
		while (current != nullptr) {
			if (current->ca.C1->id == id) {
				cout << "\nAccount found:\nFirst name: " << current->ca.C1->fname << "\nLast name: " << current->ca.C1->lname;
				do {
					cout << "\n\nEnter the amount you want to transfer: ";
					cin >> amount;
				} while (amount <= 0);

				if (amount > ca->ca.balance + ca->ca.overdraftLimit) {
					cout << "Transaction denied. Insufficient funds.\n";
					return;
				}

				if (ca->ca.overdraft && amount > 50) {
					cout << "Your account is currently overdrafted.\n"
						<< "You can only withdraw amounts up to $50 while overdrafted.\n";
					return;
				}
				ca->ca.balance -= amount;
				current->ca.balance += amount;
				cout << "\nTransaction successful\n";

				ca->ca.checkOverdraft();
				current->ca.checkOverdraft();
				return;
			}
			current = current->next;
		}
	}
	cout << "\nAccount not found!";
}



struct SavingsAccount {
	double minBalance;
	double Balance;
	double interestRate;
	time_t lastTransactionTime;
	int transactionCount = 0;
	int dailyTransactionLimit = 2;
	string password;
	Client* client;
};

struct SavingAccountNode {
	SavingsAccount* account;
	SavingAccountNode* next;
};

void freezeAccount(SavingsAccount* SA, bool freeze) {
	if (freeze) {
		cout << "Account is now frozen. No transactions allowed!" << endl;
		SA->transactionCount = SA->dailyTransactionLimit; 
	}
	else {
		cout << "Account is now unfrozen. Transactions are allowed!" << endl;
		SA->transactionCount = 0; 
	}
}

void resetDailyTransactionCount(SavingsAccount* SA) {
	time_t now = time(0);
	double daysElapsed = difftime(now, SA->lastTransactionTime) / (60 * 60 * 24);
	if (daysElapsed >= 1) {
		SA->transactionCount = 0; 
		SA->lastTransactionTime = now;
	}
}

void updateLinkedListBalance(SavingAccountNode* head, SavingsAccount* SA) {
	SavingAccountNode* current = head;
	while (current != nullptr) {
		if (current->account == SA) {
			current->account->Balance = SA->Balance;
			break;
		}
		current = current->next;
	}
}
bool checkSavingAccount(SavingAccountNode* head, ClientNode* c) {
	if (head == nullptr) {
		return false;
	}

	SavingAccountNode* current = head;
	while (current != nullptr) {
		if (current->account->client != nullptr) {
			if (current->account->client->id == c->client.id &&
				current->account->client->password == c->client.password) {
				return true;
			}
		}

		current = current->next;
	}
	return false;
}
SavingAccountNode* accessSavingAccountNode(SavingAccountNode* head) {
	if (head == nullptr) {
		return nullptr;
	}

	SavingAccountNode* current = head;
	string password,id;

	cout << "\nEnter the account's password: ";
	cin >> password;
	cout << "\nEnter the client's id:";
	cin >> id;

	while (current != nullptr) {
		if (current->account->password == password&&current->account->client->id==id) {
			cout << "\nAccess granted." << endl;
			return current;
		}
		current = current->next;
	}

	cout << "\nInvalid password. Access denied." << endl;
	return nullptr;
}

double depositCash(SavingsAccount* SA, SavingAccountNode* head) {
	double amount;
	cout << "Enter the amount you want to deposit: ";
	cin >> amount;

	resetDailyTransactionCount(SA);

	if (SA->transactionCount >= SA->dailyTransactionLimit) {
		cout << "Transaction limit exceeded. Freezing account..." << endl;
		freezeAccount(SA, true);
		return SA->Balance;
	}

	time_t now = time(0);
	double daysElapsed = difftime(now, SA->lastTransactionTime) / (60 * 60 * 24);
	SA->Balance += (SA->Balance * (SA->interestRate / 100)) * (daysElapsed / 365);

	SA->Balance += amount;
	SA->lastTransactionTime = now;
	SA->transactionCount++;

	updateLinkedListBalance(head, SA);

	cout << "Deposit successful. New balance: " << SA->Balance << endl;
	return SA->Balance;
}

double withdrawCash(SavingsAccount* SA, SavingAccountNode* head) {
	double amount;
	cout << "Enter amount you want to withdraw: ";
	cin >> amount;

	resetDailyTransactionCount(SA);

	if (SA->transactionCount >= SA->dailyTransactionLimit) {
		cout << "Transaction limit exceeded. Freezing account..." << endl;
		freezeAccount(SA, true);
		return SA->Balance;
	}

	time_t now = time(0);
	double daysElapsed = difftime(now, SA->lastTransactionTime) / (60 * 60 * 24);
	SA->Balance += (SA->Balance * (SA->interestRate / 100)) * (daysElapsed / 365);

	if (SA->Balance - amount < SA->minBalance) {
		cout << "Insufficient balance after maintaining the minimum balance!" << endl;
		return SA->Balance;
	}

	SA->Balance -= amount;
	SA->lastTransactionTime = now;
	SA->transactionCount++;

	updateLinkedListBalance(head, SA);

	cout << "Withdrawal successful. New balance: " << SA->Balance << endl;
	return SA->Balance;
}
double interestPerDay(SavingsAccount* SA) {
	return (SA->Balance * (SA->interestRate / 100)) / 365;
}
double transferFunds(ClientNode* clientHead, SavingAccountNode* accountHead) {
	string fromClientId, toClientId;
	string fromPassword, toPassword;
	double amount;

	// Step 1: Get sender's account details
	cout << "Enter the client ID of the sender: ";
	cin >> fromClientId;
	cout << "Enter the password of the sender account: ";
	cin >> fromPassword;

	// Search for the sender's account in the client linked list and verify the password
	ClientNode* fromClient = nullptr;
	SavingAccountNode* fromAccount = nullptr;
	bool senderFound = false;

	// Search client list for sender account
	ClientNode* clientIter = clientHead;
	while (clientIter != nullptr) {
		if (clientIter->client.id == fromClientId) {
			fromClient = clientIter;
			break;
		}
		clientIter = clientIter->next;
	}

	if (fromClient == nullptr) {
		cout << "Sender client ID not found!" << endl;
		return -1;  // Indicating failure
	}

	// Search savings account list for the sender account
	SavingAccountNode* accountIter = accountHead;
	while (accountIter != nullptr) {
		if (accountIter->account->client->id == fromClientId) {
			fromAccount = accountIter;
			break;
		}
		accountIter = accountIter->next;
	}

	if (fromAccount == nullptr) {
		cout << "Sender account not found!" << endl;
		return -1;  // Indicating failure
	}

	// Verify the sender's password
	if (fromClient->client.password != fromPassword) {
		cout << "Incorrect password for the sender account!" << endl;
		return -1;  // Indicating failure
	}

	// Step 2: Get recipient's account details
	cout << "Enter the client ID of the recipient: ";
	cin >> toClientId;
	

	// Search client list for recipient account
	ClientNode* toClient = nullptr;
	SavingAccountNode* toAccount = nullptr;

	// Search client list for recipient
	clientIter = clientHead;
	while (clientIter != nullptr) {
		if (clientIter->client.id == toClientId) {
			toClient = clientIter;
			break;
		}
		clientIter = clientIter->next;
	}

	if (toClient == nullptr) {
		cout << "Recipient client ID not found!" << endl;
		return -1;  // Indicating failure
	}

	// Search savings account list for recipient account
	accountIter = accountHead;
	while (accountIter != nullptr) {
		if (accountIter->account->client->id == toClientId) {
			toAccount = accountIter;
			break;
		}
		accountIter = accountIter->next;
	}

	if (toAccount == nullptr) {
		cout << "Recipient account not found!" << endl;
		return -1;  // Indicating failure
	}

	

	// Step 3: Get the transfer amount and verify the transaction
	cout << "Enter the amount you want to transfer: ";
	cin >> amount;

	// Check if the sender has sufficient balance after the transaction
	time_t now = time(0);
	double daysElapsedFrom = difftime(now, fromAccount->account->lastTransactionTime) / (60 * 60 * 24);
	fromAccount->account->Balance += (fromAccount->account->Balance * (fromAccount->account->interestRate / 100)) * (daysElapsedFrom / 365);

	double daysElapsedTo = difftime(now, toAccount->account->lastTransactionTime) / (60 * 60 * 24);
	toAccount->account->Balance += (toAccount->account->Balance * (toAccount->account->interestRate / 100)) * (daysElapsedTo / 365);

	// Check if transfer is possible
	if (fromAccount->account->Balance - amount < fromAccount->account->minBalance) {
		cout << "Insufficient balance for transfer after maintaining the minimum balance!" << endl;
		return fromAccount->account->Balance;  // Return remaining balance
	}

	// Perform the transfer
	fromAccount->account->Balance -= amount;
	toAccount->account->Balance += amount;

	// Update transaction time for both accounts
	fromAccount->account->lastTransactionTime = now;
	toAccount->account->lastTransactionTime = now;

	// Update the balances in the linked list
	updateLinkedListBalance(accountHead, fromAccount->account);
	updateLinkedListBalance(accountHead, toAccount->account);

	cout << "Funds transferred successfully." << endl;
	cout << "New sender balance: " << fromAccount->account->Balance << endl;
	cout << "New recipient balance: " << toAccount->account->Balance << endl;

	return fromAccount->account->Balance;  // Return the sender's new balance
}

SavingAccountNode* createNode(SavingsAccount* account) {
	SavingAccountNode* newNode = new SavingAccountNode();
	newNode->account = account;
	newNode->next = nullptr;
	return newNode;
}

void addSavingAccount(SavingAccountNode*& head, SavingsAccount* newAccount) {
	SavingAccountNode* newNode = createNode(newAccount);
	if (head == nullptr) {
		head = newNode;
	}
	else {
		SavingAccountNode* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newNode;
	}
}
SavingsAccount* createSavingsAccount(ClientNode*& head) {
	string clientId;
	string password;
	cout << "Enter client ID: ";
	cin >> clientId;
	cout << "Enter Client Password:";
	cin >> password;

	const string filePath = "C:\\Users\\Administrator\\Desktop\\Clients.csv";
	vector<string> existingIds = readExistingIds(filePath);

	Client* client = nullptr;
	if (find(existingIds.begin(), existingIds.end(), clientId) != existingIds.end()) {
		ClientNode* clientNode = searchClientNode(head, clientId, password);
		if (clientNode != nullptr) {
			client = &clientNode->client;
		}
		else {
			// If not found in memory, read clients from the CSV and populate the list
			readClientsFromCSV(head);
			clientNode = searchClientNode(head, clientId, password);
			if (clientNode != nullptr) {
				client = &clientNode->client;
			}
		}
	}
	else {
		cout << "Client not found. Creating a new client..." << endl;
		client = createClient();
		addClient(head, client); 
		writeClientsToCSV(head); 
	}

	SavingsAccount* account = new SavingsAccount;
	account->client = client; 

	cout << "Enter minimum balance: ";
	cin >> account->minBalance;

	cout << "Enter balance: ";
	cin >> account->Balance;

	cout << "Enter interest rate: ";
	cin >> account->interestRate;

	account->lastTransactionTime = time(0); 

	cout << "Enter password: ";
	cin >> account->password;

	return account;
};


void writeSavingAccountsToCSV(SavingAccountNode* head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\SavingAccounts.csv";
	ofstream outFile(filename);
	if (!outFile) {
		cerr << "Error: Unable to open file for writing." << endl;
		return;
	}

	outFile << "ID,FirstName,LastName,ClientPassword,MinBalance,Balance,InterestRate,TransactionCount,LastTransactionTime,AccountPassword\n";

	SavingAccountNode* current = head;
	while (current != nullptr) {
		outFile << current->account->client->id << ","
			<< current->account->client->fname << ","
			<< current->account->client->lname << ","
			<< current->account->client->password << ","
			<< current->account->minBalance << ","
			<< current->account->Balance << ","
			<< current->account->interestRate << ","
			<< current->account->transactionCount << ",";

		time_t lastTransactionTime = current->account->lastTransactionTime;
		tm tm = {};
		localtime_s(&tm, &lastTransactionTime);
		outFile << put_time(&tm, "%Y-%m-%d %H:%M:%S") << ",";

		outFile << current->account->password << "\n";

		current = current->next;
	}

	outFile.close();
	cout << "Savings Accounts successfully written to " << filename << endl;
}
void readSavingAccountsFromCSV(SavingAccountNode*& head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\SavingAccounts.csv";
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error: Unable to open file " << filename << " for reading." << endl;
		return;
	}

	string line;
	getline(file, line); 

	while (getline(file, line)) {
		istringstream ss(line);
		string id, fname, lname, clientPassword, minBalanceStr, balanceStr, interestRateStr, transactionCountStr, lastTransactionTimeStr, accountPassword;

		if (getline(ss, id, ',') &&
			getline(ss, fname, ',') &&
			getline(ss, lname, ',') &&
			getline(ss, clientPassword, ',') &&
			getline(ss, minBalanceStr, ',') &&
			getline(ss, balanceStr, ',') &&
			getline(ss, interestRateStr, ',') &&
			getline(ss, transactionCountStr, ',') &&
			getline(ss, lastTransactionTimeStr, ',') &&
			getline(ss, accountPassword)) {

			if (id.empty() || fname.empty() || lname.empty() || clientPassword.empty() || accountPassword.empty()) {
				cerr << "Error: Missing required field(s) in line: " << line << endl;
				continue;
			}

			SavingsAccount* newAccount = new SavingsAccount;
			newAccount->client = new Client; 
			newAccount->client->id = id;
			newAccount->client->fname = fname;
			newAccount->client->lname = lname;
			newAccount->client->password = clientPassword; 
			newAccount->password = accountPassword;       

			try {
				newAccount->minBalance = stod(minBalanceStr);
				newAccount->Balance = stod(balanceStr);
				newAccount->interestRate = stod(interestRateStr);
				newAccount->transactionCount = stoi(transactionCountStr);
			}
			catch (const std::exception& e) {
				cerr << "Error: Invalid numeric data in line: " << line << endl;
				continue;
			}

			std::tm tm = {};
			std::istringstream timeStream(lastTransactionTimeStr);
			timeStream >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
			if (timeStream.fail()) {
				cerr << "Error: Invalid time format in line: " << line << endl;
				continue;
			}
			newAccount->lastTransactionTime = mktime(&tm);

			addSavingAccount(head, newAccount);
		}
		else {
			cerr << "Error: Malformed line in CSV file: " << line << endl;
			continue;
		}
	}

	file.close();
}


void checkAccount(SavingsAccount* SA) {
	cout << "Account Balance: " << SA->Balance << endl;
	cout << "Account min Balance: " << SA->minBalance << endl;

	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &(SA->lastTransactionTime));
	cout << "Account last Transaction: " << buffer;

	cout << "Account Interest Rate: " << SA->interestRate << "%" << endl;
}




void createCSV(const std::string& filename) {
	std::ofstream file(filename);

	if (!file) {
		std::cerr << "Error creating file: " << filename << std::endl;
		return;
	}

	file.close();

	std::cout << "CSV file created: " << filename << std::endl;
}

void sortAccountsByBalance(SavingAccountNode*& head) {
	if (head == nullptr || head->next == nullptr) {
		return;
	}

	bool swapped;
	do {
		swapped = false;
		SavingAccountNode* current = head;
		while (current->next != nullptr) {
			if (current->account->Balance > current->next->account->Balance) {
				std::swap(current->account, current->next->account);
				swapped = true;
			}
			current = current->next;
		}
	} while (swapped);

}


void sortCurrentAccountsByBalance(CurrentAccountNode*& head) {
	if (head == nullptr || head->next == nullptr) {
		return;
	}

	bool swapped;
	do {
		swapped = false;
		CurrentAccountNode* current = head;
		while (current->next != nullptr) {
			if (current->ca.balance > current->next->ca.balance) {
				std::swap(current->ca, current->next->ca);
				swapped = true;
			}
			current = current->next;
		}
	} while (swapped);

}



struct Admin {
	string id, firstName, lastName, password;

	string generateId() {
		const string filePath = "C:\\Users\\Administrator\\Desktop\\Admins.csv";
		vector<string> existingIds = readExistingIds(filePath);

		srand(time(0));
		do {
			int randomNum = std::rand() % 8999 + 1000;
			id = firstName.substr(0, 1) + lastName.substr(0, 1) + to_string(randomNum);
		} while (find(existingIds.begin(), existingIds.end(), id) != existingIds.end());

		cout << "Generated unique ID: " << id << endl;
		return id;
	}

	string createPassword() {
		do {
			cout << "\nEnter a password composed of 8 characters:";
			cin >> password;
		} while (password.size() != 8);
		return password;
	}
};

struct AdminNode {
	Admin admin;
	AdminNode* next;
};

void addAdmin(AdminNode*& head, Admin* newAdmin) {
	AdminNode* newNode = new AdminNode;
	newNode->admin = *newAdmin;
	newNode->next = nullptr;

	if (head == nullptr) {
		head = newNode;
	}
	else {
		AdminNode* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newNode;
	}

	delete newAdmin; 
}

void displayAdmins(AdminNode* head) {
	if (head == nullptr) {
		cout << "No admins in the list." << endl;
		return;
	}

	AdminNode* current = head;
	while (current != nullptr) {
		cout << "\nAdmin ID: " << current->admin.id << endl;
		cout << "First Name: " << current->admin.firstName << endl;
		cout << "Last Name: " << current->admin.lastName << endl;
		cout << "Password: " << current->admin.password << endl;
		cout << "------------------------" << endl;
		current = current->next;
	}
}

AdminNode* searchAdminNode(AdminNode* head, string adminId, string password) {
	AdminNode* current = head;
	while (current != nullptr) {
		if (current->admin.id == adminId && current->admin.password == password) {
			cout << "Admin found" << endl;
			return current;
		}
		current = current->next;
	}

	return nullptr;
}

void writeAdminsToCSV(AdminNode* head) {
	if (head == nullptr) {
		cout << "The admin list is empty. Nothing to write to the CSV file." << endl;
		return;
	}

	string filename = "C:\\Users\\Administrator\\Desktop\\Admins.csv";
	ofstream outFile(filename, ios::app); 
	if (!outFile) {
		cerr << "Error: Unable to open file for writing." << endl;
		return;
	}

	if (outFile.tellp() == 0) {
		outFile << "ID,First Name,Last Name,Password\n";  
	}

	AdminNode* current = head;
	while (current != nullptr) {
		outFile << current->admin.id << ","
			<< current->admin.firstName << ","
			<< current->admin.lastName << ","
			<< current->admin.password << "\n";
		current = current->next;
	}

	outFile.close();
	cout << "Admins successfully appended to " << filename << endl;
}

void readAdminsFromCSV(AdminNode*& head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\Admins.csv";
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error: Unable to open file " << filename << " for reading." << endl;
		return;
	}

	string line;
	getline(file, line); 

	while (getline(file, line)) {
		istringstream ss(line);
		string firstName, lastName, id, password;

		if (getline(ss, id, ',') &&
			getline(ss, firstName, ',') &&
			getline(ss, lastName, ',') &&
			getline(ss, password)) {

			Admin* newAdmin = new Admin;
			newAdmin->firstName = firstName;
			newAdmin->lastName = lastName;
			newAdmin->id = id;
			newAdmin->password = password;

			addAdmin(head, newAdmin); 
		}
	}

	file.close();
}

Admin* createAdmin() {
	cout << "Creating a Client:" << endl;
	Admin* a = new Admin;
	cout << "Enter first name:";
	cin >> a->firstName;
	cout << "Enter last name:";
	cin >> a->lastName;
	a->generateId();
	a->createPassword();
	return a;
}





struct Employee {
	string id;
	string fname, lname, password;

	string generateId() {
		const string filePath = "C:\\Users\\Administrator\\Desktop\\Employees.csv";
		const string filePath2 = "C:\\Users\\Administrator\\Desktop\\Clients.csv";

		vector<string> existingIds = readExistingIds(filePath);
		vector<string> existingIds2 = readExistingIds(filePath2);


		srand(time(0));
		do {
			int randomNum = rand() % 8999 + 1000;
			id = fname.substr(0, 1) + lname.substr(0, 1) + to_string(randomNum);
		} while (find(existingIds.begin(), existingIds.end(), id) != existingIds.end()&&
			find(existingIds2.begin(), existingIds2.end(), id) != existingIds2.end());

		cout << "Generated unique ID: " << id << endl;
		return id;
	}

	string createPassword() {
		do {
			cout << "\nEnter a password composed of 8 characters: ";
			cin >> password;
		} while (password.size() != 8);
		return password;
	}
};

struct EmployeeNode {
	Employee employee;
	EmployeeNode* next;
};

void addEmployee(EmployeeNode*& head, Employee* newEmployee) {
	EmployeeNode* newNode = new EmployeeNode;
	newNode->employee = *newEmployee; 
	newNode->next = nullptr;

	if (head == nullptr) {
		head = newNode;
	}
	else {
		EmployeeNode* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newNode;
	}

	delete newEmployee;
}

void displayEmployees(EmployeeNode* head) {
	if (head == nullptr) {
		cout << "No employees in the list." << endl;
		return;
	}

	EmployeeNode* current = head;
	while (current != nullptr) {
		cout << "\nEmployee ID: " << current->employee.id << endl;
		cout << "First Name: " << current->employee.fname << endl;
		cout << "Last Name: " << current->employee.lname << endl;
		cout << "Password: " << current->employee.password << endl;
		cout << "------------------------" << endl;
		current = current->next;
	}
}


EmployeeNode* searchEmployeeNode(EmployeeNode* head, string employeeId, string password) {
	EmployeeNode* current = head;

	while (current != nullptr) {
		if (current->employee.id == employeeId && current->employee.password == password) {
			cout << "Welcome back!" << endl;
			return current;
		}
		current = current->next;
	}

	return nullptr;
}

void writeEmployeesToCSV(EmployeeNode* head) {
	if (head == nullptr) {
		cout << "The employee list is empty. Nothing to write to the CSV file." << endl;
		return;
	}

	string filename = "C:\\Users\\Administrator\\Desktop\\Employees.csv";
	ofstream outFile(filename);

	if (!outFile) {
		cerr << "Error: Unable to open file for writing." << endl;
		return;
	}

	outFile << "ID,First Name,Last Name,Password\n";

	EmployeeNode* current = head;
	while (current != nullptr) {
		outFile << current->employee.id << ","
			<< current->employee.fname << ","
			<< current->employee.lname << ","
			<< current->employee.password << "\n";
		current = current->next;
	}

	outFile.close();
	cout << "Employees successfully written to " << filename << endl;
}

void readEmployeesFromCSV(EmployeeNode*& head) {
	string filename = "C:\\Users\\Administrator\\Desktop\\Employees.csv";
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "Error: Unable to open file " << filename << " for reading." << endl;
		return;
	}

	string line;
	getline(file, line); 

	while (getline(file, line)) {
		istringstream ss(line);
		string id, fname, lname, password;

		if (getline(ss, id, ',') &&
			getline(ss, fname, ',') &&
			getline(ss, lname, ',') &&
			getline(ss, password)) {
			Employee* newEmployee = new Employee;
			newEmployee->id = id;
			newEmployee->fname = fname;
			newEmployee->lname = lname;
			newEmployee->password = password;

			addEmployee(head, newEmployee);
		}
	}

	file.close();
}
Employee* createEmployee() {
	Employee* e = new Employee;
	cout << "Enter first name:";
	cin >> e->fname;
	cout << "Enter last name:";
	cin >> e->lname;
	e->generateId();
	e->createPassword();

	return e;
}
void checkAccount2(SavingAccountNode* head) {
	SavingAccountNode* current = head;
	
	// Traverse the linked list and display account details
	while (current != nullptr) {
		SavingsAccount* SA = current->account;  // Access the SavingsAccount from the node

		cout << "Account Balance: " << SA->Balance << endl;
		cout << "Account min Balance: " << SA->minBalance << endl;

		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &(SA->lastTransactionTime));
		cout << "Account last Transaction: " << buffer;

		cout << "Account Interest Rate: " << SA->interestRate << "%" << endl;
		cout << "----------------------------------------" << endl;

		current = current->next;  // Move to the next node
	}
}


int main() {
	EmployeeNode* employeeList = nullptr;
	readEmployeesFromCSV(employeeList);

	ClientNode* clientList = nullptr;
	readClientsFromCSV(clientList);

	CurrentAccountNode* caList = nullptr;
	readCurrentAccountsFromCSV(caList);

	SavingAccountNode* saList = nullptr;
	readSavingAccountsFromCSV(saList);

	AdminNode* adminNode = nullptr;
	readAdminsFromCSV(adminNode);

	sortAccountsByBalance(saList);
	sortCurrentAccountsByBalance(caList);

	string id, password;
	cout << "\nEnter your id:";
	cin >> id;
	cout << "\nEnter your password:";
	cin >> password;
	int count = 0;
	ClientNode* c = searchClientNode(clientList, id, password);
	EmployeeNode* e = searchEmployeeNode(employeeList, id, password);
	Employee* Empl;
	bool adminNotFound=true;

	if (adminNode->admin.id == id && adminNode->admin.password ==password) {
		adminNotFound = false;
		int choiceAdmin = 0;
		cout << "This is an Admin." << endl;
		do {
			cout << "Enter Your Choice:\n1-Check Clients\n2-Check Employee\n3-Create employee\n4-display currentAccounts\n5-display savingsAccounts\n0-Exit\n";
			cout << "Enter your choice:";
			cin >> choiceAdmin;
			switch (choiceAdmin) {

			case 1:
				displayClients(clientList);
				break;

			case 2:
				displayEmployees(employeeList);
				break;

			
			case 3:Empl = createEmployee();
				addEmployee(employeeList, Empl);

				break;
			
			case 4:displayCurrentAccounts(caList);
				break;
			case 5:checkAccount2(saList);
				break;

			case 0:cout << "Exiting Menu." << endl;
				break;
			}

		} while (choiceAdmin != 0);

	}

	if (c != nullptr) {
		int choice = 0;
		cout << "\nWelcome to the client's section!";
		do {
			
			if (checkCurrentAccount(caList, c)) {
				cout << "\n1-Current Account";
				count++;
			}
			if (checkSavingAccount(saList,c)) {
				cout << "\n2-Saving Account";
				count++;
			}
			if (count == 0) {
				cout << "\nYou don't have any account\nReach out to us so we create you one!";
			}
			cout << "\n0- to exit";
			if (count > 0) {
				cout << "\nEnter the number of the account you want to have access to:";
				cin >> choice;
			}


			int choice2 = 0;
			CurrentAccountNode* ca = nullptr;
			SavingAccountNode* sa = nullptr;
			do {
				if (choice == 1) {
				 ca = accessCurrentAcountNode(caList);

					if (ca != nullptr) {
						cout << "\nEnter your choice:\n1-withdraw money\n2-deposit money\n3-transfer money to another account"
							"\n4-Display account details\n0-exit";
						cin >> choice2;
						switch (choice2) {

						case 1:ca->ca.withdraw();
							break;

						case 2:ca->ca.deposit();
							break;

						case 3:transferToCurrentAccount(caList, ca);
							break;


						case 4:ca->ca.displayAccountDetails();
							break;

						case 0:cout << "\nExiting this menu";
							break;

						}
					}
				}


				if (choice == 2) {
				    sa = accessSavingAccountNode(saList);

					if (sa != nullptr) {
						cout << "\nEnter your choice:\n1-deposit money\n2-withdraw money\n3-transfer money to another account\n4-Calculate interest per day"
							"\n5-Display account details\n0-exit";
						cin >> choice2;
						switch (choice2) {

						case 1:depositCash(sa->account,saList);
							break;

						case 2:withdrawCash(sa->account,saList);
							break;

						case 3:transferFunds(clientList,saList);
							break;


						case 4:interestPerDay(sa->account);
							break;

						case 5:checkAccount(sa->account);
							break;

						case 0:cout << "\nExiting this menu";
							break;

						}
					}
				}
			} while (choice2 != 0);



		} while (choice != 0);

	}


	if (e != nullptr) {
		int choice3;
		do {
			cout << "\nWelcome to the employee section\nEnter the number of the action you want to make:";
			cout << "\n1-Create a client"
				<< "\n2-Create an account to an existing client"
				<< "\n3-Modify anything in one of the two accounts"
				<< "\n0-Exit\n";
			cin >> choice3;

			if (choice3 == 1) {
				Client* c = createClient();
				addClient(clientList, c);
			}

			if (choice3 == 2) {
				string id, password;
				cout << "\nEnter the client's id:";

				cin >> id;
				cout << "\nEnter the client's password:";
				cin >> password;

				ClientNode* c6 = searchClientNode(clientList, id, password);
				if (c6 == nullptr) {
					cout << "Invalid client";
				}
				else {
					int choice4 = 0;
					do {
						cout << "\nEnter the number of the type you want to create:";
						cout << "\n1-Current Account\n2-Savings Account\n0-to exit";
						cin >> choice4;

						switch (choice4) {
						case 1: {
							CurrentAccount* ca2 = createCurrentAccount(c6, caList);
							addCurrentAccount(caList, ca2);
							break;
						}
						case 2: {
							SavingsAccount* sa7 = createSavingsAccount(clientList);
							addSavingAccount(saList, sa7);
								break;
						}
						case 0:
							cout << "\nExiting";
							break;
						default:
							cout << "\nInvalid choice. Try again.";
						}
					} while (choice4 != 0); 
				}
			}

			if (choice3 == 3) {
				int choice4 = 0;
				do {
					cout << "\nEnter the number of the type you want to edit:";
					cout << "\n1-Current Account\n2-Savings Account\n0-to exit\n";
					cin >> choice4;

					if (choice4 == 1) {
						CurrentAccountNode* ca9 = accessCurrentAcountNode(caList);
						if (ca9 != nullptr) {
							int choice9;
							do {
								cout << "\nEnter the number of the change you want to make:";
								cout << "\n1-Make a withdraw\n2-Put money\n3-Displaay account's details\n0-exit";
								cin >> choice9;

								switch (choice9) {
								case 1:ca9->ca.withdraw();
									break;
								case 2:ca9->ca.deposit();
									break;
								case 3:ca9->ca.displayAccountDetails();
									break;
								case 0:cout << "\nExiting";
									break;
								}
							} while (choice9 != 0);
						}

					}

					if (choice4 == 2) {
						SavingAccountNode* sa9 = accessSavingAccountNode(saList);
						if (sa9 != nullptr) {
							int choice9;
							do {
								cout << "\nEnter your choice:\n1-deposit money\n2-withdraw money\n3-transfer money to another account\n4-Calculate interest per day"
									"\n5-Display account details\n0-exit";
								cin >> choice9;
								switch (choice9) {

								case 1:depositCash(sa9->account, saList);
									break;

								case 2:withdrawCash(sa9->account, saList);
									break;

								case 3:transferFunds(clientList,saList);
									break;

								case 4:interestPerDay(sa9->account);
									break;

								case 5:checkAccount(sa9->account);
									break;

								case 0:cout << "\nExiting this menu";
									break;

								}
							} while (choice9 != 0);
						}

					}
				} while (choice4 != 0);
			}

		} while (choice3 != 0); 
	}

	if (e == nullptr && c == nullptr&&adminNotFound) {
		cout << "\nInvalid id or password";
	}
	else {
		cout << "\nDo you want to save your changes\n1-yes\n2-no";
		int choice43;
		cin >> choice43;
		if (choice43 == 1) {
			writeAdminsToCSV(adminNode);
			writeEmployeesToCSV(employeeList);
			writeSavingAccountsToCSV(saList);
			writeCurrentAccountsToCSV(caList);
			writeClientsToCSV(clientList);
		}
		else {
			cout<<"\nnothing has been saved";
		}
	}
	return 0;
}
