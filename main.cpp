// Project: Bank Account Management System
// Description: A beginner-level banking simulation project in C++
// Author: Arian Bozorgzad
// Note: This code demonstrates object-oriented programming, encapsulation,
// file persistence, and simple banking operations.

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

class BankAccount {
private:
    friend class bank;
    static int nextID;
    int accountNumber;
    string accountHolder;
    int balance;
    int pin;
    int failed = 0;
    bool islocked = false;
    vector<string> history;

public:
    BankAccount(string ah, int b, int p) : accountHolder(ah), balance(b), pin(p) {
        accountNumber = nextID;
        cout << "your account number is " << nextID << endl;
        nextID += 1;
    }

    BankAccount(string ah, int accNum, int p, int b, vector<string> hist)
        : accountHolder(ah), accountNumber(accNum), pin(p), balance(b), history(hist) {
        if (accNum >= nextID)
            nextID = accNum + 1;
    }

    string currentTime() {
        time_t now = time(0);
        char* dt = ctime(&now);
        return string(dt);
    }

    int getaccountNumber() { return accountNumber; }
    int getbalance() { return balance; }
    string getaccountHolder() { return accountHolder; }
    int getpin() { return pin; }

    bool isaccountlocked() const { return islocked; }

    vector<string> getHistory() const { return history; }

    void deposit(int value) {
        cout << "the value of " << value << " has been added to your balance\n";
        string transaction = "Deposited " + to_string(value) + " at " + currentTime();
        history.push_back(transaction);
        balance += value;
    }

    void withdraw(int value) {
        if (value <= balance) {
            cout << "the value of " << value << " has been withdrew from your balance\n";
            string transaction = "withdrawn  " + to_string(value) + " at " + currentTime();
            history.push_back(transaction);
            balance -= value;
        } else {
            cerr << "Insufficient balance!!\n";
        }
    }

    void displayinfo() {
        cout << "name: " << accountHolder << endl;
        cout << "your account number: " << accountNumber << endl;
        cout << "your balance: " << balance << endl;
    }

    bool checkPin(int p) {
        if (islocked) {
            cerr << "Account is locked due to multiple failed attempts.\n";
            return false;
        }
        if (p == pin) {
            failed = 0;
            return true;
        } else {
            failed++;
            if (failed >= 3) {
                islocked = true;
                cerr << " account is now locked.\n";
            } else {
                cerr << "Incorrect pin. attempts left: " << 3 - failed << "\n";
            }
            return false;
        }
    }
};

int BankAccount::nextID = 1001;

class bank {
private:
    vector<BankAccount*> dataBase;

public:
    void registerUser() {
        string holder;
        int b, p, conformationpin;
        cin.ignore();
        cout << "name: ";
        getline(cin, holder);
        cout << "what is the amount you wanna put in your account?: ";
        cin >> b;
        bool flag = true;
        while (flag) {
            cout << "enter your pin: ";
            cin >> p;
            cout << "repeat the pin number: ";
            cin >> conformationpin;
            if (p == conformationpin) {
                cout << "welcome to our bank!!";
                dataBase.push_back(new BankAccount(holder, b, p));
                flag = false;
            } else {
                cerr << "the pin didn't match the conformation pin\n";
            }
        }
    }

    int findUser(int id) {
        int index = 0;
        for (BankAccount* i : dataBase) {
            if (id == i->getaccountNumber()) {
                return index;
            }
            index++;
        }
        cerr << "account not found!\n";
        return -1;
    }

    void deleteUser(int id) {
        int i = findUser(id);
        if (i == -1) {
            cout << " account not found. can not delete!!\n";
            return;
        }
        dataBase.erase(dataBase.begin() + i);
    }

    ~bank() {
        for (auto acc : dataBase) {
            delete acc;
        }
    }

