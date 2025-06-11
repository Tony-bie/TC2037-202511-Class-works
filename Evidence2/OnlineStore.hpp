//
//  OnlineStore.hpp
//  ECommerce
//
//  Created by Developer on 10/06/25.
//

#ifndef OnlineStore_hpp
#define OnlineStore_hpp

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <map>

enum class ProductCategory {
    ELECTRONICS,
    CLOTHING,
    BOOKS,
    HOME,
    SPORTS,
    TOYS
};

enum class OrderStatus {
    PENDING,
    PROCESSING,
    SHIPPED,
    DELIVERED,
    CANCELLED
};

enum class PaymentMethod {
    CREDIT_CARD,
    DEBIT_CARD,
    PAYPAL,
    CASH_ON_DELIVERY
};

struct Date {
    int day, month, year;
    
    Date() : day(1), month(1), year(2025) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    bool isAfter(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
};

class Product {
private:
    static int nextProductId;
    int productId;
    std::string name;
    std::string description;
    double price;
    int stockQuantity;
    ProductCategory category;
    std::string brand;
    double rating;
    int reviewCount;
    bool isActive;
    double weight;
    std::string dimensions;
    
public:
    Product(const std::string& productName, const std::string& desc, double productPrice,
            int stock, ProductCategory cat, const std::string& productBrand = "")
        : productId(nextProductId++), name(productName), description(desc), 
          price(productPrice), stockQuantity(stock), category(cat), brand(productBrand),
          rating(0.0), reviewCount(0), isActive(true), weight(0.0) {}
    
    ~Product() {}
    
    bool isInStock() const {
        return stockQuantity > 0 && isActive;
    }
    
    bool hasStock(int quantity) const {
        return stockQuantity >= quantity && isActive;
    }
    
    bool reduceStock(int quantity) {
        if (hasStock(quantity)) {
            stockQuantity -= quantity;
            return true;
        }
        return false;
    }
    
    void addStock(int quantity) {
        if (quantity > 0) {
            stockQuantity += quantity;
        }
    }
    
    void addReview(double newRating) {
        if (newRating >= 1.0 && newRating <= 5.0) {
            rating = ((rating * reviewCount) + newRating) / (reviewCount + 1);
            reviewCount++;
        }
    }
    
    bool isLowStock() const {
        return stockQuantity <= 10 && stockQuantity > 0;
    }
    
    double getDiscountedPrice(double discountPercent) const {
        return price * (1.0 - discountPercent / 100.0);
    }
    
    // Getters
    int getId() const { return productId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }
    ProductCategory getCategory() const { return category; }
    std::string getBrand() const { return brand; }
    double getRating() const { return rating; }
    int getReviewCount() const { return reviewCount; }
    bool getIsActive() const { return isActive; }
    double getWeight() const { return weight; }
    std::string getDimensions() const { return dimensions; }
    
    // Setters
    void setName(const std::string& productName) { name = productName; }
    void setDescription(const std::string& desc) { description = desc; }
    void setPrice(double productPrice) { price = productPrice; }
    void setCategory(ProductCategory cat) { category = cat; }
    void setBrand(const std::string& productBrand) { brand = productBrand; }
    void setIsActive(bool active) { isActive = active; }
    void setWeight(double productWeight) { weight = productWeight; }
    void setDimensions(const std::string& dim) { dimensions = dim; }
    
    std::string getCategoryString() const {
        switch (category) {
            case ProductCategory::ELECTRONICS: return "Electronics";
            case ProductCategory::CLOTHING: return "Clothing";
            case ProductCategory::BOOKS: return "Books";
            case ProductCategory::HOME: return "Home";
            case ProductCategory::SPORTS: return "Sports";
            case ProductCategory::TOYS: return "Toys";
            default: return "Unknown";
        }
    }
    
