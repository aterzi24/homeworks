import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class Common {
    private static final String title = "Gold Wars";
    private static final int windowWidth = 1650;
    private static final int windowHeight = 1000;

    private static final GoldPrice goldPrice = new GoldPrice(400, 62);

    private static final Random randomGenerator = new Random(1234);
    private static final int upperLineY = 100;

    // the horizontal line where country images should display
    static final int countyLine = windowHeight - 250;
    // the horizontal line where agent images should display
    static final int agentLine = upperLineY + 150;
    static final List<Country> countries = new ArrayList<>();
    static final List<Order> orders = new ArrayList<>();
    static final List<Agent> agents = new ArrayList<>();
    static final List<String> countryNames = Arrays.asList("USA", "Israel", "Turkey", "Russia", "China");
    static final List<String> agentNames = Arrays.asList("CIA", "Mossad", "MIT", "SVR", "MSS");
    static final OrderFactory[] orderFactory = new OrderFactory[2];

    static  {
        int i;
        // adjusting the GUI according to the window's width and height
        final int displacement = (windowWidth - 750) / 6;
        final int fixed = 150 + displacement;
        for (i = 0; i < 5; i++) {
            countries.add(new Country(countryNames.get(i), displacement + fixed * i, countyLine));
        }
        for (i = 0; i < 5; i++) {
            agents.add(new BasicAgent(agentNames.get(i), displacement + fixed * i, agentLine));
        }
        orderFactory[0] = new BuyOrderFactory();
        orderFactory[1] = new SellOrderFactory();
    }

    // getters
    public static String getTitle() { return title; }
    public static int getWindowWidth() { return windowWidth; }
    public static int getWindowHeight() { return windowHeight; }

    // getter
    public static GoldPrice getGoldPrice() { return goldPrice; }

    // getters
    public static Random getRandomGenerator() { return randomGenerator; }
    public static int getUpperLineY() { return upperLineY; }

    public static void stepAllEntities() {
        if (randomGenerator.nextInt(200) == 0) goldPrice.step();
        for (Country country: countries)
            if (randomGenerator.nextInt(250) == 0)
                country.step();
        for (Order order: orders)
            order.step();
        orders.removeIf(Order::didReachUpperLine);
        for (Agent agent: agents) {
            agent.step();
            if (randomGenerator.nextInt(400) == 0)
                State.randomState(agent);
        }
        for (int i = 0; i < 5; i++) {
            if (agents.get(i).willUpdate())
                agents.set(i, agents.get(i).updated());
        }
    }
}