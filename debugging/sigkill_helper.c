#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>
#include <sys/syscall.h>

// initialize u_int64_t value with a funny hex value
u_int64_t value = 0x619000002380;


int send_signal(pid_t pid, int signo) {
    // Set up the siginfo structure
    siginfo_t siginfo;

    memset(&siginfo, 0, sizeof(siginfo));
    siginfo.si_signo = signo;
    siginfo.si_code = SI_QUEUE; // -1
    siginfo.si_pid = getpid();
    siginfo.si_uid = getuid();
    siginfo.si_addr = (void *)value;
    // Can be found via:
    // ls -la /proc/$target_pid/fd
    siginfo.si_fd = 10;


    // https://man7.org/linux/man-pages/man2/rt_sigqueueinfo.2.html
    // There is no glibc wrapper for this system call; see NOTES above
    int ret = syscall(__NR_rt_sigqueueinfo, pid, signo, &siginfo);
    if (ret) {
        printf("ret:%d\n", ret);
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <process_id> <sig_number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the process ID and signal number arguments
    pid_t pid = atoi(argv[1]);
    int signo = atoi(argv[2]);

    // Send the signal with siginfo to the target process
    send_signal(pid, signo); // Replace with the appropriate signal code

    printf("Sent signal:%d with siginfo to process %d\n", signo, pid);

    return 0;
}

// ./a.out <pid> <sig>
// time=2023-05-03T19:24:24.712963162Z message="SIGBUS at address 0x26b" level=info  process_id=1 thread_id=1 myriad_service=target source_file="/context/source/src/storage/storage_nvme.c:1469"
// time=2023-05-03T19:24:24.713011093Z message="Controller 0x619000002380" level=info  process_id=1 thread_id=1 myriad_service=target source_file="/context/source/src/storage/storage_nvme.c:1472"