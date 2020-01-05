const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};

// Presupplied functions
char **create_scene();
bool add_to_scene(char **, int, int, const char *);
void print_scene(char **);
void destroy_scene(char **);
const char *status_description(int);

char** make_river_scene (const char *,  const char *);
void draw_boat(char**, bool);
int perform_crossing (char*, const char*);
int count_char(const char*, char);
int play_game();