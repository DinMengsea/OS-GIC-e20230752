
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>
#include <string>

using namespace std;

struct Particle {
    int machine_id;
    int pair_id;
    string type;
};

vector<Particle> buffer;
const int BUFFER_CAPACITY = 100;

mutex buffer_mutex;
atomic<bool> stop_program(false);

void producer(int machine_id) {
    int pair_id = 0;

    while (!stop_program) {
        pair_id++;

        Particle p1 = {machine_id, pair_id, "P1"};
        Particle p2 = {machine_id, pair_id, "P2"};

        // Add the first particle
        {
            lock_guard<mutex> lock(buffer_mutex);

            if (buffer.size() >= BUFFER_CAPACITY) {
                cout << "The producing machine is broken" << endl;
                stop_program = true;
                return;
            }

            buffer.push_back(p1);
        }

        // Allow another thread to interfere
        this_thread::yield();

        // Add the second particle separately
        {
            lock_guard<mutex> lock(buffer_mutex);

            if (buffer.size() >= BUFFER_CAPACITY) {
                cout << "The producing machine is broken" << endl;
                stop_program = true;
                return;
            }

            buffer.push_back(p2);
        }

        this_thread::sleep_for(chrono::milliseconds(5));
    }
}

void consumer() {
    while (!stop_program) {
        Particle p1, p2;

        {
            lock_guard<mutex> lock(buffer_mutex);

            if (buffer.size() < 2) {
                // Wait until two particles are available
                continue;
            }

            p1 = buffer[0];
            buffer.erase(buffer.begin());

            p2 = buffer[0];
            buffer.erase(buffer.begin());
        }

        // Verify that both particles belong to the same pair
        if (p1.machine_id != p2.machine_id ||
            p1.pair_id != p2.pair_id ||
            p1.type != "P1" ||
            p2.type != "P2") {

            cout << "Pairs are incorrect" << endl;
            stop_program = true;
            return;
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

int main() {
    cout << "=== Particle Buffer BEFORE Semaphores ===" << endl;

    thread p1(producer, 1);
    thread p2(producer, 2);
    thread p3(producer, 3);
    thread c(consumer);

    p1.join();
    p2.join();
    p3.join();
    c.join();

    cout << "Simulation stopped." << endl;

    return 0;
}
