/************************************************
 *
 * Author: Michael Mathews
 * Assignment: Program 2
 * Class: CSI 4337
 * File: Mult.java
 *
 ************************************************/

/**
 * Computes the f(x) for a given range of numbers with a specified number of
 * threads
 */
public class Mult {
    private static double product = 1.0;
    private static final Object mutex = new Object();

    /**
     * Function computed at each value in thread
     *
     * @param x value in thread
     * @return value computed from thread value
     */
    public static double f(int x) {
        return (x == 0) ? 1.0 : (double) x;
    }

    /**
     * Class with function for each thread to execute and number range
     * for execution.
     */
    public static class ThreadFunct extends Thread {
        int start, end;

        /**
         * Constructs ThreadFunct that holds a start and end index
         *
         * @param start starting number to compute f(x)
         * @param end ending number to compute f(x)
         */
        public ThreadFunct(int start, int end) {
            this.start = start;
            this.end = end;
        }

        /**
         * Computes one or more f(x) functions efficiently and multiplies them
         */
        @Override
        public void run() {
            double threadProd = 1.0;
            for (int x = start; x < end; x++) {
                double fx = f(x);
                if (fx == 0) {
                    threadProd = 0;
                    break;
                }
                threadProd *= fx;
            }

            // Mutex for shared resource updating
            synchronized (mutex) {
                product *= threadProd;
            }
        }
    }

    /**
     * Distributes work among threads to compute f(x) for a given range of
     * numbers
     *
     * @param args range of numbers to compute f(x) for and number of desired
     *             threads
     */
    public static void main(String[] args) {
        // Verify the number of arguments
        if (args.length != 2) {
            System.err.println("Usage: java Mult <N> <T>");
            System.exit(1);
        }

        // Read and verify input
        int N = 0, T = 0;
        try {
            N = Integer.parseInt(args[0]);
            T = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            System.err.println("Error: Invalid N or T value");
            System.exit(1);
        }

        // End immediately if 0!
        if (N == 0) {
            System.out.println("Answer: 1");
            System.exit(0);
        }
        N += 1;

        if (N < 0 || T < 1 || T > N) {
            System.err.println("Error: Invalid N or T value");
            System.exit(1);
        }

        Thread[] threads = new Thread[T];

        // Calculate range for each thread
        int rangePerChild = N / T;
        int remainder = N % T;

        for (int i = 0; i < T; i++) {
            // Split up work
            int start = i * rangePerChild;
            int end = start + rangePerChild;

            if (i < remainder) {
                start += i;
                end += i + 1;
            } else {
                start += remainder;
                end += remainder;
            }

            // Create and run threads
            threads[i] = new ThreadFunct(start, end);
            threads[i].start();
        }

        // Wait for threads to finish
        for (int i = 0; i < T; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.printf("Answer: %.0f%n", product);
    }
}

