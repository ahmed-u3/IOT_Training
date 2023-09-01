#include<iostream>

using namespace std;

int main(){

	int num;
	int fact = 1;
	cout << "Please enter a number to get it's factorial: ";
	cin >> num;

	for(int i = 1; i < num+1; i++){
		fact = fact*i;
	}
	printf("Factorial: %d", fact);

}
