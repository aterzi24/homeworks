package project.parts.logics;

import project.SimulationRunner;
import project.components.Robot;
import project.parts.Arm;
import project.parts.Base;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.List;

public class Inspector extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n", ...);
        // System.out.printf( "Robot %02d : Notifying waiting fixers.%n", ...);
        int no = (int) Common.get(robot, "serialNo");
        synchronized (SimulationRunner.factory.robots) {
            List<Robot> temp = SimulationRunner.factory.robots;
            // check all robots if it is missing any part
            for (Robot r: temp) {
                Base base = (Base) r;
                Arm arm = (Arm) Common.get(base, "arm");
                Payload payload = (Payload) Common.get(base, "payload");
                Logic logic = (Logic) Common.get(base, "logic");
                // if detected then add it to the brokenRobots and notify all fixers
                if ((arm == null) || (payload == null) || (logic == null)) {
                    int no2 = (int) Common.get(r, "serialNo");
                    System.out.printf( "Robot %02d : Detected a broken robot (%02d), adding it to broken robots list.%n", no, no2);
                    synchronized (SimulationRunner.factory.brokenRobots) {
                        SimulationRunner.factory.brokenRobots.add(r);
                        System.out.printf( "Robot %02d : Notifying waiting fixers.%n", no);
                        SimulationRunner.factory.brokenRobots.notifyAll();
                        return;
                    }
                }
            }
        }
    }
}