    void printProduct() const {
        std::cout << "Product [" << productId << "] " << name << std::endl;
        std::cout << "  Description: " << description << std::endl;
        std::cout << "  Price: $" << std::fixed << std::setprecision(2) << price << std::endl;
        std::cout << "  Brand: " << brand << " | Category: " << getCategoryString() << std::endl;
        std::cout << "  Stock: " << stockQuantity;
        if (isLowStock()) {
            std::cout << " ⚠️ LOW STOCK";
        }
        std::cout << std::endl;
        std::cout << "  Rating: " << std::fixed << std::setprecision(1) << rating 
                  << "/5.0 (" << reviewCount << " reviews)" << std::endl;
        std::cout << "  Status: " << (isActive ? "Active" : "Inactive") << std::endl;
        if (weight > 0) {
            std::cout << "  Weight: " << weight << " kg" << std::endl;
        }
        if (!dimensions.empty()) {
            std::cout << "  Dimensions: " << dimensions << std::endl;
        }
    }
    
    void printCompact() const {
        std::cout << std::setw(4) << productId << " | " 
                  << std::setw(25) << name.substr(0, 25) << " | "
                  << "$" << std::setw(8) << std::fixed << std::setprecision(2) << price << " | "
                  << std::setw(5) << stockQuantity << " | "
                  << std::setw(12) << getCategoryString() << std::endl;
    }
};

int Product::nextProductId = 1;

class CartItem {
private:
    Product* product;
    int quantity;
    double unitPrice;
    
public:
    CartItem(Product* prod, int qty) : product(prod), quantity(qty) {
        unitPrice = prod ? prod->getPrice() : 0.0;
    }
    
    ~CartItem() {}
    
    double getTotalPrice() const {
        return unitPrice * quantity;
    }
    
    bool updateQuantity(int newQuantity) {
        if (newQuantity > 0 && product && product->hasStock(newQuantity)) {
            quantity = newQuantity;
            return true;
        }
        return false;
    }
    
    // Getters
    Product* getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    
    void printCartItem() const {
        if (product) {
            std::cout << "  " << product->getName() << " x" << quantity 
                      << " @ $" << std::fixed << std::setprecision(2) << unitPrice
                      << " = $" << std::fixed << std::setprecision(2) << getTotalPrice() << std::endl;
        }
    }
};

class ShoppingCart {
private:
    std::vector<CartItem> items;
    int customerId;
    
public:
    ShoppingCart(int custId) : customerId(custId) {}
    ~ShoppingCart() {}
    
    bool addItem(Product* product, int quantity = 1) {
        if (!product || quantity <= 0 || !product->hasStock(quantity)) {
            return false;
        }
        
        // Check if item already exists
        for (auto& item : items) {
            if (item.getProduct()->getId() == product->getId()) {
                return item.updateQuantity(item.getQuantity() + quantity);
            }
        }
        
        // Add new item
        items.emplace_back(product, quantity);
        return true;
    }
    
    bool removeItem(int productId) {
        auto it = std::find_if(items.begin(), items.end(),
            [productId](const CartItem& item) {
                return item.getProduct()->getId() == productId;
            });
        
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }
    
    bool updateItemQuantity(int productId, int newQuantity) {
        if (newQuantity <= 0) {
            return removeItem(productId);
        }
        
        for (auto& item : items) {
            if (item.getProduct()->getId() == productId) {
                return item.updateQuantity(newQuantity);
            }
        }
        return false;
    }
    
    void clearCart() {
        items.clear();
    }
    
    double getTotalAmount() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }
    
    int getTotalItems() const {
        int total = 0;
        for (const auto& item : items) {
            total += item.getQuantity();
        }
        return total;
    }
    
    bool isEmpty() const {
        return items.empty();
    }
    
    bool validateStock() const {
        for (const auto& item : items) {
            if (!item.getProduct()->hasStock(item.getQuantity())) {
                return false;
            }
        }
        return true;
    }
    
    // Getters
    std::vector<CartItem> getItems() const { return items; }
    int getCustomerId() const { return customerId; }
    
    void printCart() const {
        std::cout << "Shopping Cart (Customer " << customerId << "):" << std::endl;
        if (isEmpty()) {
            std::cout << "  Cart is empty." << std::endl;
            return;
        }
        
        for (const auto& item : items) {
            item.printCartItem();
        }
        
        std::cout << "  ────────────────────────────────" << std::endl;
        std::cout << "  Total Items: " << getTotalItems() << std::endl;
        std::cout << "  Total Amount: $" << std::fixed << std::setprecision(2) << getTotalAmount() << std::endl;
    }
};

