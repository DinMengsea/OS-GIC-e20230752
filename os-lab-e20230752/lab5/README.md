# OS Lab 5 — Threads, Kernel Workers & Process Signals

**Student ID:** e20230752  
**Student Name:** Din Mengsea  
**Environment:** Ubuntu Linux  
**Lab:** 5 — POSIX Threads (Pthreads)

---

## 1. Objectives

The objectives of this lab are:

1. Create POSIX threads using `pthread_create()`.
2. Use `pthread_join()` to wait for worker threads to finish.
3. Check the return codes of pthread functions.
4. Understand shared memory between threads.
5. Understand the difference between thread synchronization and mutual exclusion.
6. Use execution traces to identify race conditions.

## 2. Personal Values

The following personal values were assigned for this lab:

| Variable | Value |
|---|---:|
| Worker A ID (`id_a`) | 2 |
| Worker B ID (`id_b`) | 6 |
| Worker A Steps (`steps_a`) | 5 |
| Worker B Steps (`steps_b`) | 3 |

### Expected Results

Each worker adds its ID to its result during every step.

**Worker A:**

\[
a.result = 2 \times 5 = 10
\]

**Worker B:**

\[
b.result = 6 \times 3 = 18
\]

Expected final output:

```text
A result=10 B result=18
```

## 3. Implementation

### 3.1 Thread Creation

The program creates two worker threads using `pthread_create()`.

```c
pthread_create(&ta, NULL, worker, &a);
pthread_create(&tb, NULL, worker, &b);
```

Each worker receives its own job structure containing its ID, number of steps, and result.

### 3.2 Thread Synchronization

The main function uses `pthread_join()` to wait for both workers.

```c
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
```

Each return code is checked to detect errors.

### 3.3 Reporting Results

After both workers finish, the main function prints their results.

```c
printf("A result=%d B result=%d\n", a.result, b.result);
```

This ensures that main reads the results after both workers have completed.

## 4. Testing and Results

### 4.1 Normal Test

The original worker values were used:

- Worker A: ID = 2, Steps = 5.
- Worker B: ID = 6, Steps = 3.

Command:

```bash
gcc -Wall -Wextra -pthread threads/two_workers.c -o threads/two_workers
timeout 5 ./threads/two_workers
```

Expected result:

```text
worker 2 finished
worker 6 finished
A result=10 B result=18
main finishing
```

The order of worker completion messages may differ because the threads execute concurrently.

### 4.2 Changed Test

A copy of the original program was created to test different step counts.

```bash
cp -- threads/two_workers.c threads/changed.c
```

Changed values:

| Worker | ID | Steps | Expected Result |
|---|---:|---:|---:|
| A | 2 | 0 | 0 |
| B | 6 | 1 | 6 |

Expected output:

```text
worker 2 finished
worker 6 finished
A result=0 B result=6
main finishing
```

This test demonstrates that a worker with zero steps still needs to be joined.

## 5. Plus — Testing Without Joining Worker B

A copy of the original program was created, and the join operation for Worker B was removed.

The program was executed three times.

All three runs produced:

```text
A result=10 B result=18
```

However, this does not guarantee that the program is correct.

Without joining Worker B, main may read `b.result` before Worker B finishes its calculations. Therefore, the result may be incomplete.

This experiment demonstrates why thread completion must be synchronized.

## 6. Challenge — Race Condition

The provided `trace.csv` contains the following execution order:

```text
thread,step
A,read
B,read
A,write
B,write
```

The shared counter initially has a value of 0.

| Step | Thread | Operation | Counter |
|---|---|---|---:|
| 1 | A | Read 0 | 0 |
| 2 | B | Read 0 | 0 |
| 3 | A | Write 1 | 1 |
| 4 | B | Write 1 | 1 |

**Final counter value: 1**

The expected value would be 2, but both workers read the original value before either writes its update.

Worker B overwrites Worker A's update. This problem is called a race condition, specifically a lost update.

A mutex can protect the shared counter by ensuring that only one worker accesses the critical section at a time.

## 7. Important Concepts Learned

| Concept | Explanation |
|---|---|
| `pthread_create()` | Creates and starts a new thread. |
| `pthread_join()` | Waits for a specified thread to finish. |
| Shared address space | Threads within the same process share memory. |
| Race condition | Program results can depend on the execution order of threads. |
| Lost update | One thread overwrites another thread's update. |
| Mutex | Protects shared resources from concurrent access. |
| Critical section | A section of code that accesses shared resources. |

### Thread Synchronization vs Mutual Exclusion

Thread synchronization using `pthread_join()` ensures that a worker finishes before main continues.

Mutual exclusion using a mutex protects shared data when multiple threads access or modify it concurrently.

Therefore, joining threads does not automatically prevent race conditions during their execution.

## 8. Checkpoint

The live checkpoint involved:

- Analyzing a program that joins Worker A but does not join Worker B.
- Understanding why reading an unjoined worker's result is unsafe.
- Identifying the required number of join operations for multiple workers.
- Creating a copy of the program with new step counts.
- Compiling and executing the copied program.
- Saving the output in `checkpoint.txt`.

The checkpoint was completed using the assigned instructions.

## 9. Conclusion

In this lab, I learned how to create and manage POSIX threads using `pthread_create()` and `pthread_join()`.

I also learned that threads share the same address space, allowing them to access shared data. However, concurrent access to shared mutable data can cause race conditions and lost updates.

The experiments demonstrated that joining a thread ensures its completion before reading its result, while mutexes are needed to protect shared resources from concurrent modifications.

Overall, this lab helped me understand thread creation, synchronization, shared memory, and race conditions in Linux.
