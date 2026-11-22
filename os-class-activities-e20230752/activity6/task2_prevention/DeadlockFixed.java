
import java.util.concurrent.Semaphore;

class Bank {
    int balanceA = 1000;
    int balanceB = 1000;

    // One shared mutex with one permit
    static Semaphore mutex = new Semaphore(1);

    void transfer(String worker, String from, String to, int amount) {
        boolean acquired = false;

        try {
            System.out.println(worker + " waiting for mutex");

            mutex.acquire();
            acquired = true;

            System.out.println(worker + " acquired mutex");

            // Simulate transfer processing
            Thread.sleep(500);

            if (from.equals("A")) {
                balanceA -= amount;
                balanceB += amount;
            } else {
                balanceB -= amount;
                balanceA += amount;
            }

            System.out.println(worker + " transferred " + amount
                    + " from Account " + from
                    + " to Account " + to);

        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();

        } finally {
            if (acquired) {
                mutex.release();
                System.out.println(worker + " released mutex");
            }
        }
    }
}

public class DeadlockFixed {
    public static void main(String[] args)
            throws InterruptedException {

        Bank bank = new Bank();

        int startingTotal = bank.balanceA + bank.balanceB;

        System.out.println("Starting balances:");
        System.out.println("Account A: " + bank.balanceA);
        System.out.println("Account B: " + bank.balanceB);
        System.out.println("Starting total: " + startingTotal);

        Thread t1 = new Thread(
            () -> bank.transfer("Worker 1", "A", "B", 100)
        );

        Thread t2 = new Thread(
            () -> bank.transfer("Worker 2", "B", "A", 200)
        );

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        int finalTotal = bank.balanceA + bank.balanceB;

        System.out.println("\nFinal balances:");
        System.out.println("Final A: " + bank.balanceA);
        System.out.println("Final B: " + bank.balanceB);
        System.out.println("Final total: " + finalTotal);

        if (startingTotal == finalTotal) {
            System.out.println("No deadlock occurred");
        }
    }
}
