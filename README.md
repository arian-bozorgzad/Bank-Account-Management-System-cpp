# Bank Account Management System (C++)

This is a beginner-to-intermediate level bank account management system written in C++ with features like secure PIN-based login, ATM simulation, and file-based data storage.

##  Features

- create and manage multiple bank accounts
- pin-based system with account lock after 3 failed attempts
- Full transaction history per account (saved to file)
- Card-to-card transfer with dual history recording
- ATM interface class for interaction
- Data persistence via `accounts.txt`

##  files

- main.cpp: Full source code (no external libraries used)
- accounts.txt: Stores account data and transaction history
- README.md: This documentation file

##  How to Compile

```bash
g++ main.cpp -o main
./main
