
/* receiver.c — Receives a message from a POSIX message queue */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include "common.h"

int main(void) {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];

    /* Open the existing message queue (read-only) */
    mq = mq_open(QUEUE_NAME, O_RDONLY);

    if (mq == (mqd_t)-1) {
        perror("mq_open");
        fprintf(stderr, "Hint: Did you run sender first?\n");
        exit(EXIT_FAILURE);
    }

    /* Get queue attributes */
    if (mq_getattr(mq, &attr) == -1) {
        perror("mq_getattr");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }

    /* Receive a message from the queue */
    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("mq_receive");
        mq_close(mq);
        exit(EXIT_FAILURE);
    }

    /* Ensure the message is null-terminated */
    buffer[MAX_SIZE] = '\0';

    printf("Receiver: message received from queue '%s'\n", QUEUE_NAME);
    printf("Receiver: message = \"%s\"\n", buffer);

    /* Close the queue */
    mq_close(mq);

    /* Remove the queue */
    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("mq_unlink");
        exit(EXIT_FAILURE);
    }

    printf("Receiver: queue unlinked.\n");

    return 0;
}
