struct Customer
{
    int baristaIdModel2;
    float arrivalTime,orderTime,brewTime,coffeePrice,orderReadyTime,coffeeReadyTimeModel1,coffeeReadyTimeModel2;
    Customer(){}
    Customer(float arrivalTime,float orderTime,float brewTime,float coffeePrice)
    {
        this->arrivalTime = arrivalTime;
        this->orderTime = orderTime;
        this->brewTime = brewTime;
        this->coffeePrice = coffeePrice;    
    }
};