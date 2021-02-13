import java.awt.*;
import java.awt.geom.Rectangle2D;

public class Novice extends AgentDecorator {
	public Novice(Agent agent) {
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
		return agent.getCash() > 4000;
	}

	@Override
	public Agent updated() {
		return new Master(this);
	}

	@Override
	public void draw(Graphics2D g2d) {
		agent.draw(g2d);
		g2d.setColor(Color.WHITE);
		g2d.fill(new Rectangle2D.Double(position.getX(), position.getY() - 40, 15, 15));
	}

	@Override
	public void step() {
		agent.step();
	}
}