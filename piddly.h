/*
 * piddly : a little PID controller
 * Copyright © Darren Kulp, darren@kulp.ch
 */

#ifndef PIDDLE_H_
#define PIDDLE_H_

#include <stdint.h>

typedef int32_t fix16p16;

/* using #define instead of typedef is intentional here */
#define pid_coeff fix16p16 /*< coefficient to P, I, D */
#define pid_value fix16p16 /*< Process Value or Manipulated Value */

typedef struct {
	struct pid_impl *impl;
	char _priv[64];
} pid_t[1], *pid_tp;

typedef int pid_measure(pid_t pid, pid_value *pv);
typedef int pid_control(pid_t pid, pid_value mv);

int pid_init(pid_t pid, pid_coeff p, pid_coeff i, pid_coeff d);
int pid_loop(pid_t pid, pid_measure *want, pid_measure *get, pid_control *set);

#endif

