import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class Country extends Entity {
    private final String name;
	private int gold = 50;
	private double cash = 10000;
	private final Font font = new Font("Verdana", Font.BOLD,17);
	private BufferedImage img;

	public Country(String name, double x, double y) {
		super(x, y);
		this.name = name;
		try {
			img = ImageIO.read(new File("images/" + name.toLowerCase() + ".jpg"));
		} catch (IOException e) {
			System.out.println("error");
		}
	}

	// getter
	public String getName() { return name; }

	// buy gold and pay cash
	public void buyGold(int amount) {
		double currentPrice = Common.getGoldPrice().getCurrentPrice();
		if (cash >= currentPrice * amount) {
			gold += amount;
			cash -= currentPrice * amount;
		}
	}

	// sell gold and get money
	public void sellGold(int amount) {
		double currentPrice = Common.getGoldPrice().getCurrentPrice();
		if (gold >= amount) {
			gold -= amount;
			cash += currentPrice * amount;
		}
	}

	// your order is stolen and you must lose gold if enough
	public boolean loseGold(int amount) {
		if (gold >= amount) {
			gold -= amount;
			return true;
		}
		return false;
	}

	// your order is stolen and you must lose cash if enough
	public boolean loseCash(double amount) {
		if (cash >= amount) {
			cash -= amount;
			return true;
		}
		return false;
	}

	// get some gold
	public void gainGold(int amount) { gold += amount; }

	// get some cash
	public void gainCash(double amount) { cash += amount; }

	// returns the nickname of the country
	public String getNickName() {
		switch (name) {
			case "USA":
				return "US";
			case "Israel":
				return "IL";
			case "Turkey":
				return "TR";
			case "Russia":
				return "RU";
			case "China":
				return "CN";
			default:
				return "";
		}
	}

	@Override
	public void draw(Graphics2D g2d) {
		g2d.drawImage(img, position.getIntX(), position.getIntY(), 150, 100, null);

		g2d.setFont(font);

		g2d.setColor(Color.BLACK);
		g2d.drawString(name, position.getIntX() + 40, position.getIntY()+125);

		g2d.setColor(Color.YELLOW);
		g2d.drawString(gold + " gold", position.getIntX() + 15, position.getIntY()+150);

		g2d.setColor(Color.GREEN);
		g2d.drawString((int)cash + " cash", position.getIntX() + 15, position.getIntY()+175);

		g2d.setColor(Color.BLUE);
		g2d.drawString("Worth: " + (int)(cash+gold*Common.getGoldPrice().getCurrentPrice()),
				position.getIntX() + 15,position.getIntY() + 200);
	}

	@Override
	public void step() { Common.orderFactory[Common.getRandomGenerator().nextInt(2)].createOrder(this); }
}