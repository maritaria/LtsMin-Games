#include <ltsmin/pins.h>

// state slot values
#define PIT 0
#define FLOOR 1
#define GOAL 2
#define WIDTH 10
#define HEIGHT 6
#define SIZE (WIDTH * HEIGHT)
#define INFO_X 0
#define INFO_Y 1
#define INFO_ROT 2
#define INFO_COUNT 3
#define STATE_SIZE (INFO_COUNT + SIZE)

#define ROT_STAND 0
#define ROT_HFLAT 1
#define ROT_VFLAT 2

// transition labels
#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3
#define GROUP_COUNT 4

// state labels
#define LABEL_GOAL 0

int group_count() {
	return GROUP_COUNT;
}

int state_length() {
	return STATE_SIZE;
}

int label_count() {
	return 1;
}

typedef struct {
	int x;//topleft
	int y;//topleft
	int rot;
	int board[SIZE];
} game_t;

int get_value(game_t *game, int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) { return PIT; }
	return game->board[(y * WIDTH) + x];
}

void set_value(game_t *game, int x, int y, int value) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) { return; }
	game->board[(y * WIDTH) + x] = value;
}

int is_valid(game_t *game) {
	//Check pos and rot
	switch(game->rot) {
		case ROT_STAND: {
			int value = get_value(game, game->x, game->y);
			return value == FLOOR || value == GOAL;
		}; break;
		case ROT_HFLAT: {
			int value1 = get_value(game, game->x, game->y);
			int value2 = get_value(game, game->x + 1, game->y);
			return (value1 == FLOOR || value1 == GOAL) && (value2 == FLOOR || value2 == GOAL);
		}; break;
		case ROT_VFLAT: {
			int value1 = get_value(game, game->x, game->y);
			int value2 = get_value(game, game->x , game->y + 1);
			return (value1 == FLOOR || value1 == GOAL) && (value2 == FLOOR || value2 == GOAL);
		}; break;
		default: return 0;
	}
}

int is_winner(game_t *game) {
	return (game->rot == ROT_STAND) && (get_value(game, game->x, game->y) == GOAL);
}

void print_game(game_t *game) {
	printf("X: %i Y: %i R:", game->x, game->y);
	switch(game->rot) {
		case ROT_STAND: printf("S"); break;
		case ROT_HFLAT: printf("H"); break;
		case ROT_VFLAT: printf("V"); break;
	}
	printf("\n");
	for(int y = 0; y < WIDTH; y++) {
		for(int x = 0; x < WIDTH; x++) {
			switch(get_value(game, x, y)) {
				case PIT: printf("."); break;
				case FLOOR: printf("#"); break;
				case GOAL: printf("$"); break;
			}
		}
		printf("\n");
	}
}


int move_up(game_t *game, int action, int *same, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	switch(game->rot) {
		case(ROT_STAND): {
			game->y -= 2;
			game->rot = ROT_VFLAT;
		}; break;
		case(ROT_HFLAT): {
			game->y -= 1;
		}; break;
		case(ROT_VFLAT): {
			game->y -= 1;
			game->rot = ROT_STAND;
		}; break;
		default: return 0;
	}
	if (is_valid(game)) {
		callback(arg, tinfo, (int*)game, same);
		return 1;
	}
	return 0;
}

int move_down(game_t *game, int action, int *same, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	switch(game->rot) {
		case(ROT_STAND): {
			game->y += 1;
			game->rot = ROT_VFLAT;
		}; break;
		case(ROT_HFLAT): {
			game->y += 1;
		}; break;
		case(ROT_VFLAT): {
			game->y += 2;
			game->rot = ROT_STAND;
		}; break;
		default: return 0;
	}
	if (is_valid(game)) {
		callback(arg, tinfo, (int*)game, same);
		return 1;
	}
	return 0;
}

int move_left(game_t *game, int action, int *same, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	switch(game->rot) {
		case(ROT_STAND): {
			game->x -= 2;
			game->rot = ROT_HFLAT;
		}; break;
		case(ROT_HFLAT): {
			game->x -= 1;
			game->rot = ROT_STAND;
		}; break;
		case(ROT_VFLAT): {
			game->x -= 1;
		}; break;
		default: return 0;
	}
	if (is_valid(game)) {
		callback(arg, tinfo, (int*)game, same);
		return 1;
	}
	return 0;
}

