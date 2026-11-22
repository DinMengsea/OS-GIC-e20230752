
#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <atomic>
#include <chrono>
#include <mutex>

using namespace std;

struct Particle {
    int machine_id;
    int pair_id;
    string type;
};

// Buffer configuration
const int BUFFER_CAPACITY = 100;
const int PAIR_CAPACITY = 50;

vector<Particle> buffer;

// Semaphores
counting_semaphore<50> empty_pairs(PAIR_CAPACITY);
counting_semaphore<50> full_pairs(0);
binary_semaphore buffer_mutex(1);

// Counters
atomic<int> produced_pairs(0);
atomic<int> packaged_pairs(0);
atomic<bool> stop_program(false);

mutex output_mutex;

// Producer function
void producer(int machine_id) {
    int pair_id = 0;

    while (!stop_program) {
        pair_id++;

        // Create two particles belonging to the same pair
        Particle p1 = {machine_id, pair_id, "P1"};
        Particle p2 = {machine_id, pair_id, "P2"};

        // Wait until one pair space is available
        empty_pairs.acquire();

        buffer_mutex.acquire();

        // Check buffer capacity
        if (buffer.size() + 2 > BUFFER_CAPACITY) {
            {
                lock_guard<mutex> lock(output_mutex);
                cout << "The producing machine is broken" << endl;
            }

            stop_program = true;
            buffer_mutex.release();
            return;
        }

        // Add both particles consecutively
        buffer.push_back(p1);
        buffer.push_back(p2);

        produced_pairs++;

        int current_buffer_size = buffer.size();

        buffer_mutex.release();

        // One complete pair is now available
        full_pairs.release();

        {
            lock_guard<mutex> lock(output_mutex);
            cout << "Produced pairs: " << produced_pairs
                 << " | Packaged pairs: " << packaged_pairs
                 << " | Buffer particles: " << current_buffer_size
                 << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

// Consumer function
void consumer() {
    while (!stop_program) {

        // Wait until one complete pair is available
        full_pairs.acquire();

        buffer_mutex.acquire();

        // Check that two particles are available
        if (buffer.size() < 2) {
            {
                lock_guard<mutex> lock(output_mutex);
                cout << "The packaging machine is broken" << endl;
            }

            stop_program = true;
            buffer_mutex.release();
            return;
        }

        // Remove two consecutive particles
        Particle p1 = buffer[0];
        Particle p2 = buffer[1];

        buffer.erase(buffer.begin(), buffer.begin() + 2);

        // Verify that both particles belong to the same pair
        if (p1.machine_id != p2.machine_id ||
            p1.pair_id != p2.pair_id ||
            p1.type != "P1" ||
            p2.type != "P2") {

            {
                lock_guard<mutex> lock(output_mutex);
                cout << "Pairs are incorrect" << endl;
            }

            stop_program = true;
            buffer_mutex.release();
            return;
        }

        packaged_pairs++;

        int current_buffer_size = buffer.size();

        buffer_mutex.release();

        // Two buffer slots are available again
        empty_pairs.release();

        {
            lock_guard<mutex> lock(output_mutex);
            cout << "Produced pairs: " << produced_pairs
                 << " | Packaged pairs: " << packaged_pairs
                 << " | Buffer particles: " << current_buffer_size
                 << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(150));
    }
}

int main() {
    cout << "=== Particle Buffer AFTER Semaphores ===" << endl;
    cout << "Producer machines: 3" << endl;
    cout << "Buffer capacity: 100 particles (50 pairs)" << endl;
    cout << "Press Ctrl+C to stop the simulation." << endl;
    cout << "----------------------------------------" << endl;

    // Create three producer threads
    thread p1(producer, 1);
    thread p2(producer, 2);
    thread p3(producer, 3);

    // Create one consumer thread
    thread c(consumer);

    // Keep the simulation running
    p1.join();
    p2.join();
    p3.join();
    c.join();

    return 0;
}
