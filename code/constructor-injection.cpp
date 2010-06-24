#include <QObject>

#include "focusedsessionview.h"
#include "pphonedm.h"

class FocusedSessionClr : public QObject
{
Q_OBJECT;
public:
    FocusedSessionClr(const PPhoneDM& pphone, FocusedSessionViewPtr view, QObject * parent = 0);

private:
    FocusedSessionViewPtr view;

protected slots:
    void focusedSessionChanged(SessionDM* newSession);
};

///somewhere in p8cgmainscreenclr.cpp (I think)
P8CGMainScreenClr::P8CGMainScreenClr() :
    // other initializations...
    focusedSessionClr(PPhoneDM::instance(), FocusedSessionViewPtr(new FocusedSessionView(mainscreen)), this) { }
