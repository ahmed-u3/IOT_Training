#include <iostream>

using namespace std;

/*

Write a C++ program that accepts an array of integers from the user
and then calculates and displays the average of all the numbers using a
function. The function should take the array and its size as parameters
and return the average.

*/

double calc_avg(int numbers[], int length);

int main(){

    int numbers[5];
    int value;

    cout << "Please fill the array" << endl;

    for(int i = 0; i < 5; i++){
        cout << "Element Number-" << i+1 << ": ";
        cin >> value;
        numbers[i] = value;
    }

    cout << "Average value of the array: " << calc_avg(numbers, 5);

}

double calc_avg(int numbers[], int length){

    int sum = 0;
    for(int i = 0; i < length; i++){
        sum += numbers[i];
    }

    return (double)sum / (double)length;
}
