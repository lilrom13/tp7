import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

class Utils {

    static long startTime;
    
    static long corePoolSize = 0;
    static long maxPoolSize = 0;
    static long blockingQueueSize = 0;
    static long keepAliveTime = 0;
    static long period = 0;
    static long nbNodes = 0;
    static long wcet[];
    static long preds[][];

    static long getLong(Scanner s, String key) {
        String l;
        
        while (s.hasNextLine()) {
            l = s.nextLine ();
            if (l.compareTo (key) == 0) {
                l = s.nextLine();
                return Long.parseLong (l);
            }
        }
        return 0;
    }
    
    static public void init (String n) {
        Scanner s = null;
        String  l;
        try {
            s = new Scanner (new BufferedReader (new FileReader (n)));
        } catch (Exception e) {
            System.out.println ("file " + n + " not found");
            return;
        } 

        corePoolSize = (int) getLong(s, "#core_pool_size");
        System.out.println("core_pool_size = " + corePoolSize);
        maxPoolSize = (int) getLong(s, "#max_pool_size");
        System.out.println("max_pool_size = " + maxPoolSize);
        blockingQueueSize = (int) getLong(s, "#blocking_queue_size");
        System.out.println("blocking_queue_size = " + blockingQueueSize);
        keepAliveTime = (int) getLong(s, "#keep_alive_time");
        System.out.println("keep_alive_time = " + keepAliveTime);
        period = (int) getLong(s, "#period");
        System.out.println("period = " + period);
        nbNodes = (int) getLong(s, "#nb_nodes");
        System.out.println("nb_nodes = " + nbNodes);
        
        while (s.hasNextLine()) {
            l = s.nextLine ();
            if (l.compareTo ("#wcet") == 0) break;
        }
        wcet = new long[(int) nbNodes];
        for (int i = 0; i < nbNodes; i++) {
            l = s.nextLine();
            wcet[i] = Long.parseLong (l);
        }
        for (int i = 0; i < nbNodes; i++) {
            System.out.println("wcet[" + i + "]=" + wcet[i]);
        }

        while (s.hasNextLine()) {
            l = s.nextLine ();
            if (l.compareTo ("#preds") == 0) break;
        }
        preds = new long[(int)nbNodes][(int)nbNodes];
        for (int i = 0; i < nbNodes; i++) {
            for (int j = 0; j < nbNodes; j++)
                preds[i][j] = s.nextInt();
            s.nextLine();
        }
        for (int i = 0; i < nbNodes; i++) {
            System.out.print ("preds[" + i + "]=");
            for (int j = 0; j < nbNodes; j++)
                System.out.print (" " + preds[i][j]);
            System.out.println("");
        }
        
        startTime = System.currentTimeMillis();
    }
    

    static void delayUntil (long deadline) {
        try {
            Thread.sleep (deadline - System.currentTimeMillis());
        } catch (Exception e){};
    }

    static long elapsedTime(){
        return System.currentTimeMillis() - startTime;
    }

    static void printLog(long time, String msg, int wcet, int period){
        String s = String.format("%1$06d", time);
        System.out.println(s + " " + msg + " execution=" + wcet +
                           " period=" + period);
    }
}
