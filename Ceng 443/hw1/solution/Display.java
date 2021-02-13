import javax.swing.*;
import java.awt.*;

public class Display extends JPanel {
    public Display() { this.setBackground(new Color(180, 180, 180)); }

    @Override
    public Dimension getPreferredSize() { return super.getPreferredSize(); }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Common.getGoldPrice().draw((Graphics2D) g);
        g.drawLine(0, Common.getUpperLineY(), Common.getWindowWidth(), Common.getUpperLineY());
        for (Country country: Common.countries)
            country.draw((Graphics2D) g);
        for (Order order: Common.orders)
            order.draw((Graphics2D) g);
        for (Agent agent: Common.agents)
            agent.draw((Graphics2D) g);
    }
}