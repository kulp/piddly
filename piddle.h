/*
 * piddle : a little PID controller
 * Copyright © Darren Kulp, darren@kulp.ch
 */

#ifndef PIDDLE_H_
#define PIDDLE_H_

typedef struct {
	struct pid_impl *impl;
	char _priv[64];
} pid_t[1], *pid_tp;

typedef float pid_coeff; ///< coefficient to P, I, D
typedef float pid_value; ///< Process Value or Manipulated Value

typedef int pid_measure(pid_t pid, pid_value *pv);
typedef int pid_control(pid_t pid, pid_value mv);

int pid_init(pid_t pid, pid_coeff p, pid_coeff i, pid_coeff d);
int pid_loop(pid_t pid, pid_measure *want, pid_measure *get, pid_control *set);

#endif

