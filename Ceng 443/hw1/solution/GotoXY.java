import java.awt.*;

public class GotoXY extends State {
	private Position destination;
	private double speed;

	public GotoXY(Agent agent) {
		super(agent);
		getNewTarget();
	}

	// get new target to go to
	private void getNewTarget() {
		int x = Common.getRandomGenerator().nextInt(Common.getWindowWidth() - 95) + 5;
		int y = Common.getRandomGenerator().nextInt(Common.countyLine - Common.getUpperLineY() - 105)
				+ Common.getUpperLineY() + 5;
		destination = new Position(x, y);
		speed = 1.25 * Common.getRandomGenerator().nextDouble() + 0.5;
		if (calculateDistance(destination) < 2.0) getNewTarget();
	}

	@Override
	public void execute() {
		move(speed, destination);
		if (calculateDistance(destination) < 2.0) getNewTarget();
	}

	@Override
	public void drawName(Graphics2D g2d, int x, int y) {
		g2d.drawString("GotoXY", x, y);
	}
}