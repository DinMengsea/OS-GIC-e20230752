
#include <iostream>
#include <thread>
#include <semaphore>
#include <mutex>

using namespace std;

// Semaphores
binary_semaphore start_h(1);
binary_semaphore after_e(0);
binary_semaphore after_l2(0);

mutex output_mutex;

// Process 1: Prints H and E
void process1() {
    start_h.acquire();

    {
        lock_guard<mutex> lock(output_mutex);
        cout << "H" << flush;
        cout << "E" << flush;
    }

    after_e.release();
}

// Process 2: Prints L and L
void process2() {
    after_e.acquire();

    {
        lock_guard<mutex> lock(output_mutex);
        cout << "L" << flush;
        cout << "L" << flush;
    }

    after_l2.release();
}

// Process 3: Prints O
void process3() {
    after_l2.acquire();

    {
        lock_guard<mutex> lock(output_mutex);
        cout << "O" << flush;
    }
}

int main() {
    cout << "=== HELLO AFTER Semaphores ===" << endl;
    cout << "Starting three concurrent processes..." << endl;

    // Create three concurrent threads
    thread p1(process1);
    thread p2(process2);
    thread p3(process3);

    // Wait for all threads to finish
    p1.join();
    p2.join();
    p3.join();

    cout << endl;
    cout << "Final output: HELLO" << endl;
    cout << "All processes completed successfully." << endl;

    return 0;
}
