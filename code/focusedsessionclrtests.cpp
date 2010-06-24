#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "pphonedm.h"
#include "sessiondm.h"
#include "focusedsessionview.h"
#include "focusedsessionclr.h"
#include "builders.h"

class MockFocusedSessionView : public FocusedSessionView
{
public:
    MockFocusedSessionView() : FocusedSessionView(0) { }
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(hide, void());
    MOCK_METHOD0(configureForDialing, void());
    MOCK_METHOD1(updateDialString, void(const QString&));

    operator FocusedSessionViewPtr() { return FocusedSessionViewPtr(this, null_deleter()); }

private:
    struct null_deleter {
        void operator ()(FocusedSessionView*) { }
    };
};

class FocusedSessionClrTests : public ::testing::Test
{
public:
    FocusedSessionClrTests() : 
        Test(),
        phoneModel(PPhoneDM::singleton()) { }
    
    ~FocusedSessionClrTests()
    {
        cleanupPhoneModel();
    }

protected:
    PPhoneDM& phoneModel;
    
    virtual void SetUp()
    {
        cleanupPhoneModel();
    }
private:
    void cleanupPhoneModel()
    {
        phoneModel.isHandsetLifted.setBool(false);
        phoneModel.maxCallContexts.setInt(6);
        phoneModel.activeRegistration.set(0);
        phoneModel.activeSession.set(0);
        phoneModel.activeSession.set(0);
    }

};

TEST_F(FocusedSessionClrTests, shouldShowFocusedSessionViewWhenFocusedSessionIsSet)
{
    using ::testing::NiceMock;

    SessionDM session;
    MockFocusedSessionView view;
    FocusedSessionClr clr(phoneModel, view);

    EXPECT_CALL(view, show());
    
    phoneModel.focusedSession.set(&session);
}

TEST_F(FocusedSessionClrTests, shouldHideFocusedSessionViewWhenFocusedSessionIsCleared)
{
    SessionDM session;
    phoneModel.focusedSession.set(&session);
    
    MockFocusedSessionView view;
    FocusedSessionClr clr(phoneModel, view);

    EXPECT_CALL(view, hide());
    
    phoneModel.focusedSession.set(0);
}

TEST_F(FocusedSessionClrTests, shouldPutTheFocusedWindowViewInDialingModeWhenADialingSessionIsSet)
{
    SessionPtr session = New.Session().thatIsInitialized();
    MockFocusedSessionView view;
    FocusedSessionClr clr(phoneModel, view);

    EXPECT_CALL(view, configureForDialing());

    phoneModel.focusedSession.set(session.data());
}

TEST_F(FocusedSessionClrTests, shouldEchoDialStringOnTheView)
{
    SessionPtr session = New.Session().thatIsInitialized();
    phoneModel.focusedSession.set(session.data());
    QString newDialString("1");

    MockFocusedSessionView view;
    FocusedSessionClr(phoneModel, view);

    EXPECT_CALL(view, updateDialString(newDialString));
    session->dialString().text.setString(newDialString);
}