class Order {
private:
    static int nextOrderId;
    int orderId;
    int customerId;
    std::vector<CartItem> orderItems;
    double totalAmount;
    double shippingCost;
    double tax;
    OrderStatus status;
    PaymentMethod paymentMethod;
    Date orderDate;
    Date shippingDate;
    Date deliveryDate;
    std::string shippingAddress;
    std::string trackingNumber;
    
public:
    Order(int custId, const std::vector<CartItem>& items, PaymentMethod payment)
        : orderId(nextOrderId++), customerId(custId), orderItems(items),
          paymentMethod(payment), status(OrderStatus::PENDING) {
        
        orderDate = Date();
        shippingCost = calculateShippingCost();
        tax = calculateTax();
        totalAmount = calculateTotal();
        trackingNumber = "TRK" + std::to_string(orderId) + "2025";
    }
    
    ~Order() {}
    
    double calculateSubtotal() const {
        double subtotal = 0.0;
        for (const auto& item : orderItems) {
            subtotal += item.getTotalPrice();
        }
        return subtotal;
    }
    
    double calculateShippingCost() const {
        double subtotal = calculateSubtotal();
        if (subtotal >= 50.0) {
            return 0.0; // Free shipping over $50
        }
        return 9.99;
    }
    
    double calculateTax() const {
        return calculateSubtotal() * 0.08; // 8% tax
    }
    
    double calculateTotal() const {
        return calculateSubtotal() + shippingCost + tax;
    }
    
    bool processOrder() {
        if (status != OrderStatus::PENDING) {
            return false;
        }
        
        // Reduce stock for all items
        for (const auto& item : orderItems) {
            if (!item.getProduct()->reduceStock(item.getQuantity())) {
                return false;
            }
        }
        
        status = OrderStatus::PROCESSING;
        return true;
    }
    
    void shipOrder() {
        if (status == OrderStatus::PROCESSING) {
            status = OrderStatus::SHIPPED;
            shippingDate = Date();
        }
    }
    
    void deliverOrder() {
        if (status == OrderStatus::SHIPPED) {
            status = OrderStatus::DELIVERED;
            deliveryDate = Date();
        }
    }
    
    void cancelOrder() {
        if (status == OrderStatus::PENDING || status == OrderStatus::PROCESSING) {
            // Return stock
            for (const auto& item : orderItems) {
                item.getProduct()->addStock(item.getQuantity());
            }
            status = OrderStatus::CANCELLED;
        }
    }
    
