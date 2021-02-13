import java.awt.*;
import java.awt.geom.Rectangle2D;

public class Master extends AgentDecorator {
	public Master(Agent agent) {
		super(agent);
	}

	@Override
	public double getCash() {
		return agent.getCash();
	}

	@Override
	public void setState(State state) {
		agent.setState(state);
	}

	@Override
	public boolean notSameCountry(String str) {
		return agent.notSameCountry(str);
	}

	@Override
	public boolean willUpdate() {
		return agent.getCash() > 6000;
	}

	@Override
	public Agent updated() {
		return new Expert(this);
	}

	@Override
	public void draw(Graphics2D g2d) {
		agent.draw(g2d);
		g2d.setColor(Color.YELLOW);
		g2d.fill(new Rectangle2D.Double(position.getX() + 20, position.getY() - 40, 15, 15));
	}

	@Override
	public void step() {
		agent.step();
	}
}