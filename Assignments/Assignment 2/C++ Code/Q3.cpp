#include <iostream>

using namespace std;

void fill_array(int arr[], int length){

    int value;

    for(int i = 0; i < length; i++){
        cout << "Element Number-" << i+1 << ": ";
        cin >> value;
        arr[i] = value;
    }
}

void swap_(int *x, int *y){

    int temp = *x;
    *x = *y;
    *y = temp;

}

int find_max_product(int arr[], int length){

    int max_product = 1;

    for(int i = 0; i < length-1; i++){                    // Bubble sorting the array

        for(int j = 0; j < length-1; j++){

            if(arr[j] > arr[j+1]){
                swap_(&arr[j], &arr[j+1]);
            }
        }
    }

    cout << "Max 3 numbers in this array: ";
    for(int i = 0; i < 3; i++){
        cout << arr[length - 1 - i] << " ";
        max_product *= arr[length - 1 - i];
    }

    return max_product;

}

int main(){

    int length;
    int max_product;    // max 3 elements in an array

    cout << "Enter size of the array: ";
    cin >> length;
    int numbers[length];

    fill_array(numbers, length);

    max_product = find_max_product(numbers, length);
    cout << "\nMaximum Product = " << max_product << endl;
}
