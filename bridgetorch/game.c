#include <ltsmin/pins.h>

// state slot values
#define PERSON_A 0
#define PERSON_B 1
#define PERSON_C 2
#define PERSON_D 3
#define TORCH 4
#define TIME 5
#define ITEM_COUNT 6

// transition labels
#define SWITCH_A 0
#define SWITCH_B 1
#define SWITCH_C 2
#define SWITCH_D 3
#define SWITCH_AB 4
#define SWITCH_AC 5
#define SWITCH_AD 6
#define SWITCH_BC 7
#define SWITCH_BD 8
#define SWITCH_CD 9
#define GROUP_COUNT 10

// state labels
#define LABEL_GOAL 0

#define LEFTSIDE 0
#define RIGHTSIDE 1

#define TIME_A 1
#define TIME_B 2
#define TIME_C 5
#define TIME_D 8
#define TIME_AB 2
#define TIME_AC 5
#define TIME_AD 8
#define TIME_BC 5
#define TIME_BD 8
#define TIME_CD 8
#define MAX_TIME 15

int group_count() {
	return GROUP_COUNT;
}

int state_length() {
	return ITEM_COUNT;
}

int label_count() {
	return 1;
}

int is_valid(int *dst) {
	return dst[TIME] <= MAX_TIME;
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

int switch2(int *dst, int p1, int p2, int time, int switchcase, int *same, int *action, transition_info_t *tinfo, TransitionCB callback, int *arg) {
	if ((dst[p1] == dst[p2]) && (dst[p1] == dst[TORCH])) {
		if (dst[p1] == RIGHTSIDE) {
			dst[p1] = LEFTSIDE;
			dst[p2] = LEFTSIDE;
			dst[TORCH] = LEFTSIDE;
		} else {
			dst[p1] = RIGHTSIDE;
			dst[p2] = RIGHTSIDE;
			dst[TORCH] = RIGHTSIDE;
		}
		dst[TIME] += time;
		same[p1] = 0;
		same[p2] = 0;
		same[TORCH] = 0;
		action[0] = switchcase;
		if (is_valid(dst)) {
			callback(arg, tinfo, dst, same);
			return 1;
		}
	}
	return 0;
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
		case SWITCH_A:	return switch1(dst, PERSON_A, TIME_A, SWITCH_A, same, action, &tinfo, callback, arg); break;
		case SWITCH_B:	return switch1(dst, PERSON_B, TIME_B, SWITCH_B, same, action, &tinfo, callback, arg); break;
		case SWITCH_C:	return switch1(dst, PERSON_C, TIME_C, SWITCH_C, same, action, &tinfo, callback, arg); break;
		case SWITCH_D:	return switch1(dst, PERSON_D, TIME_D, SWITCH_D, same, action, &tinfo, callback, arg); break;
		case SWITCH_AB:	return switch2(dst, PERSON_A, PERSON_B, TIME_AB, SWITCH_AB, same, action, &tinfo, callback, arg); break;
		case SWITCH_AC:	return switch2(dst, PERSON_A, PERSON_C, TIME_AC, SWITCH_AC, same, action, &tinfo, callback, arg); break;
		case SWITCH_AD:	return switch2(dst, PERSON_A, PERSON_D, TIME_AD, SWITCH_AD, same, action, &tinfo, callback, arg); break;
		case SWITCH_BC:	return switch2(dst, PERSON_B, PERSON_C, TIME_BC, SWITCH_BC, same, action, &tinfo, callback, arg); break;
		case SWITCH_BD:	return switch2(dst, PERSON_B, PERSON_D, TIME_BD, SWITCH_BD, same, action, &tinfo, callback, arg); break;
		case SWITCH_CD: return switch2(dst, PERSON_C, PERSON_D, TIME_CD, SWITCH_CD, same, action, &tinfo, callback, arg); break;
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
