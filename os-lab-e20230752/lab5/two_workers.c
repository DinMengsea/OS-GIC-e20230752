#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
struct job { int id; int steps; int result; };
static void *worker(void *arg) {
    struct job *job = arg;
    const struct timespec delay = {0, 20000000};
    for (int i = 0; i < job->steps; ++i) {
        job->result += job->id;
        nanosleep(&delay, NULL); /* teaching delay, not synchronization */
    }
    printf("worker %d finished\n", job->id);
    return NULL;
}
int main(void) {
    struct job a = {2, 5, 0}, b = {6, 3, 0};
    pthread_t ta, tb;
    int rc = pthread_create(&ta, NULL, worker, &a);
    if (rc) { fprintf(stderr, "create A: %s\n", strerror(rc)); return 1; }
    rc = pthread_create(&tb, NULL, worker, &b);
    if (rc) {
        fprintf(stderr, "create B: %s\n", strerror(rc));
        pthread_join(ta, NULL);
        return 1;
    }
    /* The report line must look like:  A result=NUMBER B result=NUMBER */
    rc = pthread_join(ta, NULL);
	if (rc) {
    	fprintf(stderr, "join A: %s\n", strerror(rc));
    	return 1;
	}

    rc = pthread_join(tb, NULL);
	if (rc) {
	    fprintf(stderr, "join B: %s\n", strerror(rc));
	    return 1;
	}

    printf("A result=%d B result=%d\n", a.result, b.result);
    puts("main finishing");
    return 0;
}
