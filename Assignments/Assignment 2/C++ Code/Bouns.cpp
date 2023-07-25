#include <iostream>
#include <ctime>
using namespace std;
#define LINE cout<<"\n-------------------------------------------------------"<<endl

//void array_init(int arr[],int n);
void print_array(int arr[],int n);
void Merge_Sort(int arr[],int l ,int r);
void Merge(int arr[],int l , int m , int r);
int median(int arr[],int n);

int main()
{
    int n; // size of the dynamic array
    cout<<"Enter the size of the array : ";
    cin>>n;
    int *arr = new int[n]; // allocating the array in the heap to make in dynamic , take place during runtime not compilation time
    srand(time(0)); // set the seed of srand , time(0) to use current time
    for(int i = 0;i<n;i++){
        arr[i] = rand()%100+1; // to make the range of the numbers between 0 and 99 then we add 1 to be between 1 and 100
    }
    cout<<"The random array before sorting";
    LINE;
    print_array(arr,n); // function to print the array
    //LINE;
    Merge_Sort(arr,0,n-1); // sorting the array using merge sort
    cout<<"the array after sorting ";
    LINE;
    print_array(arr,n); // printing the array
    //LINE;
    cout<<"the median of the array is : "<<median(arr,n); // getting the median of the array
    LINE;
    delete[] arr; // deallocating the array from the heap to avoid memory leak
    return 0;
}

void Merge(int arr[],int l , int m , int r){
    int i , j , k ;
    int n1 = m - l + 1 , n2 = r - m ;// calculating the size of the temp arrays for sorting
    int *left_arr = new int[n1]; // allocating the temp array in the heap
    int *right_arr = new int[n2]; // allocating the temp array in the heap

    for(i = 0 ; i<n1 ; i++)
        left_arr[i] = arr[l+i]; // filling the subarrays
    for(j = 0 ; j<n2 ; j++)
        right_arr[j] = arr[m+1+j]; // filling the subarrays

    i = j = 0 ;
    k = l ;

    while(i<n1 && j<n2){//looping to sort the two subarrays, loop stop condition is the smallest subarray
        if(left_arr[i]<=right_arr[j]){
            arr[k] = left_arr[i];
            i++;
        }
        else{
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }
    while(i<n1){//incase n1 is the smallest then fill the array
        arr[k] = left_arr[i];
        i++ ;
        k++ ;
    }
    while(j<n2){//incase n2 is the smallest then fill the array
        arr[k] = right_arr[j];
        j++ ;
        k++ ;
    }

    delete[] left_arr;// deallocating the array
    delete[] right_arr;// deallocating the array


}

void Merge_Sort(int arr[],int l , int r){
    if(l<r){//here the stopping condition if the array has only one element
        int m = l + (r-l)/2;// calculating the midpoint for cutting
        Merge_Sort(arr,l,m);//recursion to apply divide for array
        Merge_Sort(arr,m+1,r);//recursion to apply divide for array
        Merge(arr,l,m,r);//to apply conquer for array
    }
}

int median(int arr[],int n){
    if(n%2==0) // to get the median in case of even
        return arr[n/2-1];
    return arr[n/2];// to get median in case of odd
}


// function to print the array
void print_array(int arr[],int n){
    for(int i = 0 ;i<n;i++){
        cout<<arr[i]<<"  ";
    }
    LINE;

}