    // Getters
    int getId() const { return orderId; }
    int getCustomerId() const { return customerId; }
    std::vector<CartItem> getOrderItems() const { return orderItems; }
    double getTotalAmount() const { return totalAmount; }
    double getShippingCost() const { return shippingCost; }
    double getTax() const { return tax; }
    OrderStatus getStatus() const { return status; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    Date getOrderDate() const { return orderDate; }
    Date getShippingDate() const { return shippingDate; }
    Date getDeliveryDate() const { return deliveryDate; }
    std::string getShippingAddress() const { return shippingAddress; }
    std::string getTrackingNumber() const { return trackingNumber; }
    
    // Setters
    void setShippingAddress(const std::string& address) { shippingAddress = address; }
    
    std::string getStatusString() const {
        switch (status) {
            case OrderStatus::PENDING: return "Pending";
            case OrderStatus::PROCESSING: return "Processing";
            case OrderStatus::SHIPPED: return "Shipped";
            case OrderStatus::DELIVERED: return "Delivered";
            case OrderStatus::CANCELLED: return "Cancelled";
            default: return "Unknown";
        }
    }
    
    std::string getPaymentMethodString() const {
        switch (paymentMethod) {
            case PaymentMethod::CREDIT_CARD: return "Credit Card";
            case PaymentMethod::DEBIT_CARD: return "Debit Card";
            case PaymentMethod::PAYPAL: return "PayPal";
            case PaymentMethod::CASH_ON_DELIVERY: return "Cash on Delivery";
            default: return "Unknown";
        }
    }
    
    void printOrder() const {
        std::cout << "Order [" << orderId << "] - Customer " << customerId << std::endl;
        std::cout << "  Status: " << getStatusString() << std::endl;
        std::cout << "  Payment: " << getPaymentMethodString() << std::endl;
        std::cout << "  Order Date: " << orderDate.toString() << std::endl;
        
        if (status == OrderStatus::SHIPPED || status == OrderStatus::DELIVERED) {
            std::cout << "  Shipping Date: " << shippingDate.toString() << std::endl;
            std::cout << "  Tracking: " << trackingNumber << std::endl;
        }
        
        if (status == OrderStatus::DELIVERED) {
            std::cout << "  Delivery Date: " << deliveryDate.toString() << std::endl;
        }
        
        if (!shippingAddress.empty()) {
            std::cout << "  Shipping Address: " << shippingAddress << std::endl;
        }
        
        std::cout << "  Items:" << std::endl;
        for (const auto& item : orderItems) {
            item.printCartItem();
        }
        
        std::cout << "  ────────────────────────────────" << std::endl;
        std::cout << "  Subtotal: $" << std::fixed << std::setprecision(2) << calculateSubtotal() << std::endl;
        std::cout << "  Shipping: $" << std::fixed << std::setprecision(2) << shippingCost << std::endl;
        std::cout << "  Tax: $" << std::fixed << std::setprecision(2) << tax << std::endl;
        std::cout << "  Total: $" << std::fixed << std::setprecision(2) << totalAmount << std::endl;
    }
};

int Order::nextOrderId = 1;

class Customer {
private:
    static int nextCustomerId;
    int customerId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    Date joinDate;
    bool isActive;
    double totalSpent;
    int totalOrders;
    std::vector<int> orderHistory;
    
public:
    Customer(const std::string& first, const std::string& last, 
             const std::string& emailAddr, const std::string& phoneNum)
        : customerId(nextCustomerId++), firstName(first), lastName(last),
          email(emailAddr), phone(phoneNum), isActive(true), 
          totalSpent(0.0), totalOrders(0) {
        joinDate = Date();
    }
    
    ~Customer() {}
    
    void addOrder(int orderId, double orderAmount) {
        orderHistory.push_back(orderId);
        totalSpent += orderAmount;
        totalOrders++;
    }
    
    double getAverageOrderValue() const {
        return totalOrders > 0 ? totalSpent / totalOrders : 0.0;
    }
    
    bool isVipCustomer() const {
        return totalSpent >= 1000.0 || totalOrders >= 10;
    }
    
    std::string getFullName() const {
        return firstName + " " + lastName;
    }
    
    // Getters
    int getId() const { return customerId; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    Date getJoinDate() const { return joinDate; }
    bool getIsActive() const { return isActive; }
    double getTotalSpent() const { return totalSpent; }
    int getTotalOrders() const { return totalOrders; }
    std::vector<int> getOrderHistory() const { return orderHistory; }
    
    // Setters
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setEmail(const std::string& emailAddr) { email = emailAddr; }
    void setPhone(const std::string& phoneNum) { phone = phoneNum; }
    void setAddress(const std::string& addr) { address = addr; }
    void setIsActive(bool active) { isActive = active; }
    
    void printCustomer() const {
        std::cout << "Customer [" << customerId << "] " << getFullName() << std::endl;
        std::cout << "  Email: " << email << " | Phone: " << phone << std::endl;
        std::cout << "  Address: " << address << std::endl;
        std::cout << "  Joined: " << joinDate.toString() << std::endl;
        std::cout << "  Status: " << (isActive ? "Active" : "Inactive");
        if (isVipCustomer()) {
            std::cout << " 🌟 VIP";
        }
        std::cout << std::endl;
        std::cout << "  Total Orders: " << totalOrders << std::endl;
        std::cout << "  Total Spent: $" << std::fixed << std::setprecision(2) << totalSpent << std::endl;
        std::cout << "  Average Order: $" << std::fixed << std::setprecision(2) << getAverageOrderValue() << std::endl;
    }
};

int Customer::nextCustomerId = 1;

class OnlineStore {
private:
    std::vector<std::unique_ptr<Product>> products;
    std::vector<std::unique_ptr<Customer>> customers;
    std::vector<std::unique_ptr<Order>> orders;
    std::map<int, ShoppingCart> carts;
    std::string storeName;
    double totalRevenue;
    int totalOrdersProcessed;
    
public:
    OnlineStore(const std::string& name) 
        : storeName(name), totalRevenue(0.0), totalOrdersProcessed(0) {}
    
