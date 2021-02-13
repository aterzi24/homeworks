import java.awt.*;

public abstract class State {
	// the agent which this state belongs to
	protected Agent belongingAgent;

	public State(Agent agent) { belongingAgent = agent; }

	// assigns a random state to the agent given in the parameter
	public static void randomState(Agent agent) {
		int choice = Common.getRandomGenerator().nextInt(4);
		State state;
		switch (choice) {
			case 0:
				state = new Shake(agent);
				break;
			case 1:
				state = new GotoXY(agent);
				break;
			case 2:
				state = new ChaseClosest(agent);
				break;
			default:
				state = new Rest(agent);
		}
		agent.setState(state);
	}

	// calculates the distance between belonging agent and the destination
	protected double calculateDistance(Position destination) {
		return destination.distanceTo(belongingAgent.position.getX(), belongingAgent.position.getY());
	}

	// move the agent towards the destination with given speed
	protected void move(double speed, Position destination) {
		if(destination == null)
			return;
		double distance = calculateDistance(destination);
		double rate = distance / speed;
		double xDisplacement = (destination.getX() - belongingAgent.position.getX()) / rate;
		double yDisplacement = (destination.getY() - belongingAgent.position.getY()) / rate;
		belongingAgent.position.setX(belongingAgent.position.getX() + xDisplacement);
		belongingAgent.position.setY(belongingAgent.position.getY() + yDisplacement);
	}

	// does what this state must do
    public abstract void execute();
	// draws the name of the state on the given points in the parameter
	public abstract void drawName(Graphics2D g2d, int x, int y);
}