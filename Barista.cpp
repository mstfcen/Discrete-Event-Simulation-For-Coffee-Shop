class Barista
{
public:
    int id;
    float runTime;
    float emptyTime;
    Barista();
    ~Barista();
};

Barista::Barista()
{
    this->runTime=0;
    this->id=0;
    this->emptyTime =-1.0;
}

Barista::~Barista()
{
}
