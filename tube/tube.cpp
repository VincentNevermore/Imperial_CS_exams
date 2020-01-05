#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <fstream>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
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

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

//Question 1
bool get_symbol_position(char **map, int height, int width, char target, int &r, int &c) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      if (map[row][col] == target) {
        r = row;
        c = col;
        return true;
      }
    }
  }
  r = -1;
  c = -1;
  return false;
}

//Question 2
char get_symbol_for_station_or_line (const char* name) {
  char symbol = ' ';

  // first search throught stations.txt
  symbol = get_symbol_from_file("stations.txt", name);

  if (symbol == ' ') {
    // second search throught 
    symbol = get_symbol_from_file("lines.txt", name);

  }
  return symbol;
}

char get_symbol_from_file(const char *file_name, const char *name) {
  ifstream in;
  char line[255];
  // open the file
  in.open(file_name);

  if (in.fail()) {
      cout << "Failed to open " << file_name << " file.";
      exit(1);
    }

  while (!in.eof()){
    in.getline(line, 255);
    if (same_name(line, name)) {
      return line[0];
      }
  }

  return ' ';
}

bool same_name(const char *line, const char *name) {
  int len_name = strlen(name);
  int len_line = strlen(line);
  for (int i =len_name-1; i>=0; i--){
    if (name[i] != line[len_line-len_name+i]){
      return false;
    }
    if (i==0 && (isalnum(line[len_line-len_name-1]) == 0)) {
      return true;
    }
  }
  return false; 
}

//Question 3
int validate_route(char **m, int height, int width, const char *start_station, char *route, char *destination) {
  char start;
  int current_row =0;
  int current_col =0;

  start = get_symbol_for_station_or_line(start_station);
  // get the start postion on the map
  bool start_position = get_symbol_position(m, height, width, start, current_row, current_col);
  
  // check if start_station is valid or not
  if (isalnum(start) ==0 || start_position == false) {
    return ERROR_START_STATION_INVALID;
  }

  // check if the route is valid or not
  int res = check_route(route);
  if (res != 0) {
    return res;
  }
  return 1;
}

// check route will check the route is valid or not: 
// In the same time, it will check the backtracking problem as well
int check_route (char *route) {
  int length = strlen(route);
  for(int j = 0; j < length; j++){
    char direction[255]="\0";
    int i = 0;
    while(route[j]!=',' && route[j]!='\0'){
      direction[i] = route[j];
      j++;
      i++;
    }
    Direction enum_direction = string_to_direction(direction);
    cout << enum_direction <<endl;
  }
  return 1;
}
