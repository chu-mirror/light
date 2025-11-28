#ifndef LIGHT_STATE_H
#define LIGHT_STATE_H

#include <stdbool.h>

typedef void *Signal;
typedef struct state *State;

typedef enum {
    STATE_XOR,
    STATE_AND
} StateType;

#define root_state NULL

void new_state(State *s_r, State p, StateType t, State (*hd)(State, Signal));
void free_state(State *s_r);
void state_register_in_func(State s, State (*in)(State, Signal));
void state_register_out_func(State s, State (*out)(State, Signal));
void state_init(State s);
void state_clear(State s);
bool state_handle_signal(State s, Signal sig);
void **state_local(State s);
State state_active_child(State s);

#endif