int move_right(game_t *game, int action, int *same, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	switch(game->rot) {
		case(ROT_STAND): {
			game->x += 1;
			game->rot = ROT_HFLAT;
		}; break;
		case(ROT_HFLAT): {
			game->x += 2;
			game->rot = ROT_STAND;
		}; break;
		case(ROT_VFLAT): {
			game->x += 1;
		}; break;
		default: return 0;
	}
	if (is_valid(game)) {
		callback(arg, tinfo, (int*)game, same);
		return 1;
	}
	return 0;
}

int next_state(void* model, int group, int *src, TransitionCB callback, void *arg) {
	// the next state values
	game_t dst;
	// not all variables are modified so copy the source state
	memcpy(&dst, src, sizeof(int) * state_length());
	int same[state_length()];
	for (int i = INFO_COUNT; i < state_length(); i++) {
		same[i] = 1;
	}
	// provide transition labels and group number of transition.
	int action[1] = { group };
	transition_info_t tinfo = { action, group };
	
	switch(group) {
		case MOVE_UP: return move_up(&dst, group, same, &tinfo, callback, arg);
		case MOVE_DOWN: return move_down(&dst, group, same, &tinfo, callback, arg);
		case MOVE_LEFT: return move_left(&dst, group, same, &tinfo, callback, arg);
		case MOVE_RIGHT: return move_right(&dst, group, same, &tinfo, callback, arg);
		default: return 0;
	}
}

game_t initial;
int* initial_state() {
	initial.x = 1;
	initial.y = 1;
	initial.rot = ROT_STAND;
	
	//Bloxorz level 1
	
	set_value(&initial, 0, 0, FLOOR);
	set_value(&initial, 1, 0, FLOOR);
	set_value(&initial, 2, 0, FLOOR);
	
	set_value(&initial, 0, 1, FLOOR);
	set_value(&initial, 1, 1, FLOOR);
	set_value(&initial, 2, 1, FLOOR);
	set_value(&initial, 3, 1, FLOOR);
	set_value(&initial, 4, 1, FLOOR);
	set_value(&initial, 5, 1, FLOOR);
	
	set_value(&initial, 0, 2, FLOOR);
	set_value(&initial, 1, 2, FLOOR);
	set_value(&initial, 2, 2, FLOOR);
	set_value(&initial, 3, 2, FLOOR);
	set_value(&initial, 4, 2, FLOOR);
	set_value(&initial, 5, 2, FLOOR);
	set_value(&initial, 6, 2, FLOOR);
	set_value(&initial, 7, 2, FLOOR);
	set_value(&initial, 8, 2, FLOOR);
	
	set_value(&initial, 1, 3, FLOOR);
	set_value(&initial, 2, 3, FLOOR);
	set_value(&initial, 3, 3, FLOOR);
	set_value(&initial, 4, 3, FLOOR);
	set_value(&initial, 5, 3, FLOOR);
	set_value(&initial, 6, 3, FLOOR);
	set_value(&initial, 7, 3, FLOOR);
	set_value(&initial, 8, 3, FLOOR);
	set_value(&initial, 9, 3, FLOOR);
	
	//Edit to make it easier: blame dfs instead of bfs
	set_value(&initial, 5, 4, FLOOR);
	set_value(&initial, 6, 4, FLOOR);
	set_value(&initial, 7, 4, GOAL);
	set_value(&initial, 8, 4, FLOOR);
	set_value(&initial, 9, 4, FLOOR);
	
	set_value(&initial, 6, 5, FLOOR);
	set_value(&initial, 7, 5, FLOOR);
	set_value(&initial, 8, 5, FLOOR);
	
	return (int*)&initial;
}

int rm[GROUP_COUNT][STATE_SIZE] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int* read_matrix(int row) {
	return rm[row];
}

int wm[GROUP_COUNT][STATE_SIZE] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int* write_matrix(int row) {
	return wm[row];
}

int lm[1][5] = {
	{1,0,0,0,1}
};
int* label_matrix(int row) {
	return lm[row];
}

int state_label(void* model, int label, int* src) {
	return is_valid((game_t*)src) && is_winner((game_t*)src);
}