    ~OnlineStore() {}
    
    void addProduct(std::unique_ptr<Product> product) {
        if (product) {
            products.push_back(std::move(product));
        }
    }
    
    void addCustomer(std::unique_ptr<Customer> customer) {
        if (customer) {
            int custId = customer->getId();
            customers.push_back(std::move(customer));
            carts[custId] = ShoppingCart(custId);
        }
    }
    
    Product* findProduct(int productId) {
        auto it = std::find_if(products.begin(), products.end(),
            [productId](const std::unique_ptr<Product>& product) {
                return product->getId() == productId;
            });
        return (it != products.end()) ? it->get() : nullptr;
    }
    
    Customer* findCustomer(int customerId) {
        auto it = std::find_if(customers.begin(), customers.end(),
            [customerId](const std::unique_ptr<Customer>& customer) {
                return customer->getId() == customerId;
            });
        return (it != customers.end()) ? it->get() : nullptr;
    }
    
    bool addToCart(int customerId, int productId, int quantity = 1) {
        Product* product = findProduct(productId);
        if (product && carts.find(customerId) != carts.end()) {
            return carts[customerId].addItem(product, quantity);
        }
        return false;
    }
    
    bool removeFromCart(int customerId, int productId) {
        if (carts.find(customerId) != carts.end()) {
            return carts[customerId].removeItem(productId);
        }
        return false;
    }
    
    ShoppingCart* getCart(int customerId) {
        if (carts.find(customerId) != carts.end()) {
            return &carts[customerId];
        }
        return nullptr;
    }
    
    int createOrder(int customerId, PaymentMethod payment) {
        ShoppingCart* cart = getCart(customerId);
        Customer* customer = findCustomer(customerId);
        
        if (!cart || !customer || cart->isEmpty() || !cart->validateStock()) {
            return -1;
        }
        
        auto order = std::make_unique<Order>(customerId, cart->getItems(), payment);
        int orderId = order->getId();
        
        if (order->processOrder()) {
            double orderAmount = order->getTotalAmount();
            customer->addOrder(orderId, orderAmount);
            totalRevenue += orderAmount;
            totalOrdersProcessed++;
            
            orders.push_back(std::move(order));
            cart->clearCart();
            
            return orderId;
        }
        
        return -1;
    }
    
    Order* findOrder(int orderId) {
        auto it = std::find_if(orders.begin(), orders.end(),
            [orderId](const std::unique_ptr<Order>& order) {
                return order->getId() == orderId;
            });
        return (it != orders.end()) ? it->get() : nullptr;
    }
    
    std::vector<Product*> searchProducts(const std::string& query) {
        std::vector<Product*> results;
        for (const auto& product : products) {
            if (product->getName().find(query) != std::string::npos ||
                product->getDescription().find(query) != std::string::npos ||
                product->getBrand().find(query) != std::string::npos) {
                results.push_back(product.get());
            }
        }
        return results;
    }
    
    std::vector<Product*> getProductsByCategory(ProductCategory category) {
        std::vector<Product*> results;
        for (const auto& product : products) {
            if (product->getCategory() == category && product->getIsActive()) {
                results.push_back(product.get());
            }
        }
        return results;
    }
    
    void printStoreSummary() {
        std::cout << "=== " << storeName << " Summary ===" << std::endl;
        std::cout << "Total Products: " << products.size() << std::endl;
        std::cout << "Total Customers: " << customers.size() << std::endl;
        std::cout << "Total Orders: " << totalOrdersProcessed << std::endl;
        std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
        
        if (totalOrdersProcessed > 0) {
            std::cout << "Average Order Value: $" << std::fixed << std::setprecision(2) 
                      << (totalRevenue / totalOrdersProcessed) << std::endl;
        }
    }
    
