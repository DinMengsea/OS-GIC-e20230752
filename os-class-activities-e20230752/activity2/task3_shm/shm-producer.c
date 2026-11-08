
/* shm-producer.c — Writes to POSIX shared memory */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    // Size of shared memory region (bytes)
    const int SIZE = 4096;

    // Unique shared memory name
    const char *name = "/OS-mengsea";

    // Messages to write
    const char *message_0 = "Hello, ";
    const char *message_1 = "this is shared memory IPC!";

    int shm_fd;
    void *ptr;

    // Create or open shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set shared memory size
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Map shared memory into process address space
    ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
               MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Write messages into shared memory
    snprintf((char *)ptr, SIZE, "%s%s",
             message_0, message_1);

    printf("Producer: wrote to shared memory '%s'\n", name);

    printf("Producer: message = \"%s\"\n", (char *)ptr);

    // Unmap shared memory
    if (munmap(ptr, SIZE) == -1) {
        perror("munmap");
    }

    // Close file descriptor
    if (close(shm_fd) == -1) {
        perror("close");
    }

    printf("Producer: finished writing.\n");

    return 0;
}
