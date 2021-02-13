package project.parts.logics;

import project.SimulationRunner;
import project.components.Factory;
import project.components.Robot;
import project.parts.Arm;
import project.parts.Base;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.List;

public class Fixer extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n", ...);
        // System.out.printf("Robot %02d : Nothing to fix, waiting!%n", ...);
        // System.out.printf("Robot %02d : Fixer woke up, going back to work.%n", ...);
        int no = (int) Common.get(robot, "serialNo");
        synchronized (SimulationRunner.factory.brokenRobots) {
            List<Robot> temp = SimulationRunner.factory.brokenRobots;
            // wait until there is at least one robot to be fixed
            while (temp.size() == 0) {
                System.out.printf("Robot %02d : Nothing to fix, waiting!%n", no);
                try {
                    if (SimulationRunner.factory.stopProduction) { return; }
                    temp.wait();
                    System.out.printf("Robot %02d : Fixer woke up, going back to work.%n", no);
                    if ( SimulationRunner.factory.stopProduction)  { return ; }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            Base base = (Base) temp.get(0);
            Arm arm = (Arm) Common.get(base, "arm");
            Payload payload = (Payload) Common.get(base, "payload");
            Logic logic = (Logic) Common.get(base, "logic");
            int no2 = (int) Common.get(base, "serialNo");
            // determine which part is missing and build another one
            if (arm == null) {
                Common.set(base, "arm", Factory.createPart("Arm"));
            }
            else if (payload == null) {
                int index = Common.logicNames.indexOf(logic.getClass().getSimpleName());
                String str = Common.payloadNames.get(index);
                Common.set(base, "payload", Factory.createPart(str));
            }
            else if (logic == null) {
                int index = Common.payloadNames.indexOf(payload.getClass().getSimpleName());
                String str = Common.logicNames.get(index);
                Common.set(base, "logic", Factory.createPart(str));
            }
            temp.remove(0);
            // wake up fixed robot
            synchronized (base) { base.notify(); }
            System.out.printf("Robot %02d : Fixed and waken up robot (%02d).%n", no, no2);
        }
    }
}