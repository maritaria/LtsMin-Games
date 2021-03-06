#include <ltsmin/pins.h>

// state slot values
#define WIDTH 9
#define HEIGHT 9
#define ITEM_COUNT WIDTH * HEIGHT

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

typedef struct {int x; int y;} pos_t;


int get_value(int* dst, int x, int y) {
	return dst[(y * WIDTH) + x];
}

void set_value(int* dst, int x, int y, int value) {
	dst[(y * WIDTH) + x] = value;
}

void print_board(int *dst) {
	printf("board:\n");
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			printf("%d ", get_value(dst, x, y)); 
		}
		printf("\n");
	}
}

int next_empty(int *dst) {
	for(int i = 0; i < ITEM_COUNT; i++) {
		if (dst[i] == VALUE_EMPTY) {
			return i;
		}
	}
	return -1;
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
int is_valid_block(int *dst, int x, int y) {
	int values[9] = {0};
	for(int i = x; i < x + 3; i++) {
		for(int j = y; j < y + 3; j++) {
			int value = get_value(dst, i, j);
			if (value != VALUE_EMPTY) {
				values[value]++;
			}
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
	
	for (int x = 0; x < WIDTH; x+=3) {
		for (int y = 0; y < WIDTH; y+=3) {
			if (!is_valid_block(dst, x, y)) {
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
	
	int i = next_empty(src);
	//printf("next_empty %d\n", i);
	if (i == -1) {
		return 0;
	}
	
	dst[i] = group;
	same[i] = 0;
	
	if (is_valid(dst)) {
		callback(arg, &tinfo, dst, same);
		return 1;
	}
	return 0;
}
/*
int initial[ITEM_COUNT] = {
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
//*/
//*
int initial[ITEM_COUNT] = {
	5, 3, 9, 9, 7, 9, 9, 9, 9,
	6, 9, 9, 1, 0, 5, 9, 9, 9,
	9, 0, 8, 9, 9, 9, 9, 6, 9,
	
	8, 9, 9, 9, 6, 9, 9, 9, 3,
	4, 9, 9, 8, 9, 3, 9, 9, 1,
	7, 9, 9, 9, 2, 9, 9, 9, 6,
	
	9, 6, 9, 9, 9, 9, 2, 8, 9,
	9, 9, 9, 4, 1, 0, 9, 9, 5,
	9, 9, 9, 9, 8, 9, 9, 7, 0
};
//*/

int* initial_state() {
	return initial;
	
	for(int i = 0; i < ITEM_COUNT; i++) {
		initial[i] = VALUE_EMPTY;
	}
	set_value(initial, 1, 0, 4);
	set_value(initial, 2, 0, 5);
	set_value(initial, 3, 0, 1);
	
	set_value(initial, 4, 1, 3);
	set_value(initial, 8, 1, 8);
	
	set_value(initial, 4, 2, 2);
	set_value(initial, 7, 2, 7);
	/*
	set_value(initial, 1, 3, 2);
	set_value(initial, 2, 3, 0);
	set_value(initial, 3, 3, 7);
	set_value(initial, 8, 3, 4);
	
	set_value(initial, 0, 5, 8);
	set_value(initial, 5, 5, 1);
	set_value(initial, 6, 5, 3);
	set_value(initial, 7, 5, 6);
	
	set_value(initial, 0, 6, 1);
	set_value(initial, 1, 6, 8);
	set_value(initial, 4, 6, 6);
	
	set_value(initial, 0, 7, 7);
	set_value(initial, 4, 7, 5);
	
	set_value(initial, 5, 8, 4);
	set_value(initial, 6, 8, 6);
	set_value(initial, 7, 8, 0);
	//*/
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
