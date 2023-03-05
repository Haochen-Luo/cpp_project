#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

struct Order {
    int id;
    int side;
    int volume;
    int price;
    int time;

    Order(int id, int side, int volume, int price, int time)
        : id(id), side(side), volume(volume), price(price), time(time) {}

     bool operator<(const Order& other) const{
        if (price != other.price) {
            return side == 0 ? price > other.price : price < other.price;
        } else {
            return time > other.time;
        }
    
    }
};

class Exchange {
private:
    std::vector<Order> buy_orders;
    std::vector<Order> sell_orders;
    std::unordered_map<int,std::pair<int,int>> result; // id, total_trading_sum
    
    int matched = 0;
    int order_seq = 0;
    
    // Match buy and sell orders based on their prices
    void matchOrders() {
        // Sort buy and sell orders
        std::sort(buy_orders.begin(), buy_orders.end());
        std::sort(sell_orders.begin(), sell_orders.end());
        
        // Match orders
        int buy_idx = 0, sell_idx = 0;
        while (buy_idx < buy_orders.size() && sell_idx < sell_orders.size()) {
            Order& buy = buy_orders[buy_idx];
            Order& sell = sell_orders[sell_idx];
            int price = sell.price;

            if (buy.price < sell.price) {
                std::cout<<buy.price << sell.price<<std::endl;
                break; // No more matches
            }
            
            int traded_volume = std::min(buy.volume, sell.volume);
            
            
            // Update buy order
            buy.volume -= traded_volume;      
            if(result.count(buy.id)){
                auto[vol, sum] = result[buy.id];
                int vol_ = vol+traded_volume;
                int sum_ = sum+traded_volume*price;
                result[buy.id] = {vol_,sum_};
            }else{
                result[buy.id] = {traded_volume,traded_volume*price};
            }
            
            // Update sell order
            sell.volume -= traded_volume;   
             if(result.count(sell.id)){
                auto[vol, sum] = result[sell.id];
                int vol_ = vol+traded_volume;
                int sum_ = sum+traded_volume*price;
                result[sell.id] = {vol_,sum_};
            }else{
                result[sell.id] = {traded_volume,traded_volume*price};
            }         
            
            // Remove orders with zero volume
            if (buy.volume == 0) {
                buy_idx++;
            }
            if (sell.volume == 0) {
                sell_idx++;
            }
        }
    }
    
public:
    // InputOrder receives order, and return assigned order Id.
    // Parameters:
    // int side: 0 is buy, 1 is sell
    // int volume: order's quantity
    // int price: order's price
    // Return:
    // int: order Id
    int InputOrder(int side, int volume, int price){
        Order order = {++order_seq, side, volume, price, order_seq};
        if (side == 0) {
            buy_orders.push_back(order);
        } else {
            sell_orders.push_back(order);
        }
        std::cout<<order_seq<<std::endl;
        
        return order.id;
    };
    
    // QueryOrderTrade queries order's trade volume and average price.
    // Parameters
    // int orderId: assigned order Id
    // int averagePrice: return order's average trade price
    // Return:
    // int: return order's trade volume
    int QueryOrderTrade(int orderId, int& averagePrice){
        // Find order by id 
        if(!matched){
            matchOrders();
            matched=1;
        }
        if(result.count(orderId)){
            auto[vol,sum] =result[orderId];
            averagePrice = sum/vol;
            return vol;
        }else{
            
            averagePrice = -1;
            return 0;
        }
        
        
        


    }
};

int main(){
    Exchange ex;
    std::vector<int> orders;
    orders.push_back(ex.InputOrder(0, 1, 100));
    orders.push_back(ex.InputOrder(0, 2, 101));
    orders.push_back(ex.InputOrder(0, 3, 101));
    orders.push_back(ex.InputOrder(0, 3, 102));
    orders.push_back(ex.InputOrder(1, 4, 100));
    orders.push_back(ex.InputOrder(1, 5, 101));
    orders.push_back(ex.InputOrder(1, 6, 102));
    std::cout<<orders.size()<<std::endl;
    for (auto& orderId : orders)
    {
        int averagePrice;
        int tradeVolume = ex.QueryOrderTrade(orderId, averagePrice);
        // std::cout<<"hekko"<<std::endl;
        std::cout << "orderId: " << orderId << " tradeVolume: " << 
        tradeVolume << " averagePrice: " << averagePrice << std::endl;
    }
    return 0;
}
