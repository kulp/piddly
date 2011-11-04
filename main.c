#include "piddle.h"

#include <stdio.h>

static int output_val(pid_t pid, pid_value mv)
{
    printf("%f\n", mv);
    return 0;
}

static int input_val(pid_t pid, pid_value *pv)
{
    char buf[100];
    fgets(buf, sizeof buf, stdin);
    sscanf(buf, "%f", pv);
    return 0;
}

static int want_val(pid_t pid, pid_value *sp)
{
    *sp = 5;
    return 0;
}

int main(int argc, char *argv[])
{
    pid_t pid;
    pid_init(pid, 1, 1, 1);
    pid_loop(pid, want_val, input_val, output_val);

    return 0;
}

