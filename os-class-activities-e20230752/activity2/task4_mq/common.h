
/* common.h — Shared constants for message queue IPC */

#ifndef COMMON_H
#define COMMON_H

/*
 * Unique POSIX message queue name.
 * The name must start with '/'.
 * Both sender and receiver must use the same name.
 */

#define QUEUE_NAME  "/queue-mengsea"
#define MAX_SIZE    1024

#endif
