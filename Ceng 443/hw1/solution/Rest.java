import java.awt.*;

public class Rest extends State {
	public Rest(Agent agent) {
		super(agent);
	}

	@Override
	// do nothing and enjoy the moment
	public void execute() {}

	@Override
	public void drawName(Graphics2D g2d, int x, int y) {
		g2d.drawString("Rest", x, y);
	}
}