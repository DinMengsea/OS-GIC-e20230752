
/* dir_list_sys.c */
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

int main() {
    char buffer[512];

    // 1. Open the current directory
    DIR *dir = opendir(".");

    if (dir == NULL) {
        write(2, "Error opening directory!\n", 25);
        return 1;
    }

    // 2. Print the header line
    char header[] = "Name\t\t\tSize (bytes)\n";
    write(1, header, strlen(header));

    // 3. Loop through directory entries
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        struct stat fileStat;

        // 4. Get file information and size
        if (stat(entry->d_name, &fileStat) == -1) {
            continue;
        }

        // 5. Format the output into buffer
        int len = snprintf(buffer, sizeof(buffer),
                           "%-24s %ld\n",
                           entry->d_name,
                           (long)fileStat.st_size);

        // Write the formatted output to the terminal
        if (len > 0 && len < sizeof(buffer)) {
            write(1, buffer, len);
        }
    }

    // 6. Close the directory
    closedir(dir);

    return 0;
}
