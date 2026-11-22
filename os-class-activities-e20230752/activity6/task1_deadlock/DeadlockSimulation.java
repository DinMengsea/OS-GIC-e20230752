
import java.util.concurrent.Semaphore;
import java.util.concurrent.CountDownLatch;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {
    static CountDownLatch ready = new CountDownLatch(2);

    static void transfer(Account from, Account to, int amount) {
        try {
            String worker = Thread.currentThread().getName();

            System.out.println(worker + " trying to lock FROM " + from.name);
            from.lock.acquire();

            System.out.println(worker + " locked FROM " + from.name);

            ready.countDown();
            ready.await();

            Thread.sleep(100);

            System.out.println(worker + " waiting for TO " + to.name);
            to.lock.acquire();

            System.out.println(worker + " locked TO " + to.name);

            from.balance -= amount;
            to.balance += amount;

            System.out.println(worker + " transfer completed");

            to.lock.release();
            from.lock.release();

        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class DeadlockSimulation {
    public static void main(String[] args)
            throws InterruptedException {

        Account accountA = new Account("Account A", 1000);
        Account accountB = new Account("Account B", 1000);

        System.out.println("Starting balances:");
        System.out.println("Account A: " + accountA.balance);
        System.out.println("Account B: " + accountB.balance);

        Thread t1 = new Thread(
            () -> Transfer.transfer(accountA, accountB, 100),
            "Worker 1"
        );

        Thread t2 = new Thread(
            () -> Transfer.transfer(accountB, accountA, 200),
            "Worker 2"
        );

        t1.setDaemon(true);
        t2.setDaemon(true);

        t1.start();
        t2.start();

        t1.join(2000);
        t2.join(2000);

        if (t1.isAlive() || t2.isAlive()) {
            System.out.println();
            System.out.println(
                "Deadlock detected: transactions are stuck"
            );

            System.out.println("Worker 1 state: " + t1.getState());
            System.out.println("Worker 2 state: " + t2.getState());

            System.out.println("Final Account A: " + accountA.balance);
            System.out.println("Final Account B: " + accountB.balance);
        } else {
            System.out.println("Both transfers completed.");
        }
    }
}
