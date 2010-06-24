class Monostate
{
public:
    Monostate();

    int getSomeState() { return Monostate::SomeState; }
    int getSomeOtherState { return Monostate::SomeOtherState; }

    void setSomeState(int newState) { Monostate::SomeState = newState; }
    void setSomeOtherState(int newState) { Monostate::SomeOtherState = newState; }

private:
    static int SomeState;
    static int SomeOtherState
};

int Monostate::SomeState = 0;
int Monostate::SomeOtherState = 1;

class Client
{
public:
    int doSomething()
    {
        Monostate ms;
        return ms.getSomeState() + 1 * ms.getSomeOtherState();
    }

    void doSomeOtherThing()
    {
        Monostate ms;
        int previousSomeState = ms.getSomeState();
        ms.setSomeState(doSomething());
        ms.setSomeOtherState(previousSomeState());
    }
};
