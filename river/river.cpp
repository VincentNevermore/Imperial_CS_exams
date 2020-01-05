#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */
char** make_river_scene (const char *left_bank, const char *boat) {
  // Draw the initialize picture
  char **scene = create_scene();
  // check for the left bank 
  if (strlen(left_bank) > 8 || strlen(boat) > 2) {
    cout << "There is too many cannibals or missionaries or even boats on left bank!!!" <<endl;
    destroy_scene(scene);
    return scene;
  }
  int left_C = 0;
  int left_M = 0;
  int boat_C = 0;
  int boat_M = 0;
  bool is_Boat_at_left_Bank = false;
  while (*left_bank !='\0'){
    switch (*left_bank){
      case 'C':
        left_C++;
        break;
      case 'M':
        left_M++;
        break;
      case 'B':
        is_Boat_at_left_Bank = true;
        break;
      default:
        cout << "Not valid input read from left_bank";
        destroy_scene(scene);
        return scene;
    }
    left_bank++;
  }

  while (*boat !='\0'){
    switch (*boat){
      case 'C':
        boat_C++;
        break;
      case 'M':
        boat_M++;
        break;
      default:
        cout << "Not valid input read from boat";
        destroy_scene(scene);
        return scene;
    }
    boat++;
  }
  // Error Check
  if (left_C >3 || left_C + boat_C > 3) {
    cout << "Too many cannibals on the left bank!" <<endl;
    destroy_scene(scene);
    return scene;
  }
  
  if (left_M >3 || left_M + boat_M > 3) {
    cout << "Too many missionaries on the left bank!" <<endl;
    destroy_scene(scene);
    return scene;
  }
   
  //draw bank, sun, river
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  
  //draw the boat
  draw_boat(scene, is_Boat_at_left_Bank);

  int boat_start;
  if (is_Boat_at_left_Bank) {
    boat_start = 22;
  }
  else {
    boat_start = 39;
  }
  
  for (int i = 0; i < boat_C; i++) {
    add_to_scene(scene, 11, boat_start, "cannibal.txt");
    boat_start += 6;
  }
  for (int i = 0; i < boat_M; i++) {
    add_to_scene(scene, 11, boat_start, "missionary.txt");
    boat_start += 6;
  }

  for (int i = 0; i < left_M; i++) {
    add_to_scene(scene, 2, 1+i*6, "missionary.txt");
  }

  for (int i = 0; i < left_C; i++) {
    add_to_scene(scene, 11, 1+i*6, "cannibal.txt");
  }

  for (int i = 0; i < 3-left_M-boat_M; i++) {
    add_to_scene(scene, 2, 54+i*6, "missionary.txt");
  }
  for (int i = 0; i < 3-left_C-boat_C; i++) {
    add_to_scene(scene, 11, 54+i*6, "cannibal.txt");
  }
  return scene;
}

// Question 2
int perform_crossing (char* left, const char* targets) {
  if (strlen(targets) < 1 && strlen(targets) > 2) {
    return ERROR_INVALID_MOVE;
  }

  std::cout << "Loading the boat..." << left << std::endl;
  
  int m_count = count_char(left, 'M');
  int c_count = count_char(left, 'C');
  int b_count = count_char(left, 'B');
  bool isleft = false;
  int m_boat_count = count_char(targets, 'M');
  int c_boat_count = count_char(targets, 'C');
  
  *left ='\0';
  //boat at left bank: i.e transfor from left to right
  for (int i = 0; i < m_count - m_boat_count; i++) {
    strcat(left, "M");
  }
  for (int i = 0; i < c_count - c_boat_count; i++) {
    strcat(left, "C");
  }
  if (b_count == 1) {
    strcat(left, "B");
    isleft = true;
  }
  cout <<left<< endl;
  char** scene = make_river_scene(left, targets);
  print_scene(scene);

  //Crossing the river
  if (isleft){
    left[strlen(left)-1] = '\0';
    isleft = false;
  }
  else {
    strcat(left, "B");
    isleft = true;
  }
  std::cout << "Crossing the river..." << std::endl;
  scene = make_river_scene(left, targets);
  print_scene(scene);

  //Unloading the boat
  std::cout << "Unloading the boat" << std::endl;
  //check the boat at left or right
  if (isleft){
    for (int i = 0; i < m_boat_count; i++) {
      strcat(left, "M");
      };
    for (int i = 0; i < c_boat_count; i++) {
      strcat(left, "C");
      };
      scene = make_river_scene(left, ""); 
      print_scene(scene);
  }
  else {
    scene = make_river_scene(left, ""); 
    print_scene(scene);
  }
  
  // check if it is a valid move or not
  m_count = count_char(left, 'M');
  c_count = count_char(left, 'C');
  cout << m_count << c_count <<endl;
  if (m_count < c_count || 3- m_count < 3 - c_count) {
    return ERROR_MISSIONARIES_EATEN;
  }
  if (m_count != 0 && c_count != 0) {
    return VALID_NONGOAL_STATE;
  }
  return VALID_GOAL_STATE;
}

int play_game() {
  int result = -1;
  while (result != VALID_GOAL_STATE) {
    char left[255] = "";
    std::cout << "Enter left bank: ";
    std::cin >> left;
    if (strlen(left) > 8) {
      return ERROR_INPUT_STREAM_ERROR;
    }
    //counter the input:
    int m_count = count_char(left, 'M');
    int c_count = count_char(left, 'C');
 
    if (m_count > 3) {
      return ERROR_INVALID_MISSIONARY_COUNT;
    }
    if (c_count > 3) {
      return ERROR_INVALID_CANNIBAL_COUNT;
    }
  
    char target[10] = "";
    std::cout << "Enter boat configuration: ";
    std::cin >> target;
  
  if (strlen(target) > 2) {
    return ERROR_INPUT_STREAM_ERROR;
  } 
  result = perform_crossing(left, target);
  
  if (result == ERROR_MISSIONARIES_EATEN) {
    break;
    return result; 
    }
  }
  return VALID_GOAL_STATE;
}
// Helper Functions
void draw_boat(char**scene, bool is_left) {
  if (is_left){
    add_to_scene(scene, 17,19, "boat.txt");
  }
  else {
    add_to_scene(scene, 17,36, "boat.txt");
  }
}

int count_char(const char* str, char ch) {
  int count = 0;
  while (*str != '\0') {
    if (*str == ch) {
      count++;
    };
    str++;
  }
  return count;
}
