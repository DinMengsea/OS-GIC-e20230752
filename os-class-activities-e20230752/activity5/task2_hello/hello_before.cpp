
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

mutex output_mutex;

// Print one letter safely without controlling its execution order
void print_letter(char letter) {
    {
        lock_guard<mutex> lock(output_mutex);
        cout << letter << flush;
    }
}

// Process 1: Prints H and E
void process1() {
    print_letter('H');

    this_thread::sleep_for(chrono::milliseconds(10));

    print_letter('E');
}

// Process 2: Prints L and L
void process2() {
    print_letter('L');

    this_thread::sleep_for(chrono::milliseconds(10));

    print_letter('L');
}

// Process 3: Prints O
void process3() {
    print_letter('O');
}

int main() {
    cout << "=== HELLO BEFORE Semaphores ===" << endl;
    cout << "Starting three concurrent processes..." << endl;

    // Create three concurrent threads without ordering semaphores
    thread p1(process1);
    thread p2(process2);
    thread p3(process3);

    // Wait for all threads to finish
    p1.join();
    p2.join();
    p3.join();

    cout << endl;
    cout << "Simulation completed without semaphore ordering." << endl;

    return 0;
}
