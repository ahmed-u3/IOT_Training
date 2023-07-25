#include <iostream>

using namespace std;

int main(){

    int arr[5];
    int sum = 0;

    cout << "Please Fill The Array" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << "Number-" << i+1 << ": ";
        cin >> arr[i];
    }

    for (int i = 0; i < 5; ++i) {
        sum += arr[i];
    }

    cout << "Sum of elements in the array: " << sum << endl;

    return 0;
}
