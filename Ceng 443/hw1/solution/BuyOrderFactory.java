public class BuyOrderFactory extends OrderFactory {
    public void createOrder(Country country) {
    	Common.orders.add(new BuyOrder(country));
    }
}