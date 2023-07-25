#include <string>
#include <iostream>

class BankAccount
{
    public:
        string owner_name;
        double balance;
        string id;
        BankAccount();
        BankAccount(string name, string account_num) {
        owner_name = x ;
        id = account_num ;
      }
       void deposit(double x) { // add money
       balance+=x;
      cout << "your balance is :" << balance << endl;
        }
        void withdraw(double x) {
        if(balance<x ){ // to check if the account have enough money to withdraw
        cout << "your balance less than this amount" ;
        }else {
        balance-=x;
        cout << "your balance is :" << balance << endl ;
        }
        }
        void get_info(){
        cout << "balance : " << balance << endl
         << "Account number is : " << id << endl;
        }
    protected:

    private:
}