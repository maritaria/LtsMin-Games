#include <ltsmin/pins.h>

// state slot values
#define WIDTH 9
#define HEIGHT 9
#define SIZE_BOARD (WIDTH * HEIGHT)
#define CLUSTER_LENGTH 9
#define CLUSTER_COUNT 9
#define CLUSTER_ITEM_SIZE 2
#define SIZE_CLUSTERS (CLUSTER_LENGTH * CLUSTER_COUNT * CLUSTER_ITEM_SIZE)
#define ITEM_COUNT (SIZE_BOARD + SIZE_CLUSTER)

// transition labels
#define VALUE_1 0
#define VALUE_2 1
#define VALUE_3 2
#define VALUE_4 3
#define VALUE_5 4
#define VALUE_6 5
#define VALUE_7 6
#define VALUE_8 7
#define VALUE_9 8
#define VALUE_EMPTY 9
#define GROUP_COUNT 9

// state labels
#define LABEL_GOAL 0

int group_count() {
	return GROUP_COUNT;
}

int state_length() {
	return ITEM_COUNT;
}

int label_count() {
	return 1;
}

typedef struct {
	int board[WIDTH][HEIGHT];
	pos_t clusters[CLUSTER_COUNT][CLUSTER_LENGTH];
} game_t;

typedef struct { int x; int y; } pos_t;


int get_value(int* dst, int x, int y) {
	return board[x][y];
}

void set_value(int* dst, int x, int y, int value) {
	board[x][y] = value;
}

pos_t* get_cluster(game_t *game, int i, int j) {
	return game->clusters[i][j];
}

void set_cluster(game_t *game, int i, int j, pos_t pos) {
	game->clusters[i][j].x = pos.x;
	game->clusters[i][j].y = pos.y;
}

void print_board(game_t *dst) {
	printf("board:\n");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%d ", get_value(game, x, y)); 
		}
		printf("\n");
	}
}

pos_t next_empty(game_t *dst) {
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < HEIGHT; y++) {
			if (get_value(dst, x, y) == VALUE_EMPTY) {
				pos_t r = { .x = x, .y = y };
				return r;
			}
		}
	}
	pos_t r = { .x = -1, .y = -1 };
	return r;
}

int is_valid_row(int *dst, int y) {
	int values[9] = {0};
	for(int x = 0; x < WIDTH; x++) {
		int value = get_value(dst, x, y);
		values[value]++;
	}
	for(int i = 0; i < 9; i++) {
		if (values[i] > 1) {
			return 0;
		}
	}
	return 1;
}
int is_valid_column(int *dst, int x) {
	int values[9] = {0};
	for(int y = 0; y < HEIGHT; y++) {
		int value = get_value(dst, x, y);
		values[value]++;
	}
	for(int i = 0; i < 9; i++) {
		if (values[i] > 1) {
			return 0;
		}
	}
	return 1;
}
int is_valid_block(int *dst, int i) {
	int values[9] = {0};
	for(int j = 0; j < CLUSTER_LENGTH; j++) {
		pos_t pos = get_cluster(dst, i, j);
		int value = get_value(dst, pos.x, pos.y);
		if (value != VALUE_EMPTY) {
			values[value]++;
		}
	}
	for(int i = 0; i < 9; i++) {
		if (values[i] > 1) {
			return 0;
		}
	}
	return 1;
}

int is_valid(int *dst) {
	for(int x = 0; x < WIDTH; x++) {
		if (!is_valid_column(dst, x)) {
			/*
			printf("Invalid column\n%d %d %d %d %d %d %d %d %d\n",
				get_value(dst, x, 0), get_value(dst, x, 1), get_value(dst, x, 2),
				get_value(dst, x, 3), get_value(dst, x, 4), get_value(dst, x, 5),
				get_value(dst, x, 6), get_value(dst, x, 7), get_value(dst, x, 8));
			//*/
			return 0;
		}
	}
	for(int y = 0; y < HEIGHT; y++) {
		if (!is_valid_row(dst, y)) {
			/*
			printf("Invalid row\n%d %d %d %d %d %d %d %d %d\n",
				get_value(dst, 0, y), get_value(dst, 1, y), get_value(dst, 2, y),
				get_value(dst, 3, y), get_value(dst, 4, y), get_value(dst, 5, y),
				get_value(dst, 6, y), get_value(dst, 7, y), get_value(dst, 8, y));
			//*/
			return 0;
		}
	}
	
	for (int i = 0; i < CLUSTER_COUNT; i++) {
		if (!is_valid_block(dst, i)) {
			/*
			printf("Invalid block\n %d %d %d\n%d %d %d\n%d %d %d\n",
				get_value(dst, x, y), get_value(dst, x + 1, y), get_value(dst, x + 2, y),
				get_value(dst, x, y + 1), get_value(dst, x + 1, y + 1), get_value(dst, x + 2, y + 1),
				get_value(dst, x, y + 2), get_value(dst, x + 1, y + 2), get_value(dst, x + 2, y + 2));
			//*/
			return 0;
		}
	}
}

int next_state(void* model, int group, int *src, TransitionCB callback, void *arg) {
	// the next state values
	int dst[state_length()];
	// not all variables are modified so copy the source state
	memcpy(dst, src, sizeof(int) * state_length());
	int same[state_length()];
	for (int i = 0; i < state_length(); i++) {
		same[i] = 1;
	}
	// provide transition labels and group number of transition.
	int action[1] = { group };
	transition_info_t tinfo = { action, group };
	
	//printf("\n\n");
	
	//print_board(src);
	
	pos_t p = next_empty(src);
	//printf("next_empty %d\n", i);
	if (p.x == -1 && p.y == -1) {
		return 0;
	}
	
	set_value(dst, p.x, p.y, group);
	set_value(same, p.x, p.y, 0);
	
	if (is_valid(dst)) {
		callback(arg, &tinfo, dst, same);
		return 1;
	}
	return 0;
}

