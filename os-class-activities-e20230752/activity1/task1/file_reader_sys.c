
/* file_reader_sys.c */
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[256];

    // 1. Open "output.txt" for reading
    int fd = open("output.txt", O_RDONLY);

    if (fd == -1) {
        write(2, "Error opening file!\n", 20);
        return 1;
    }

    // 2. Read content into buffer in a loop
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        // 3. Write the content to the terminal (fd 1)
        write(1, buffer, bytesRead);
    }

    // 4. Close the file
    close(fd);

    return 0;
}
