#include <ltsmin/pins.h>

// state slot values
#define EMPTY 0
#define FILLED 1
#define CLEARED 2
#define MARK_1 3
#define MARK_2 4
#define MARK_3 5
#define MARK_4 6
#define MARK_5 7

#define WIDTH 5
#define HEIGHT 5
#define SIZE WIDTH * HEIGHT

// transition labels
#define GROUP_FILL 0
#define GROUP_CLEAR 1
#define GROUP_COUNT 2

// state labels
#define LABEL_GOAL 0

#define ACTION_FILL 0
#define ACTION_CLEAR 1

int group_count() {
	return GROUP_COUNT;
}

int state_length() {
	return SIZE;
}

int label_count() {
	return 1;
}

typedef struct { int x; int y; } pos_t;

int get_value(int *dst, int x, int y) {
	if (x < 0 || x > WIDTH) { return FILLED; }
	if (y < 0 || y > HEIGHT) { return FILLED; }
	return dst[(y * WIDTH) + x];
}

void set_value(int *dst, int x, int y, int value) {
	if (x < 0 || x > WIDTH) { return; }
	if (y < 0 || y > HEIGHT) { return; }
	dst[(y * WIDTH) + x] = value;
}

pos_t next_empty(int * dst) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (get_value(dst, x, y) == EMPTY) {
				pos_t result = { x, y };
				return result;
			}
		}
	}
}

void explore(int *dst, int *expl, int x, int y) {
		
	int changes = 0;
	int i = 1;
	
	do {
	
		pos_t p = next_empty(expl);
		set_value(expl, p.x, p.y, i);
	} while (changes > 0);
	
	do {
		changes = 0;
		i++;
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				int value = get_value(expl, x, y);
				if (value != 0) {
					int value_l = get_value(expl, x - 1, y);
					int value_r = get_value(expl, x + 1, y);
					int value_u = get_value(expl, x, y - 1);
					int value_d = get_value(expl, x, y + 1);
				
					if (value_l == 0) { set_value(expl, x - 1, y, value); }
					if (value_r == 0) { set_value(expl, x + 1, y, value); }
					if (value_u == 0) { set_value(expl, x, y - 1, value); }
					if (value_d == 0) { set_value(expl, x, y + 1, value); }
				}
			}
		}
	} while (changes > 0);
	
	
	
}

int is_valid(int *dst) {
	
	
	int expl[SIZE] = { 0 };
	while (true) {
		explore(dst, expl, p.x, p.y);
		i++;
	}
	
	
	
	
	
}

int fill_next(int *dst, int action, int *same, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	pos_t empty = next_empty(dst);
	set_value(dst, empty.x, empty.y, action);
	set_value(same, empty.x, empty.y, 0);
	if (!is_valid(dst))
	{
		callback(arg, tinfo, dst, same);
		return 1;
	}
	else
	{
		return 0;
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
	
	return fill_next(dst, group, same, &tinfo, callback, arg);
	/*
	switch(group) {
		case ACTION_FILL:	 break;
		case ACTION_CLEAR:	return fill_next(dst, group, same, action, &tinfo, callback, arg); break;
		default: return 0;
	}
	//*/
}

int initial[WIDTH * HEIGHT];
int* initial_state() {
	set_value(initial, 0, 1, MARK_4);
	set_value(initial, 0, 3, MARK_5);
	set_value(initial, 2, 2, MARK_1);
	set_value(initial, 3, 0, MARK_4);
	return initial;
}

int rm[5][5] = {
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1}
};
int* read_matrix(int row) {
	return rm[row];
}

int wm[5][5] = {
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1},
	{1,1,1,1,1}
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
	return (label == LABEL_GOAL && 1==0);/*
		src[PERSON_A] == RIGHTSIDE &&
		src[PERSON_B] == RIGHTSIDE &&
		src[PERSON_C] == RIGHTSIDE &&
		src[PERSON_D] == RIGHTSIDE &&
		src[TIME] <= MAX_TIME);
		//*/
}
