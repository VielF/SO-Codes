/**
 * Creating a Java thread pool.
 */

import java.util.concurrent.*;

class Task implements Runnable
{
    public void run() {
        System.out.println(Thread.currentThread().getName());
    }
}

public class ThreadPoolExample
{
    public static void main(String[] args) {
        int numTasks = Integer.parseInt(args[0].trim());
        
        ExecutorService pool = Executors.newCachedThreadPool();
        
        for (int i = 0; i < numTasks; i++)
            pool.execute(new Task());

            // The following invovles using a lambda function
            // pool.execute( () -> {System.out.println(Thread.currentThread().getName());});
            
        pool.shutdown();
    }
}
