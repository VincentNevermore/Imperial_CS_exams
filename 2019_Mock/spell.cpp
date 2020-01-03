#include <iostream>
#include <fstream>
#include "spell.h"
#include <string>

using namespace std;

/* insert your function definitions here */
int frequency (const char *word) {
    ifstream in_stream;
    // Open the file
    in_stream.open("words.dat");
    
    // Error handling for open file
    if (in_stream.fail()) {
        cout << "Sorry, the file can not be open!\n";
        exit(1);
    }
    char str[255];
    // Scan the file line by line
    while (in_stream){
        in_stream.getline(str, 255);
        if (is_match(word, str)) {
            return 1;
        }
    };
    return 0;
};

// Helper Functions
bool is_match(const char *word, const char *str) {
    int word_len = strlen(word);
    int str_len = strlen(str);
    for (int i=word_len-1; i>=0; i--){
        if (word[i] != str[str_len-1]) {
            return false;
        };
        if (i==0 && str[str_len - word_len-1] == ' '){
            return true;
        };
    }
    return false;
}