#define _GNU_SOURCE

#include <errno.h>
#include <linux/sched.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct rlimit rl;

    if (getrlimit(RLIMIT_STACK, &rl))
    {
        perror("failed to get max stack size");
        return EXIT_FAILURE;
    }

    printf("soft stack size limit: %luB\n", rl.rlim_cur);
    void *child_stack = malloc(rl.rlim_cur);

    /* we use clone3 over clone because it has a more modern API with additional flexibility
       however glibc does not expose a wrapper for clone3 so we invoke it manually via `syscall' */

    struct clone_args cl_args;
    size_t size = sizeof(cl_args);

    return EXIT_SUCCESS;
}