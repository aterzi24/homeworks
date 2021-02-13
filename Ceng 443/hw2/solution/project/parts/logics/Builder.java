package project.parts.logics;

import project.SimulationRunner;
import project.components.*;
import project.parts.*;
import project.parts.payloads.Payload;
import project.utility.Common;

import java.util.List;

public class Builder extends Logic
{
    @Override public void run ( Robot robot )
    {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf("Robot %02d : Builder cannot build anything, waiting!%n", ...);
        // System.out.printf("Robot %02d : Builder woke up, going back to work.%n", ...);
        // System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", ...);
        Base base = null;
        Part foundPart = null;
        boolean foundComplete = false;
        boolean foundIncomplete = false;
        int no = (int) Common.get(robot, "serialNo");
        synchronized (SimulationRunner.factory.productionLine.parts) {
            ProductionLine temp = SimulationRunner.factory.productionLine;
            for (Part part : temp.parts) {
                // find a part on productionLine which is a base
                if (part.getClass().getSimpleName().equals("Base")) {
                    base = (Base) part;
                    Arm arm = (Arm) Common.get(base, "arm");
                    Payload payload = (Payload) Common.get(base, "payload");
                    Logic logic = (Logic) Common.get(base, "logic");
                    // if it is complete then break the loop
                    if ((arm != null) && (payload != null) && (logic != null)) {
                        foundComplete = true;
                        break;
                    }
                    // if it is not complete then iterate over productionLine for another part
                    else {
                        for (Part part2 : temp.parts)
                            // if new part is not itself and appropriate then assemble and break
                            if (!part.equals(part2) && Common.assemble(base, arm, payload, part2)) {
                                foundIncomplete = true;
                                foundPart = part2;
                                break;
                            }
                        if (foundIncomplete)
                            break;
                    }

                }
            }
            // if found a complete part
            // this if is the only one which can exit this if-else block
            if (foundComplete) {
                temp.parts.remove(base);
                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", no);
                SimulationRunner.productionLineDisplay.repaint();
            }
            // if assembled a part on an incomplete part
            else if (foundIncomplete) {
                temp.parts.remove(foundPart);
                System.out.printf("Robot %02d : Builder attached some parts or relocated a completed robot.%n", no);
                SimulationRunner.productionLineDisplay.repaint();
                return;
            }
            // builder cannot build and needs to wait for notification
            else {
                try {
                    System.out.printf("Robot %02d : Builder cannot build anything, waiting!%n", no);
                    if (SimulationRunner.factory.stopProduction) { return; }
                    temp.parts.wait();
                    System.out.printf("Robot %02d : Builder woke up, going back to work.%n", no);
                    return;
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
        // newly found complete robot will be placed to factory.robots if empty space is available
        synchronized (SimulationRunner.factory.robots) {
            List<Robot> temp = SimulationRunner.factory.robots;
            if (temp.size() < SimulationRunner.factory.maxRobots) {
                temp.add(base);
                SimulationRunner.robotsDisplay.repaint();
                new Thread(base).start();
                return;
            }
        }
        // newly found complete robot will be placed to storage
        // after placing if size reaches capacity signal the factory to initiateStop
        synchronized (SimulationRunner.factory.storage.robots) {
            Storage temp = SimulationRunner.factory.storage;
            if (temp.robots.size() < temp.maxCapacity) {
                temp.robots.add(base);
                SimulationRunner.storageDisplay.repaint();
                if (temp.robots.size() == temp.maxCapacity)
                    SimulationRunner.factory.initiateStop();
            } else
                SimulationRunner.factory.initiateStop();
        }
    }
}