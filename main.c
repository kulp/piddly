#include "piddle.h"

#include <stdio.h>

static pid_value adjustment, current;

static int output_val(pid_t pid, pid_value mv)
{
    adjustment = mv;
    printf("%f\n", mv);
    return 0;
}

static int want_val(pid_t pid, pid_value *sp)
{
    *sp = 5;
    return 0;
}

static int input_val(pid_t pid, pid_value *pv)
{
    char buf[100];
    fgets(buf, sizeof buf, stdin);
    if (sscanf(buf, "%f", pv) < 1) {
        pid_value sp;
        want_val(pid, &sp);
        *pv = current + adjustment;
        printf("Using %f\n", *pv);
    }

    current = *pv;

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t pid;
    pid_init(pid, 0.5, 0.2, 0.1);
    pid_loop(pid, want_val, input_val, output_val);

    return 0;
}

