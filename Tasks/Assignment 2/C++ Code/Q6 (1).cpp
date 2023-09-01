#include<iostream>

using namespace std;

long factorial(int n) {
    int prod = 1;
    for (int i = 1; i <= n; i++) {
        prod *= i;
    }
    return prod;
}

int main(){

	int num;
	cout << "Please enter a number to get it's factorial: ";
	cin >> num;
	long fact = factorial(num);
	printf("Factorial: %d", fact);

}
