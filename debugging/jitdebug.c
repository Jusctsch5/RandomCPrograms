
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int arc, char* argv)
{
    pid_t pid;
    char gdb_launch_str[1024] = {};
    char  *char_pointer = &gdb_launch_str[0];
    // -ex='set confirm off' -ex=run -ex 'thread apply all bt' -ex=quit --args
    pid = getpid();
    char_pointer += sprintf(gdb_launch_str,
                            "screen -d -m gdb -p %d",
                            pid);
    printf("gdb_launch_str");

    system(gdb_launch_str);
    return 0;
}