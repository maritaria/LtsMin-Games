#include <ltsmin/pins.h>

// state slot values
#define WIDTH 5
#define HEIGHT 5
#define SIZE_BOARD (WIDTH * HEIGHT)
#define MAX_HINTS 5
#define SIZE_HINTS ((WIDTH + HEIGHT) * MAX_HINTS)
#define SIZE_GAME SIZE_BOARD + SIZE_HINTS

#define NO_HINT 0

// transition labels
#define EMPTY 0
#define WHITE 1
#define BLACK 2
#define GROUP_COUNT 2

// state labels
#define LABEL_GOAL 0

int group_count() {
	return GROUP_COUNT;
}

int state_length() {
	return SIZE_GAME;
}

int label_count() {
	return 1;
}

typedef struct {
	int hints_x[WIDTH][MAX_HINTS];
	int hints_y[HEIGHT][MAX_HINTS];
	int board[SIZE_BOARD];
} game_t;

int get_value(int* dst, int x, int y) {
	return dst[(y * WIDTH) + x];
}

void set_value(int* dst, int x, int y, int value) {
	dst[(y * WIDTH) + x] = value;
}

void print_board(game_t *game) {
	printf("board:\n");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%d ", get_value(game->board, x, y)); 
		}
		printf("\n");
	}
}

int next_empty(game_t *game) {
	for(int i = 0; i < SIZE_BOARD; i++) {
		if (game->board[i] == EMPTY) {
			return i;
		}
	}
	return -1;
}

int is_valid_row(game_t *game, int y) {
	int *hints = game->hints_y[y];
	int x = 0;
	for (int i = 0; i < MAX_HINTS; i++) {
		int hint = hints[i];
		if (hint == NO_HINT) {
			//End of hints
			//printf("end of hints\n");
			break;
		}
		if (x >= WIDTH) {
			//printf("already out of bounds\n");
			return 0;
		}
		
		//printf("\nhint length: %d\n", hint);
		//printf("starting at %d\n", x);
		//Find next black square
		for (;x< WIDTH;x++) {
			int value = get_value(game->board, x, y);
			if (value == BLACK) {
				break;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile searching for black\n");
				return 1;
			}
		}
		//printf("black square %d %d\n", x, hint);
		//Read hint number of squares
		for(int j = 0; j < hint; j++) {
			if (x >= WIDTH) {
				//printf("hint over boundry\n");
				return 0;
			}
			int value = get_value(game->board, x, y);
			if (value == WHITE) {
				//printf("hint comming short %d\n", j);
				return 0;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile\n");
				return 1;
			}
			x++;
		}
		if (x < WIDTH) {
			//Check next is not black
			int value = get_value(game->board, x, y);
			//printf("hecking for white %d\n", x);
			if (value == BLACK) {
				//printf("hint too long\n");
				return 0;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile\n");
				return 1;
			}
		} else {
			//printf("end of line\n");
		}
		//printf("hint completed %d\n", x);
	}
	//Check remaining tiles to be not black
	//printf("checking remaining black from %d\n", x);
	for (;x < WIDTH;x++) {
		if (get_value(game->board, x, y) == BLACK) {
			//printf("remaining black %d\n", x);
			return 0;
		}
	}
	//printf("row valid\n");
	return 1;
}

