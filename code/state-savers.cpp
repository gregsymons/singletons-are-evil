class noncopyable
{
private:
    noncopyable(const noncopyably&);
    noncopyable& operator=(const noncopyable&);
};

class Singleton : private noncopyable
{
public:
    static Singleton& Instance() 
    {
        static Singleton instance;
        return instance;
    }

    int getState() { return state; }
    Singleton& setState(int newState) { state = newState; }
private:
    Singleton() : state(0) { };
    ~Singleton() { };

    int state;
};

class SingletonStateSaver
{
public:
    SingletonStateSaver() : { saveState(); }
    ~SingletonStateSaver() { restoreState() }
    void saveState() { savedState = Singletone::Instance().getState(); }
    void restoreState(){ Singleton::Instance().setState(savedState); }
};

class Client
{
public:
    void doSomething()
    {
        SingletonStateSaver s;

        Singleton::Instance().setState(5);
    }
};

