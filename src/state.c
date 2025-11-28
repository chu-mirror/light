#include "light.h"
#include "state.h"
#include "list.h"
#include "closure.h"

struct state {
    State parent;
    StateType type;
    List children;
    State active_child;
    State last_child;
    State (*handler)(State, Signal);
    State (*in)(State, Signal);
    State (*out)(State, Signal);
    void *local;
};

void
new_state(State *s_r, State p, StateType t, State (*hd)(State, Signal))
{
    NEW0(*s_r);
    (*s_r)->parent = p;
    (*s_r)->type = t;
    (*s_r)->handler = hd;
    if (p != root_state) {
        push(*s_r, &p->children);
    }
}

void
free_state(State *s_r)
{
    free_list(&(*s_r)->children);
    FREE(*s_r);
}

void
state_register_in_func(State s, State (*in)(State, Signal))
{
    s->in = in;
}

void
state_register_out_func(State s, State (*out)(State, Signal))
{
    s->out = out;
}

static void
into_state(State s, Signal sig)
{
    if (s->in != NULL) {
        State cld;
        cld = CLOSURE(s->in)(s, sig);
        s->active_child = cld == NULL ? s->last_child : cld;
    }
    if (s->type == STATE_XOR && s->active_child) {
        into_state(s->active_child, sig);
    }
    if (s->type == STATE_AND) {
        State cld;
        FOREACH(cld, s->children) {
            into_state(cld, sig);
        }
    }
}

static void
out_state(State s, Signal sig)
{
    if (s->type == STATE_XOR && s->active_child) {
        out_state(s->active_child, sig);
    }
    if (s->type == STATE_AND) {
        State cld;
        FOREACH(cld, s->children) {
            out_state(cld, sig);
        }
    }
    if (s->out != NULL) {
        State cld;
        cld = CLOSURE(s->out)(s, sig);
        s->last_child = cld == NULL ? s->active_child : cld;
    }
}

void
state_init(State s)
{
    into_state(s, NULL);
}

void
state_clear(State s)
{
    out_state(s, NULL);
}

bool
state_handle_signal(State s, Signal sig)
{
    State next;
    bool handled = false;

    next = s->handler == NULL ? NULL : CLOSURE(s->handler)(s, sig);
    if (next == NULL) {
        if (s->type == STATE_XOR && s->active_child != NULL) {
            handled = state_handle_signal(s->active_child, sig);
        }
        if (s->type == STATE_AND) {
            State cld;
            FOREACH(cld, s->children) {
                handled = state_handle_signal(cld, sig);
                if (handled) {
                    break;
                }
            }
        }
    } else {
        handled = true;
        assert(next->parent == s->parent);
        if (next != s) {
            assert(s->parent != root_state && s->parent->type == STATE_XOR);
            out_state(s, sig);
            s->parent->active_child = next;
            into_state(next, sig);
        }
    }
    return handled;
}

void **
state_local(State s)
{
    return &s->local;
}

State
state_active_child(State s)
{
    return s->active_child;
}
