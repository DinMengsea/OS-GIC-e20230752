
/* forkchild.c — Process creation with fork() and exec() */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // Display parent process information
    printf("Parent process (PID: %d) — creating child...\n", getpid());

    // Create a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // CHILD PROCESS
        printf("Child process (PID: %d, Parent PID: %d)\n",
               getpid(), getppid());

        printf("Child: sleeping 30s — observe the process tree now!\n");

        // Sleep for 30 seconds
        sleep(30);

        printf("Child: now executing 'ls -la'...\n");

        // Replace child process with ls -la
        execlp("ls", "ls", "-la", NULL);

        // Only executes if execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else {
        // PARENT PROCESS
        int status;

        printf("Parent: waiting for child (PID: %d) to finish...\n", pid);

        // Wait until child finishes
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // Check child's exit status
        if (WIFEXITED(status)) {
            printf("Parent: child exited with status %d\n",
                   WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status)) {
            printf("Parent: child terminated by signal %d\n",
                   WTERMSIG(status));
        }

        printf("Parent: done.\n");
    }

    return 0;
}