    void accesseAccount() {
        int pin, id;
        cout << "Enter account number: ";
        cin >> id;
        int myindex = findUser(id);
        if (myindex == -1) return;
        cout << "enter your pin: ";
        cin >> pin;
        if (!dataBase[myindex]->checkPin(pin)) {
            cout << "the pin is not correct!";
            return;
        }

        cout << "welcome " << dataBase[myindex]->getaccountHolder() << endl;
        int mychoice;

        do {
            cout << "\n1. Deposit\n2. Withdraw\n3. Card-to-Card Transfer\n4. Display Info\n5. Exit\nChoice: ";
            cin >> mychoice;
            switch (mychoice) {
                case 1: {
                    int amount;
                    cout << "amount to deposit: ";
                    cin >> amount;
                    dataBase[myindex]->deposit(amount);
                    break;
                }
                case 2: {
                    int amount;
                    cout << "Amount to withdraw: ";
                    cin >> amount;
                    dataBase[myindex]->withdraw(amount);
                    break;
                }
                case 3: {
                    int id, amount;
                    cout << "enter the card id you want to transfer money to: ";
                    cin >> id;
                    int finalindex = findUser(id);
                    if (finalindex == -1) {
                        cerr << "not found!\n";
                        break;
                    }
                    cout << "please enter the amount of the money you want to transfer: ";
                    cin >> amount;
                    if (amount > dataBase[myindex]->getbalance()) {
                        cerr << "your balance is not enough";
                        break;
                    }
                    int option;
                    cout << "are you sure ? The account holder name is " << dataBase[finalindex]->getaccountHolder() << "\n1. yes\n2. no\n";
                    cin >> option;
                    if (option == 1) {
                        dataBase[myindex]->balance -= amount;
                        dataBase[finalindex]->balance += amount;
                        string send = "You sent " + to_string(amount) +
                            " to account " + to_string(dataBase[finalindex]->getaccountNumber()) +
                            " at " + dataBase[myindex]->currentTime();
                        string receive = "You received " + to_string(amount) +
                            " from account " + to_string(dataBase[myindex]->getaccountNumber()) +
                            " at " + dataBase[finalindex]->currentTime();
                        dataBase[myindex]->history.push_back(send);
                        dataBase[finalindex]->history.push_back(receive);
                    } else {
                        cout << "have a good day\n";
                    }
                    break;
                }
                case 4:
                    dataBase[myindex]->displayinfo();
                    break;
                case 5:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid option.\n";
            }
        } while (mychoice != 5);
    }

    void saveToFile(string filename) {
        ofstream out(filename);
        for (BankAccount* acc : dataBase) {
            out << "BEGIN_ACCOUNT\n";
            out << "Name: " << acc->getaccountHolder() << "\n";
            out << "AccountNumber: " << acc->getaccountNumber() << "\n";
            out << "PIN: " << acc->getpin() << "\n";
            out << "Balance: " << acc->getbalance() << "\n";
            out << "History:\n";
            for (string h : acc->getHistory())
                out << h;
            out << "END_ACCOUNT\n";
        }
        out.close();
    }

    void loadFromFile(string filename) {
        ifstream in(filename);
        if (!in.is_open()) {
            cerr << "File not found. Starting with empty database.\n";
            return;
        }

        string line, name;
        int accNum, pin, balance;
        vector<string> hist;

        while (getline(in, line)) {
            if (line == "BEGIN_ACCOUNT") {
                hist.clear();
                getline(in, line); name = line.substr(6);
                getline(in, line); accNum = stoi(line.substr(15));
                getline(in, line); pin = stoi(line.substr(5));
                getline(in, line); balance = stoi(line.substr(9));
                getline(in, line); // History:
                while (getline(in, line) && line != "END_ACCOUNT") {
                    hist.push_back(line + "\n");
                }
                BankAccount* acc = new BankAccount(name, accNum, pin, balance, hist);
                dataBase.push_back(acc);
                if (accNum >= BankAccount::nextID)
                    BankAccount::nextID = accNum + 1;
            }
        }

        in.close();
    }
};

int main() {
    int choice;
    bank mybank;
    mybank.loadFromFile("accounts.txt");

    do {
        cout << "\n======= Bank Menu =======\n";
        cout << "1. register a new account\n";
        cout << "2. access account\n";
        cout << "3. delete account\n";
        cout << "4. exit\n";
        cout << "enter choice: ";
        cout << "\n========================\n";

        cin >> choice;

        switch (choice) {
            case 1:
                mybank.registerUser();
                break;
            case 2:
                mybank.accesseAccount();
                break;
            case 3: {
                int id;
                cout << "Enter account number to delete: ";
                cin >> id;
                mybank.deleteUser(id);
                break;
            }
            case 4:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);

    mybank.saveToFile("accounts.txt");
    return 0;
}
