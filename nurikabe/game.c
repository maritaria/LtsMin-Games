#include <ltsmin/pins.h>

// state slot values
#define EMPTY 0
#define FILLED 1
#define CLEARED 2

#define MARK_1 3
#define MARK_2 4
#define MARK_3 5
#define MARK_4 6

#define WIDTH 7
#define HEIGHT 7
#define ITEM_COUNT WIDTH * HEIGHT

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
	return ITEM_COUNT;
}

int label_count() {
	return 1;
}

typedef struct { int x; int y; } pos_t;

int is_valid(int *dst) {
	return dst[TIME] <= MAX_TIME;
}

int get_value(int *dst, int x, int y) {
	if (x < 0 || x > WIDTH) { return FILLED; }
	if (y < 0 || y > HEIGHT) { return FILLED; }
	return dst[(y * WIDTH) + x];
}

void set_value(int *dst, int x, int y, int value) {
	if (x < 0 || x > WIDTH) { return FILLED; }
	if (y < 0 || y > HEIGHT) { return FILLED; }
	dst[(y * WIDTH) + x] = value;
}


pos_t next_empty(int * dst) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (get_value(dst, x, y) == EMPTY) {
				return {x, y};
			}
		}
	}
}

int switch1(int *dst, int p1, int time, int switchcase, int *same, int *action, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	if (dst[p1] == dst[TORCH]) {
		if (dst[p1] == RIGHTSIDE) {
			dst[p1] = LEFTSIDE;
			dst[TORCH] = LEFTSIDE;
		} else {
			dst[p1] = RIGHTSIDE;
			dst[TORCH] = RIGHTSIDE;
		}
		dst[TIME] += time;
		same[p1] = 0;
		same[TORCH] = 0;
		action[0] = switchcase;
		if (is_valid(dst)) {
			callback(arg, tinfo, dst, same);
			return 1;
		}
	}
	return 0;
}

int fill_next(int *dst, int action, int *same, int *action, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	pos_t empty = next_empty(dst);
	set_value(dst, empty.x, empty.y, action);
	set_value(same, empty.x, empty.y, 0);
	
	if (!is_dead_end(dst))
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
	int action[1];
	transition_info_t tinfo = { action, group };
				
	switch(group) {
		case ACTION_FILL:	return fill_next(dst, PERSON_A, TIME_A, SWITCH_A, same, action, &tinfo, callback, arg); break;
		case ACTION_CLEAR:	return fill_next(dst, PERSON_B, TIME_B, SWITCH_B, same, action, &tinfo, callback, arg); break;
		default: return 0;
	}
}

int initial[ITEM_COUNT];
int* initial_state() {
	initial[PERSON_A] = LEFTSIDE;
	initial[PERSON_B] = LEFTSIDE;
	initial[PERSON_C] = LEFTSIDE;
	initial[PERSON_D] = LEFTSIDE;
	initial[TORCH] = LEFTSIDE;
	initial[TIME] = 0;
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
	return (label == LABEL_GOAL &&
		src[PERSON_A] == RIGHTSIDE &&
		src[PERSON_B] == RIGHTSIDE &&
		src[PERSON_C] == RIGHTSIDE &&
		src[PERSON_D] == RIGHTSIDE &&
		src[TIME] <= MAX_TIME);
}
