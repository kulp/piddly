#include "piddly.h"

#include <stdio.h>
#include <math.h>

#define PASTE(X,Y) PASTE_(X,Y)
#define PASTE_(X,Y) X ## Y

#define doubleto(Type,What) PASTE(doubleto,Type)(What)
#define print(Type,What) PASTE(print,Type)(What)

#define MASK16 ((1 << 16) - 1)

static pid_value adjustment, current;

static fix16p16 doubletofix16p16(double d)
{
    fix16p16 result = 0;
    double p;
    double r = modf(d, &p);
    result = ((int64_t)p << 16) | ((int64_t)r & MASK16);
    return result;
}

static double doubletodouble(double d)
{
    return d;
}

static float doubletofloat(double d)
{
    return d;
}

static char* printdouble(double d)
{
    static char buf[128];
    snprintf(buf, sizeof buf, "%f", d);

    return buf;
}

static char* printfloat(float d)
{
    static char buf[128];
    snprintf(buf, sizeof buf, "%f", d);

    return buf;
}

static char* printfix16p16(fix16p16 d)
{
    static char buf[128];
    snprintf(buf, sizeof buf, "%6d.%06d", (d >> 16) & MASK16, d & MASK16);

    return buf;
}

static int output_val(pid_t pid, pid_value mv)
{
    adjustment = mv;
    printf("%s\n", print(pid_value, mv));
    return 0;
}

static int want_val(pid_t pid, pid_value *sp)
{
    *sp = doubleto(pid_value, 5);
    return 0;
}

static int input_val(pid_t pid, pid_value *pv)
{
    char buf[100];
    double what;
    fgets(buf, sizeof buf, stdin);
    if (sscanf(buf, "%lf", &what) < 1) {
        pid_value sp;
        want_val(pid, &sp);
        *pv = current + adjustment;
        printf("Using %s\n", print(pid_value,*pv));
    } else {
        *pv = doubleto(pid_value, what);
    }

    current = *pv;

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t pid;
    pid_init(pid, 1 << 15, 1 << 14, 1 << 13);
    pid_loop(pid, want_val, input_val, output_val);

    return 0;
}

