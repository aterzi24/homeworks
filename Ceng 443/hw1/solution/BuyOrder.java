import java.awt.*;
import java.awt.geom.Ellipse2D;

public class BuyOrder extends Order {
	public BuyOrder(Country country) {
		super(country);
	}

	@Override
	public void stolen(Country country) {
		if (belongingCountry.loseCash(amount)) {
			country.gainCash(amount);
		}
	}

	@Override
	public void draw(Graphics2D g2d) {
		circle = new Ellipse2D.Double(position.getIntX(), position.getIntY(), 15,15);
		g2d.setFont(font);
		g2d.setColor(Color.GREEN);
		g2d.fill(circle);
		g2d.drawString(nickname, position.getIntX(), position.getIntY());
		g2d.setColor(Color.BLACK);
		g2d.drawString(Integer.toString(amount), position.getIntX() + 4, position.getIntY() + 12);
	}

	@Override
	public void step() {
		move();
		if(didReachUpperLine()) {
			belongingCountry.buyGold(amount);
		}
	}
}