game_t initial = {}

int initial_board[SIZE_BOARD] = {
	5, 3, 4, 6, 7, 8, 0, 1, 2,
	6, 7, 2, 1, 0, 5, 3, 4, 8,
	1, 0, 8, 3, 4, 2, 5, 6, 7,
	
	8, 5, 0, 7, 6, 1, 4, 2, 3,
	4, 2, 6, 8, 5, 3, 7, 0, 1,
	7, 1, 3, 0, 2, 4, 8, 5, 6,
	
	0, 6, 1, 5, 3, 7, 2, 8, 4,
	2, 8, 7, 4, 1, 0, 6, 3, 5,
	3, 4, 5, 2, 8, 6, 9, 9, 9
};

pos_t initial_cluster[CLUSTER_COUNT][CLUSTER_LENGTH] = {
	{//red
		{ .x = 0, .y = 0 },
		{ .x = 1, .y = 0 },
		{ .x = 2, .y = 0 },
		{ .x = 0, .y = 1 },
		{ .x = 1, .y = 1 },
		{ .x = 2, .y = 1 },
		{ .x = 0, .y = 2 },
		{ .x = 0, .y = 3 },
		{ .x = 1, .y = 3 },
	},
	{//orange
		{ .x = 3, .y = 0 },
		{ .x = 3, .y = 1 },
		{ .x = 4, .y = 1 },
		{ .x = 5, .y = 1 },
		{ .x = 5, .y = 2 },
		{ .x = 6, .y = 2 },
		{ .x = 7, .y = 2 },
		{ .x = 7, .y = 3 },
		{ .x = 8, .y = 3 },
	},
	{//yellow
		{ .x = 4, .y = 0 },
		{ .x = 5, .y = 0 },
		{ .x = 6, .y = 0 },
		{ .x = 7, .y = 0 },
		{ .x = 8, .y = 0 },
		{ .x = 6, .y = 1 },
		{ .x = 7, .y = 1 },
		{ .x = 8, .y = 1 },
		{ .x = 8, .y = 2 },
	},
	{//green
		{ .x = 1, .y = 2 },
		{ .x = 2, .y = 2 },
		{ .x = 3, .y = 2 },
		{ .x = 4, .y = 2 },
		{ .x = 2, .y = 3 },
		{ .x = 0, .y = 4 },
		{ .x = 1, .y = 4 },
		{ .x = 2, .y = 4 },
		{ .x = 3, .y = 4 },
	},
	{//cyan
		{ .x = 3, .y = 3 },
		{ .x = 4, .y = 3 },
		{ .x = 5, .y = 3 },
		{ .x = 6, .y = 3 },
		{ .x = 4, .y = 4 },
		{ .x = 2, .y = 5 },
		{ .x = 3, .y = 5 },
		{ .x = 4, .y = 5 },
		{ .x = 5, .y = 5 },
	},
	{//blue
		{ .x = 5, .y = 4 },
		{ .x = 6, .y = 4 },
		{ .x = 7, .y = 4 },
		{ .x = 8, .y = 4 },
		{ .x = 6, .y = 5 },
		{ .x = 4, .y = 6 },
		{ .x = 5, .y = 6 },
		{ .x = 6, .y = 6 },
		{ .x = 7, .y = 6 },
	},
	{//purple
		{ .x = 0, .y = 5 },
		{ .x = 1, .y = 5 },
		{ .x = 1, .y = 6 },
		{ .x = 2, .y = 6 },
		{ .x = 3, .y = 6 },
		{ .x = 3, .y = 7 },
		{ .x = 4, .y = 7 },
		{ .x = 5, .y = 7 },
		{ .x = 5, .y = 8 },
	},
	{//dpurple
		{ .x = 0, .y = 6 },
		{ .x = 0, .y = 7 },
		{ .x = 1, .y = 7 },
		{ .x = 2, .y = 7 },
		{ .x = 0, .y = 8 },
		{ .x = 1, .y = 8 },
		{ .x = 2, .y = 8 },
		{ .x = 3, .y = 8 },
		{ .x = 4, .y = 8 },
	},
	{//pink
		{ .x = 7, .y = 5 },
		{ .x = 8, .y = 5 },
		{ .x = 8, .y = 6 },
		{ .x = 6, .y = 7 },
		{ .x = 7, .y = 7 },
		{ .x = 8, .y = 7 },
		{ .x = 6, .y = 8 },
		{ .x = 7, .y = 8 },
		{ .x = 8, .y = 8 },
	},
};

int* initial_state() {
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < WIDTH; y++) {
			int value = initial_board[(y * WIDTH) + x];
			set_value(&initial, x, y, value);
		}
	}
	for(int i = 0; i < CLUSTER_COUNT; i++) {
		for(int j = 0; j < CLUSTER_LENGTH; j++) {
			pos_t value = inital_cluster[i][j];
			set_cluster(&initial, i, j, value);
		}
	}		
	return &initial;
}

int rm[ITEM_COUNT] = {
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1
	};
int* read_matrix(int row) {
	return rm;
}

int wm[ITEM_COUNT] = {
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1
	};
int* write_matrix(int row) {
	return wm;
}

int lm[1][ITEM_COUNT] = {
	{
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,
		1
	}
};
int* label_matrix(int row) {
	return lm[row];
}

int state_label(void* model, int label, int* src) {
	int i = next_empty(src);
	if (i == -1) {
		return is_valid(src);
	}
	return 0;
}
