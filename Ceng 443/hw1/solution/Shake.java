import java.awt.*;

public class Shake extends State {
	public Shake (Agent agent) {
		super(agent);
	}

	@Override
	public void execute() {
		final int shakeConst = 4;
		int xDisplacement = Common.getRandomGenerator().nextInt(shakeConst);
		int yDisplacement = Common.getRandomGenerator().nextInt(shakeConst);
		xDisplacement *= Common.getRandomGenerator().nextBoolean() ? 1 : -1;
		yDisplacement *= Common.getRandomGenerator().nextBoolean() ? 1 : -1;
		int newX = belongingAgent.position.getIntX() + xDisplacement;
		int newY = belongingAgent.position.getIntY() + yDisplacement;
		if (newX < 5) newX = 5;
		else if (newX > Common.getWindowWidth() - 90) newX = Common.getWindowWidth() - 90;
		if (newY < Common.getUpperLineY() + 5) newY = Common.getUpperLineY() + 5;
		else if (newY > Common.countyLine - 120) newY = Common.countyLine - 120;
		belongingAgent.position.setX(newX);
		belongingAgent.position.setY(newY);
	}

	@Override
	public void drawName(Graphics2D g2d, int x, int y) {
		g2d.drawString("Shake", x, y);
	}
}