class Staff
{
public:
    int id;
    float runTime;
    float emptyTime;
    Staff();
    Staff(int);
    void takeOrder(Customer *cust,float time);
    void brewCoffee(Customer *cust,float time,int modelNo);
    ~Staff();
};
Staff::Staff(int id)
{   
    this->id = id;
    this->runTime=0;
    this->emptyTime =0.0;
}
Staff::Staff()
{
    this->runTime=0;
    this->emptyTime =0.0;
}

Staff::~Staff()
{
}
void Staff::takeOrder(Customer *cust,float time)
{
    this->emptyTime = cust->orderReadyTime = time+cust->orderTime;
    cust->baristaIdModel2 = (id-1)/3+1;
    this->runTime+=cust->orderTime;
}
void Staff::brewCoffee(Customer *cust,float time,int modelNo)
{
    switch (modelNo)
    {
    case 1:
        this->emptyTime = cust->coffeeReadyTimeModel1 = time+cust->brewTime;
    
    case 2:
        this->emptyTime = cust->coffeeReadyTimeModel2 = time+cust->brewTime;
    default:
        this->runTime+=cust->brewTime;
        break;
    }
}
