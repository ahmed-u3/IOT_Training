#include <iostream>

using namespace std;


int count_vowels(char *str){

    int the_count = 0;

    while(*str != '\0'){
        char ch = *str;
        ch = tolower(ch);

        if(ch =='a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u'){
            the_count++;
        }
        str++;
    }
    return the_count;

}

int main(){

    int vowels;

    char text[100];
    cout << "Enter a string: ";
    cin >> text;

    vowels = count_vowels(text);

    cout << "Number of vowels in " << text << ": " << vowels;
}
