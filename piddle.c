/*
 * piddle : a little PID controller
 * Copyright © Darren Kulp, darren@kulp.ch
 */

#include "piddle.h"

struct pid_impl {
    pid_coeff p, i, d;
};

int pid_init(pid_t pid, pid_coeff p, pid_coeff i, pid_coeff d)
{
    pid->impl = (void*)pid->_priv;
    pid->impl->p = p;
    pid->impl->i = i;
    pid->impl->d = d;

    return 0;
}

int pid_loop(pid_t pid, pid_measure *want, pid_measure *get, pid_control *set)
{
    pid_value pv = 0,
              sp = 0;
    while (!want(pid, &sp) && !get(pid, &pv)) {
        pid_value e  = sp - pv;
        pid_value mv = pid->impl->p * e;
        set(pid, mv);
    }

    return 0;
}

