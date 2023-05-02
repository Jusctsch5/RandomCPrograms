#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void send_signal(pid_t pid, int signo, int code) {
    // Set up the siginfo structure
    siginfo_t siginfo;

    memset(&siginfo, 0, sizeof(siginfo));
    siginfo.si_signo = signo;
    siginfo.si_code = code;
    siginfo.si_pid = getpid();
    siginfo.si_uid = getuid();
    siginfo.si_addr = NULL;

    union sigval value;
    value.sival_ptr = &siginfo;

    // Send the signal with siginfo to the target process
    if (sigqueue(pid, signo, value) == -1) {
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <process_id> <sig_number> <code>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the process ID and signal number arguments
    pid_t pid = atoi(argv[1]);
    int signo = atoi(argv[2]);
    int code = atoi(argv[3]);

    // Send the signal with siginfo to the target process
    send_signal(pid, signo, code); // Replace with the appropriate signal code

    printf("Sent signal %d with siginfo to process %d\n", signo, pid);

    return 0;
}