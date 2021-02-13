public abstract class Agent extends Entity {
    public Agent(double x, double y) {
    	super(x, y);
    }

    // getter and setter
    public abstract double getCash();
    public abstract void setState(State state);

    // returns true if given str which is the name of a country(e.g. "Turkey")  is not the agents' country
	public abstract boolean notSameCountry(String str);

	// returns true if agent meets the condition to level up
	public abstract boolean willUpdate();

	// return the upper version of the agent
	public abstract Agent updated();
}