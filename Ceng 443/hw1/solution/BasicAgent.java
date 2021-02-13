import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Iterator;

public class BasicAgent extends Agent {
	private final String name;
	private double cash = 0;
	private BufferedImage img;
	private final Font font = new Font("Verdana", Font.BOLD,17);
	private State state;

    public BasicAgent(String name, double x, double y) {
    	super(x, y);
	    this.name = name;
	    try {
		    img = ImageIO.read(new File("images/" + name.toLowerCase() + ".png"));
	    } catch (IOException e) {
		    System.out.println("error");
	    }
	    State.randomState(this);
    }

	@Override
	public double getCash() {
		return cash;
	}

	@Override
    public void setState(State state) {
    	this.state = state;
    }

    @Override
	public boolean notSameCountry(String str) {
		return Common.countryNames.indexOf(str) != Common.agentNames.indexOf(name);
	}

	@Override
	public boolean willUpdate() {
		return cash > 2000;
	}

	@Override
	public Agent updated() {
		return new Novice(this);
	}

	@Override
	public void draw(Graphics2D g2d) {
		g2d.drawImage(img, position.getIntX(), position.getIntY(), 75, 75, null);

		g2d.setFont(font);

		g2d.setColor(Color.BLACK);
		g2d.drawString(name, position.getIntX() + 5, position.getIntY() - 5);

		g2d.setColor(Color.BLUE);
		state.drawName(g2d, position.getIntX() + 10, position.getIntY() + 90);

		g2d.setColor(Color.RED);
		g2d.drawString(Integer.toString((int)cash), position.getIntX() + 10, position.getIntY()+105);
	}

	@Override
	public void step() {
		state.execute();
		Iterator<Order> i = Common.orders.iterator();
		while (i.hasNext()) {
			Order order = i.next();
			double distance = order.position.distanceTo(position.getX()+35, position.getY()+35);
			if (distance < 38 && notSameCountry(order.belongingCountry.getName())) {
				cash += order.amount * Common.getGoldPrice().getCurrentPrice();
				order.stolen(Common.countries.get(Common.agentNames.indexOf(name)));
				i.remove();
			}
		}
	}
}