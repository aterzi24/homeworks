public abstract class AgentDecorator extends Agent {
	protected final Agent agent;

    public AgentDecorator(Agent agent) {
    	super(0, 0);
    	position = agent.position;
    	this.agent = agent;
    }
}