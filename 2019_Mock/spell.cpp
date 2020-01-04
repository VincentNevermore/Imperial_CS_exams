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
            return frq_count(str);
        }
    };
    return 0;
};

// Question 2 Copy from website
int edit_distance(string p_string1, string p_string2){
  int l_string_length1 = p_string1.length();
    int l_string_length2 = p_string2.length();
    int d[l_string_length1+1][l_string_length2+1];

    int i;
    int j;
    int l_cost;

    for (i = 0;i <= l_string_length1;i++)
    {
        d[i][0] = i;
    }
    for(j = 0; j<= l_string_length2; j++)
    {
        d[0][j] = j;
    }
    for (i = 1;i <= l_string_length1;i++)
    {
        for(j = 1; j<= l_string_length2; j++)
        {
            if( p_string1[i-1] == p_string2[j-1] )
            {
                l_cost = 0;
            }
            else
            {
                l_cost = 1;
            }
            d[i][j] = std::min(
            d[i-1][j] + 1,                  // delete
            std::min(d[i][j-1] + 1,         // insert
            d[i-1][j-1] + l_cost)           // substitution
            );
            if( (i > 1) && 
            (j > 1) && 
            (p_string1[i-1] == p_string2[j-2]) && 
            (p_string1[i-2] == p_string2[j-1])
            ) 
            {
            d[i][j] = std::min(
            d[i][j],
             d[i-2][j-2] + l_cost   // transposition
            );
            }
        }
    }
    return d[l_string_length1][l_string_length2];
};

// Helper Functions
bool is_match(const char *word, const char *str) {
    int word_len = strlen(word);
    int str_len = strlen(str);
    for (int i= word_len-1; i>=0; i--){       
        if (word[i] != str[str_len - word_len + i]) {
            return false;
        };
        if (i==0 && str[str_len - word_len-1] == ' '){
            return true;
        };
    }
    return false;
};

int frq_count (const char *str) {
    return atoi(str);
};

