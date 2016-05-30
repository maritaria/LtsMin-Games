#include <ltsmin/pins.h>

// state slot values
#define FOX 0	// "."
#define GOOSE 1	// "X"
#define BEANS 2	// "@"
#define MAN 3

// transition labels
#define SWITCH_FOX 0
#define SWITCH_GOOSE 1
#define SWITCH_BEANS 2
#define SWITCH_MAN 3

// state labels
#define LABEL_GOAL 0

#define ITEM_COUNT 4

int group_count() {
	return ITEM_COUNT;
}

#define LEFTSIDE 0
#define RIGHTSIDE 1

int state_length() {
	return ITEM_COUNT;
}

int label_count() {
	return 1;
}

int is_valid(int *src) {
	return (
		(src[FOX] != src[GOOSE] || src[FOX] == src[MAN]) &&
		(src[GOOSE] != src[BEANS] || src[GOOSE] == src[MAN])
	);
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
	transition_info_t transition_info = { action, group };
	//Stop if state is invalid
	switch(group) {
		case SWITCH_FOX: {
			if (dst[FOX] == dst[MAN]) {
				if (dst[FOX] == RIGHTSIDE) {
					dst[FOX] = LEFTSIDE;
					dst[MAN] = LEFTSIDE;
				} else {
					dst[FOX] = RIGHTSIDE;
					dst[MAN] = RIGHTSIDE;
				}
				same[FOX] = 0;
				same[MAN] = 0;
				action[0] = SWITCH_FOX;
				if (is_valid(dst)) {
					callback(arg, &transition_info, dst, same);
					return 1;
				}
			}
			break;
		}
		case SWITCH_GOOSE: {
			if (dst[GOOSE] == dst[MAN]) {
				if (dst[GOOSE] == RIGHTSIDE) {
					dst[GOOSE] = LEFTSIDE;
					dst[MAN] = LEFTSIDE;
				} else {
					dst[GOOSE] = RIGHTSIDE;
					dst[MAN] = RIGHTSIDE;
				}
				same[GOOSE] = 0;
				same[MAN] = 0;
				action[0] = SWITCH_GOOSE;
				if (is_valid(dst)) {
					callback(arg, &transition_info, dst, same);
					return 1;
				}
			}
			break;
		}
		case SWITCH_BEANS: {
			if (dst[BEANS] == dst[MAN])
			{
				if (dst[BEANS] == RIGHTSIDE) {
					dst[BEANS] = LEFTSIDE;
					dst[MAN] = LEFTSIDE;
				} else {
					dst[BEANS] = RIGHTSIDE;
					dst[MAN] = RIGHTSIDE;
				}
				same[BEANS] = 0;
				same[MAN] = 0;
				action[0] = SWITCH_BEANS;
				if (is_valid(dst)) {
					callback(arg, &transition_info, dst, same);
					return 1;
				}
			}
			break;
		}
		case SWITCH_MAN: {
			if (dst[MAN] == RIGHTSIDE) {
				dst[MAN] = LEFTSIDE;
			} else {
				dst[MAN] = RIGHTSIDE;
			}
			same[MAN] = 0;
			action[0] = SWITCH_MAN;
			if (is_valid(dst)) {
				callback(arg, &transition_info, dst, same);
				return 1;
			}
			break;
		}
		default: {
			return 0;
		}
	}
}

int initial[ITEM_COUNT];
int* initial_state() {
	initial[FOX] = LEFTSIDE;
	initial[GOOSE] = LEFTSIDE;
	initial[BEANS] = LEFTSIDE;
	initial[MAN] = LEFTSIDE;
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
	{1,0,0,0,01}
};
int* label_matrix(int row) {
	return lm[row];
}

int state_label(void* model, int label, int* src) {
	return label == LABEL_GOAL &&
		src[FOX] == RIGHTSIDE &&
		src[GOOSE] == RIGHTSIDE &&
		src[BEANS] == RIGHTSIDE;
}
