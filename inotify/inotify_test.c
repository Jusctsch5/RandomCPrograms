#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    const char* directory = argv[1];

    int fd, wd;
    char buffer[BUF_LEN];

    // Create an inotify instance
    fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Add a watch to monitor the specified directory
    wd = inotify_add_watch(fd, directory, IN_CREATE | IN_DELETE);
    if (wd == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Watching directory: %s\n", directory);

    while (1) {
        int i = 0;
        int length = read(fd, buffer, BUF_LEN);

        if (length == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];

            if (event->mask & IN_CREATE) {
                printf("File created: %s\n", event->name);
            }

            if (event->mask & IN_DELETE) {
                printf("File deleted: %s\n", event->name);
            }

            i += EVENT_SIZE + event->len;
        }
    }

    // Remove the watch and close the inotify instance
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}