    void printAllProducts() {
        std::cout << "=== All Products ===" << std::endl;
        std::cout << std::setw(4) << "ID" << " | " 
                  << std::setw(25) << "Name" << " | "
                  << std::setw(10) << "Price" << " | "
                  << std::setw(5) << "Stock" << " | "
                  << std::setw(12) << "Category" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        for (const auto& product : products) {
            product->printCompact();
        }
    }
};

int main() {
    std::cout << "=== Online Store System ===" << std::endl;
    
    OnlineStore store("TechMart Online");
    
    // Add products
    store.addProduct(std::make_unique<Product>("iPhone 15", "Latest smartphone", 999.99, 50, ProductCategory::ELECTRONICS, "Apple"));
    store.addProduct(std::make_unique<Product>("Nike Air Max", "Running shoes", 129.99, 30, ProductCategory::CLOTHING, "Nike"));
    store.addProduct(std::make_unique<Product>("Programming Book", "Learn C++ in 21 days", 49.99, 100, ProductCategory::BOOKS, "TechPress"));
    store.addProduct(std::make_unique<Product>("Coffee Maker", "Automatic coffee machine", 199.99, 25, ProductCategory::HOME, "BrewMaster"));
    store.addProduct(std::make_unique<Product>("Basketball", "Official size basketball", 29.99, 75, ProductCategory::SPORTS, "SportsPro"));
    
    // Add customers
    store.addCustomer(std::make_unique<Customer>("John", "Doe", "john@email.com", "555-1234"));
    store.addCustomer(std::make_unique<Customer>("Jane", "Smith", "jane@email.com", "555-5678"));
    store.addCustomer(std::make_unique<Customer>("Bob", "Johnson", "bob@email.com", "555-9012"));
    
    // Shopping simulation
    std::cout << "\n=== Shopping Simulation ===" << std::endl;
    
    // John's shopping
    store.addToCart(1, 1, 1); // iPhone
    store.addToCart(1, 3, 2); // Programming books
    store.addToCart(1, 4, 1); // Coffee maker
    
    ShoppingCart* johnCart = store.getCart(1);
    if (johnCart) {
        std::cout << "\nJohn's cart:" << std::endl;
        johnCart->printCart();
    }
    
    // Create John's order
    int johnOrderId = store.createOrder(1, PaymentMethod::CREDIT_CARD);
    if (johnOrderId > 0) {
        std::cout << "\nJohn's order created successfully! Order ID: " << johnOrderId << std::endl;
        Order* johnOrder = store.findOrder(johnOrderId);
        if (johnOrder) {
            johnOrder->setShippingAddress("123 Main St, City, State 12345");
            johnOrder->printOrder();
        }
    }
    
    // Jane's shopping
    store.addToCart(2, 2, 2); // Nike shoes
    store.addToCart(2, 5, 1); // Basketball
    
    ShoppingCart* janeCart = store.getCart(2);
    if (janeCart) {
        std::cout << "\nJane's cart:" << std::endl;
        janeCart->printCart();
    }
    
    int janeOrderId = store.createOrder(2, PaymentMethod::PAYPAL);
    if (janeOrderId > 0) {
        std::cout << "\nJane's order created! Order ID: " << janeOrderId << std::endl;
    }
    
    // Product search
    std::cout << "\n=== Product Search ===" << std::endl;
    auto searchResults = store.searchProducts("Apple");
    std::cout << "Search results for 'Apple':" << std::endl;
    for (Product* product : searchResults) {
        product->printCompact();
    }
    
    // Category browsing
    std::cout << "\n=== Electronics Category ===" << std::endl;
    auto electronics = store.getProductsByCategory(ProductCategory::ELECTRONICS);
    for (Product* product : electronics) {
        product->printCompact();
    }
    
    // Final store summary
    std::cout << "\n=== Final Store Summary ===" << std::endl;
    store.printStoreSummary();
    
    return 0;
}

#endif /* OnlineStore_hpp */