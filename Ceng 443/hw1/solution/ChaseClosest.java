import java.awt.*;

public class ChaseClosest extends State {
	private final double speed;

	public ChaseClosest(Agent agent) {
		super(agent);
		speed = 1.3 * Common.getRandomGenerator().nextDouble() + 0.7;
	}

	@Override
	public void execute() {
		double minDistance = 5000;
		Position destination = null;
		for (Order order: Common.orders) {
			double distance = calculateDistance(order.position);
			if (distance < minDistance && belongingAgent.notSameCountry(order.belongingCountry.getName())) {
				minDistance = distance;
				destination = order.position;
			}
		}
		if(destination == null)
			return;
		destination = new Position(destination.getX() - 35, destination.getY() - 35);
		move(speed, destination);
	}

	@Override
	public void drawName(Graphics2D g2d, int x, int y) { g2d.drawString("ChaseClosest", x, y); }
}