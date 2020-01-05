#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}
/* START WRITING YOUR FUNCTION BODIES HERE */

// Question 1
int count_words (const char *line) {
    char word[20];
    int i = 1;

    //loop using get_word
    while (get_word(line, i, word)){
      i++;
    }
    return i -1;
}

// Question 2
bool find_phonetic_ending (const char* word, char* phonetic_ending) {
   ifstream in;
   char line[255];
   char matchword[255];
   bool found = false;

   strcpy(phonetic_ending, "");

   in.open("dictionary.txt");

   while (!in.fail()) {
     in.getline(line,255);

     get_word(line,1,matchword);

     if (strcmp(word, matchword) == 0) {
       found = true;
       break;
     }
   }

   //close the stream
   in.close();

   if (!found) {
     return false;
   }
   
   //count the line of the words
   int num_words = count_words(line);
   int loc_last_vowel = 0;
   char w[255];

   for (int i = num_words; i > 1; i--) {
    get_word(line, i, w);
    if (isVowel(w)) {
      loc_last_vowel = i;
      break;
    }
  }

  for (int i = loc_last_vowel; i <= num_words; i++) {
    get_word(line, i, w);
    strcat(phonetic_ending, w);
  }

  return true;

}

bool isVowel(const char* word) {

  int len = strlen(word);

  for (int i = 0; i < len; i++) {
    char check = tolower(word[i]);

    switch(check) {

    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
      return true;
    }
  }
  return false;
}

// if the file does not exist
bool find_rhyme_scheme(const char* filename, char* scheme)
{
  int i(0), wordCount;
  ifstream input;
  char line[1000], word[100], ending[20];

  input.open(filename);
  if (input.fail()) {
    return false;
  }
  
  rhyming_letter(RESET);

  do {
    input.getline(line,1000);
    wordCount = count_words(line);
    if (!get_word(line,wordCount,word)) break;
    if (!find_phonetic_ending(word,ending)) cout << "No Dict" << endl;
    scheme[i] = rhyming_letter(ending);
    i++;
  } while (!input.fail());
  input.close();

  scheme[i] = '\0';

  return true;
}

// identify sonnet takes the given filename and identifies the sonnet contained
const char* identify_sonnet(const char* filename)
{
  char scheme[100];

  if (!find_rhyme_scheme(filename,scheme)) cerr << "File not found" << endl;

  if (!strcmp(scheme,"ababcdcdefefgg")) return "Shakespearean";
  if (!strcmp(scheme,"abbaabbacdcdcd")) return "Petrarchan";
  if (!strcmp(scheme,"ababbcbccdcdee")) return "Spenserian";

  return "Unknown";
}