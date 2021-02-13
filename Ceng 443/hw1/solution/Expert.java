import java.awt.*;
import java.awt.geom.Rectangle2D;

public class Expert extends AgentDecorator {
	public Expert(Agent agent) {
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
		return false;
	}

	@Override
	public Agent updated() {
		return null;
	}

	@Override
	public void draw(Graphics2D g2d) {
		agent.draw(g2d);
		g2d.setColor(Color.RED);
		g2d.fill(new Rectangle2D.Double(position.getX() + 40, position.getY() - 40, 15, 15));
	}

	@Override
	public void step() {
		agent.step();
	}
}