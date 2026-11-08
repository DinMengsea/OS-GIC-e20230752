
/* shm-consumer.c — Reads from POSIX shared memory */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    // Size must match the producer
    const int SIZE = 4096;

    // Must match the producer's shared memory name
    const char *name = "/OS-mengsea";

    int shm_fd;
    void *ptr;

    // Open existing shared memory object (read-only)
    shm_fd = shm_open(name, O_RDONLY, 0666);

    if (shm_fd == -1) {
        perror("shm_open");
        fprintf(stderr, "Hint: Did you run shm-producer first?\n");
        exit(EXIT_FAILURE);
    }

    // Map shared memory into this process's address space
    ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Read and display the message
    printf("Consumer: reading from shared memory '%s'\n", name);

    printf("Consumer: message = \"%s\"\n", (char *)ptr);

    // Unmap shared memory
    if (munmap(ptr, SIZE) == -1) {
        perror("munmap");
    }

    // Close shared memory file descriptor
    if (close(shm_fd) == -1) {
        perror("close");
    }

    // Remove the shared memory object
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    printf("Consumer: shared memory unlinked.\n");

    return 0;
}
