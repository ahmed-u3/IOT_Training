#include <iostream>

using namespace std;

int sum(int arr[],int length){
    int sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += arr[i];
    }
    return sum;
}
int main(){

    int arr[5];
    cout << "Please Fill The Array" << endl;
    int length = sizeof(arr)/sizeof(arr[0]);
    for (int i = 0; i < length; ++i) {
        cout << "Number-" << i+1 << ": ";
        cin >> arr[i];
    }
    int res = sum(arr,length);
    cout << "Sum of elements in the array: " << res << endl;

    return 0;
}
