#include <iostream>

/*

Write a C++ program that initializes an integer array of numbers with
the following values: 5, 10, 15, 20, 25. Then, using a loop to calculate
and display the sum of all the numbers in the array.

*/

using namespace std;

int sum(int arr[],int length){
    int sum = 0;
    for(int i = 0; i < length; i++){
        sum += arr[i];
    }
    return sum;
}

int main(){

    int numbers[] = {5, 10, 15, 20, 25};
    int length = sizeof(numbers) / sizeof(numbers[0]);
    //cout<<"the length is : "<<length<<endl;
    int result = sum(numbers,length);
    cout << "The total sum of this array: " << result;

}
