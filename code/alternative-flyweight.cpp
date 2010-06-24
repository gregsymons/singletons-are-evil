class SingletonInterface
{
public:
    virtual void ~SingletonInterface() { }
    virtual void someOperation() = 0;
    virtual void someOtherOperation() = 0;
}

class Singleton : public SingletonInterface
{
public:
    void someOperation() { /* do stuff with state */ }
    void someOtherOperation() { /* do other stuff with state */ }
private:
    int state;
};

SingletonInterface& GetDefaultSingleton() 
{ 
    static Singleton instance;
    return instance
}
