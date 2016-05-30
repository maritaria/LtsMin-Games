#include <ltsmin/pins.h>

// state slot values
#define EMPTY 0	// "."
#define BOX 1	// "X"
#define MAN 2	// "@"

// transition labels
#define CHOICE_WALK_LEFT 0
#define CHOICE_PUSH_LEFT 1
#define CHOICE_WALK_RIGHT 2
#define CHOICE_PUSH_RIGHT 3

// state labels
#define LABEL_GOAL 0

#define BOARD_WIDTH 5

int group_count() {
	return 4;
}

int state_length() {
	return BOARD_WIDTH;
}

int label_count() {
	return 1;
}

int getpos(int *src) {
	for (int i = 0; i < BOARD_WIDTH; i++) {
		if (src[i] == MAN) {
			return i;
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
	int action[1];
	transition_info_t transition_info = { action, group };
	int x = getpos(src);
	switch(group) {
		case CHOICE_WALK_LEFT: {
			if ((x >= 1) && (src[x - 1] == EMPTY)) {
				printf("Walking left at %i\n", x);
				dst[x] = EMPTY;
				dst[x - 1] = MAN;
				same[x] = 0;
				same[x - 1] = 0;
				action[0] = CHOICE_WALK_LEFT;
				callback(arg, &transition_info, dst, same);
				return 1;
			}
			break;
		}
		case CHOICE_PUSH_LEFT: {
			if ((x >= 2) && (src[x - 2] == EMPTY) && (src[x - 1] == BOX)) {
				printf("Pushing left at %i\n", x);
				dst[x] = EMPTY;
				dst[x - 1] = MAN;
				dst[x - 2] = BOX;
				same[x] = 0;
				same[x - 1] = 0;
				same[x - 2] = 0;
				action[0] = CHOICE_PUSH_LEFT;
				callback(arg, &transition_info, dst, same);
				return 1;
			}
			break;
		}
		case CHOICE_WALK_RIGHT: {
			if ((x < BOARD_WIDTH - 1) && (src[x + 1] == EMPTY)) {
				printf("Walking right at %i\n", x);
				dst[x] = EMPTY;
				dst[x + 1] = MAN;
				same[x] = 0;
				same[x + 1] = 0;
				action[0] = CHOICE_WALK_RIGHT;
				callback(arg, &transition_info, dst, same);
				return 1;
			}
			break;
		}
		case CHOICE_PUSH_RIGHT: {
			if ((x < BOARD_WIDTH - 2) && (src[x + 2] == EMPTY) && (src[x + 1] == BOX)) {
				printf("Pushing left at %i\n", x);
				dst[x] = EMPTY;
				dst[x + 1] = MAN;
				dst[x + 2] = BOX;
				same[x] = 0;
				same[x + 1] = 0;
				same[x + 2] = 0;
				action[0] = CHOICE_PUSH_RIGHT;
				callback(arg, &transition_info, dst, same);
			}
			break;
		}
		default: {
			return 0;
		}
	}
}

int initial[BOARD_WIDTH];
int* initial_state() {
	initial[0] = EMPTY;
	initial[1] = BOX;
	initial[2] = MAN;
	initial[3] = BOX;
	initial[4] = EMPTY;
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
	return label == LABEL_GOAL && src[0] == BOX && src[4] == BOX;
}
