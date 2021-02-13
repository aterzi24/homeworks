package project.parts.logics;

import project.SimulationRunner;
import project.components.ProductionLine;
import project.components.Robot;
import project.parts.Part;
import project.utility.Common;

public class Supplier extends Logic
{
    @Override public void run ( Robot robot ) {
        // TODO
        // Following messages are appropriate for this class
        // System.out.printf( "Robot %02d : Supplying a random part on production line.%n", ...);
        // System.out.printf( "Robot %02d : Production line is full, removing a random part from production line.%n", ...);
        // System.out.printf( "Robot %02d : Waking up waiting builders.%n", ...);
        int no = (int) Common.get(robot, "serialNo");
        synchronized (SimulationRunner.factory.productionLine.parts) {
            ProductionLine temp = SimulationRunner.factory.productionLine;
            // if productionLine's size reached maxCapacity then remove random part from it
            if (temp.parts.size() == temp.maxCapacity) {
                temp.parts.remove(Common.random.nextInt(temp.maxCapacity));
                System.out.printf("Robot %02d : Production line is full, removing a random part from production line.%n", no);
            }
            // else create random part and add to productionLine
            else {
                Part part = Common.createRandomPart();
                temp.parts.add(part);
                System.out.printf("Robot %02d : Supplying a random part on production line.%n", no);
            }
            SimulationRunner.productionLineDisplay.repaint();
            // wake up all builders waiting on productionLine.parts
            temp.parts.notifyAll();
            System.out.printf("Robot %02d : Waking up waiting builders.%n", no);
        }
    }
}