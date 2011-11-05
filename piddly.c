/*
 * piddly : a little PID controller
 * Copyright © Darren Kulp, darren@kulp.ch
 */

#include "piddly.h"

#define PASTE(X,Y) PASTE_(X,Y)
#define PASTE_(X,Y) X ## Y

#define OP(Op,Args) PASTE(op_,PASTE(pid_value,PASTE(_,Op))) Args

#define op_float_add(x,y) ((x) + (y))
#define op_float_sub(x,y) ((x) - (y))
#define op_float_mul(x,y) ((x) * (y))

#define op_double_add(x,y) ((x) + (y))
#define op_double_sub(x,y) ((x) - (y))
#define op_double_mul(x,y) ((x) * (y))

#define op_fix16p16_add(x,y) ((x) + (y))
#define op_fix16p16_sub(x,y) ((x) - (y))
#define op_fix16p16_mul(x,y) (((uint64_t)(x) * (uint64_t)(y)) >> 16)

#define STATIC_ASSERT(Cond) \
    typedef int PASTE(assert_type_,__LINE__)[(Cond) ? 1 : -1]

#define countof(X) (sizeof (X) / sizeof (X)[0])

static pid_t example;

struct pid_impl {
    pid_coeff p;        /*< coefficient for proportional term */
    pid_coeff i;        /*< coefficient for integral     term */
    pid_coeff d;        /*< coefficient for derivative   term */

    pid_value Se;       /*< sum of errors */
    pid_value le[2];    /*< last error circular buffer */
    int lei;            /*< last error index */
};

STATIC_ASSERT(sizeof(struct pid_impl) <= sizeof(example->_priv));

int pid_init(pid_t pid, pid_coeff p, pid_coeff i, pid_coeff d)
{
    struct pid_impl *P = pid->impl = (void*)pid->_priv;

    P->p = p;
    P->i = i;
    P->d = d;

    P->Se  =  0;
    P->lei = -1;

    return 0;
}

int pid_loop(pid_t pid, pid_measure *want, pid_measure *get, pid_control *set)
{
    pid_value pv = 0,
              sp = 0;

    while (!want(pid, &sp) && !get(pid, &pv)) {
        struct pid_impl *P = pid->impl;

        pid_value e  = OP(sub,(sp, pv));
        pid_value pe = OP(mul,(P->p, e));
        pid_value ie = OP(mul,(P->i, P->Se));
        pid_value de = OP(mul,(P->d, OP(sub,(e, P->le[P->lei]))));

        pid_value mv;

        if (P->lei == -1) de = 0; /* start condition */

        mv = OP(add,(pe, OP(add,(ie, de))));

        set(pid, mv);

        P->Se = OP(add,(P->Se, e));
        P->lei = (P->lei + 1) % countof(P->le);
        P->le[P->lei] = e;
    }

    return 0;
}