int is_valid_column(game_t *game, int x) {
	int *hints = game->hints_x[x];
	int y = 0;
	for (int i = 0; i < MAX_HINTS; i++) {
		int hint = hints[i];
		if (hint == NO_HINT) {
			//End of hints
			//printf("end of hints\n");
			break;
		}
		if (y >= HEIGHT) {
			//printf("already out of bounds\n");
			return 0;
		}
		
		//printf("\nhint length: %d\n", hint);
		//printf("starting at %d\n", x);
		//Find next black square
		for (;y< HEIGHT;y++) {
			int value = get_value(game->board, x, y);
			if (value == BLACK) {
				break;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile searching for black\n");
				return 1;
			}
		}
		//printf("black square %d %d\n", x, hint);
		//Read hint number of squares
		for(int j = 0; j < hint; j++) {
			if (y >= HEIGHT) {
				//printf("hint over boundry\n");
				return 0;
			}
			int value = get_value(game->board, x, y);
			if (value == WHITE) {
				//printf("hint comming short %d\n", j);
				return 0;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile\n");
				return 1;
			}
			y++;
		}
		if (y < HEIGHT) {
			//Check next is not black
			int value = get_value(game->board, x, y);
			//printf("hecking for white %d\n", x);
			if (value == BLACK) {
				//printf("hint too long\n");
				return 0;
			}
			if (value == EMPTY) {
				//printf("encountered empty tile\n");
				return 1;
			}
		} else {
			//printf("end of line\n");
		}
		//printf("hint completed %d\n", x);
	}
	//Check remaining tiles to be not black
	//printf("checking remaining black from %d\n", x);
	for (;y < HEIGHT;y++) {
		if (get_value(game->board, x, y) == BLACK) {
			//printf("remaining black %d\n", x);
			return 0;
		}
	}
	//printf("row valid\n");
	return 1;
}

int is_valid(game_t *dst) {
	for(int y = 0; y < HEIGHT; y++) {
		if (!is_valid_row(dst, y)) {
			return 0;
		}
	}
	int i = next_empty(dst);
	if (i == -1) {
		for(int x = 0; x < WIDTH; x++) {
			if (!is_valid_column(dst, x)) {
				return 0;
			}
		}
	}
	return 1;
}

int next_state(void* model, int group, int *src, TransitionCB callback, void *arg) {
	// the next state values
	game_t dst;
	// not all variables are modified so copy the source state
	memcpy(&dst, src, sizeof(game_t));
	int same[state_length()];
	for (int i = 0; i < state_length(); i++) {
		same[i] = 1;
	}
	// provide transition labels and group number of transition.
	int action[1] = { group };
	transition_info_t tinfo = { action, group };
	
	int i = next_empty(&dst);
	//printf("\nnext_empty %d\n", i);
	if (i == -1) {
		return 0;
	}
	
	dst.board[i] = group + 1;
	same[i] = 0;
	
	//print_board(&dst);
	
	if (is_valid(&dst)) {
		callback(arg, &tinfo, (int*)&dst, same);
		//printf("valid\n");
		return 1;
	} else {
		//printf("invalid\n");
	}
	return 0;
}

game_t initial = {
	.hints_x = {
		/*
		{1, 0, 0},
		{1, 1, 0},
		{6, 1, 0},
		{2, 7, 0},
		{8, 1, 0},
		{9, 0, 0},
		{5, 0, 0},
		{5, 0, 0},
		{5, 0, 0},
		{2, 0, 0},
		//*/
		//*
		{1, 0},
		{2, 1},
		{1, 0},
		{2, 1},
		{1, 0},
		//*/
	},
	.hints_y = {
		/*
		{2, 0, 0},
		{4, 0, 0},
		{2, 2, 0},
		{1, 5, 0},
		{8, 0, 0},
		{7, 0, 0},
		{8, 0, 0},
		{7, 0, 0},
		{1, 1, 2},
		{4, 0, 0},
		//*/
		//*
		{1, 1},
		{1, 1},
		{0, 0},
		{1, 1},
		{3, 0},
		//*/
	},
	.board = { 0 },
	
};
int* initial_state() {
	print_board(&initial);
	is_valid(&initial);
	
	
	return (int*) &initial;
}

int state_label(void* model, int label, int* src) {
	int i = next_empty((game_t*)src);
	if (i == -1) {
		if (is_valid((game_t*)src)) {
			//print_board((game_t*)src);
			return 1;
		}
		return 0;
	}
	return 0;
}
