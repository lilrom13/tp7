class ExecutorRunnable implements Runnable {
    int id;

    ExecutorRunnable (int id) {
        this.id = id;
    }

    public void run(){
        Utils.printLog (Utils.elapsedTime(),
                        "initiate",
                        (int)Utils.wcet[id],
                        (int)Utils.period);
        try {Thread.sleep(Utils.wcet[id]);} catch (Exception e){};
        Utils.printLog (Utils.elapsedTime(),
                        "complete",
                        (int)Utils.wcet[id],
                        (int)Utils.period);
    }
}
