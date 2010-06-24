class SingletonFactory

class SingletonInterface
{
public:
    virtual void someOperation() = 0;
    virtual void someOtherOperation() = 0;
};

class Singleton : public SingletonInterface
{
public:
    void someOperation() { /*do something with state*/ }
    void someOtherOperation() { /*do something else with state*/ }
private:
    int state;
};

class SingletonFactory
{
public:
    SingletonInterface& getSingleton() 
    { 
        static Singleton instance;
        return instance;
    }

    static void Initialize() {
        (void) getSingleton();
    }
};

class Client
{
public:
    Client(SingletonFactory factory = SingletonFactory());
    void doStuff()
    {
        factory.getSingleton().someOperation();
        factory.getSingleton().someOtherOperation();
    }
private:
    SingletonFactory factory;
}

int main()
{
    //define the point of initialization for the single instance
    SingletonFactory::Initialize();
    Client client;
    client.doStuff();
}

