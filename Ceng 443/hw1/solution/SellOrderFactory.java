public class SellOrderFactory extends OrderFactory {
	public void createOrder(Country country) {
		Common.orders.add(new SellOrder(country));
	}
}