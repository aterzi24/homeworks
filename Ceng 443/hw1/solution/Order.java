import java.awt.*;

public abstract class Order extends Entity {
    protected final int amount;
    protected final double speed;
    // destination point where the order is headed to
    protected final int destination;
    // nickname appearing above the order
    protected final String nickname;
    // country which this order belongs to
    protected Country belongingCountry;
    protected Shape circle;
    protected Font font = new Font("Verdana", Font.BOLD, 11);

    public Order(Country country) {
    	super(country.getPosition().getX() + 75, country.getPosition().getY());
    	amount = Common.getRandomGenerator().nextInt(5) + 1;
    	destination = Common.getRandomGenerator().nextInt(Common.getWindowWidth()) + 1;
    	belongingCountry = country;
    	speed = Common.getRandomGenerator().nextDouble() + 0.75;
    	nickname = country.getNickName();
    }

    // move the order to the destination with speed
    protected void move() {
	    double xDisplacement = (destination - position.getX()) / (position.getY() / speed);
	    position.setX(position.getX() + xDisplacement);
	    position.setY(position.getY() - speed);
    }

    // returns true if order hits the upper line
    public boolean didReachUpperLine() {
    	return (position.getIntY() <= Common.getUpperLineY());
    }

    // Agent of the country given in the parameter intercepts this order and stole the cash or gold depending on type
    public abstract void stolen(Country country);
}