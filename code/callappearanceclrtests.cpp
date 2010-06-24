#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <QCoreApplication>
#include <QSharedPointer>

#include "qstringmatchers.h"

#include "p8cgcallappearanceclr.h"
#include "p8cgcallappearanceview.h"
#include "sessiondm.h"
#include "builders.h"

struct Icons
{
    static const QString DIALING;
    static const QString INCOMING;
    static const QString CONNECTED;
    static const QString LOCAL_HOLD;
};

const QString Icons::DIALING(":/images/callappearance/dialing.png");
const QString Icons::INCOMING(":/images/callappearance/incoming.png");
const QString Icons::CONNECTED(":/images/callappearance/connected.png");
const QString Icons::LOCAL_HOLD(":/images/callappearance/local_hold.png");

class MockP8CGCallAppearanceView : public P8CGCallAppearanceView
{
public:
    MockP8CGCallAppearanceView() : P8CGCallAppearanceView(0) { }
    MOCK_METHOD1(setOrientation, void(Orientation orientation));
    MOCK_METHOD1(setLabel, void(const QString& label));
    MOCK_METHOD1(setIcon, void(const QString& iconPath));
    MOCK_METHOD1(setDefaultIcon, void(const QString& iconPath));
    MOCK_METHOD1(setActiveBackground, void(const QString& backgroundPath));
    MOCK_METHOD0(clear, void());
    MOCK_METHOD0(activate, void());
    MOCK_METHOD0(deactivate, void());
    MOCK_METHOD0(showCallTimer, void());
    MOCK_METHOD0(hideCallTimer, void());
    MOCK_METHOD1(updateCallTime, void(const QString&));
};

TEST(CallAppearanceClrTests, shouldGoOffHookWhenAssignedADialingSession)
{
    using ::testing::NiceMock;
   
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::DIALING)));
    EXPECT_CALL(view, activate());
    
    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsInitialized();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldClearViewWhenTerminatedWhileDialing)
{
    using ::testing::NiceMock;
    using ::testing::AtLeast;

    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, clear()).Times(AtLeast(2));

    P8CGCallAppearanceClr controller(0, &view);

    SessionPtr session = New.Session().thatIsInitialized();
    controller.setSession(session.data());
    
    session->state.setInt(SessionDM::kTerminated);
}

TEST(CallAppearanceClrTests, shouldDisconnectWhenSessionTerminatesWhileDialing)
{
    using ::testing::NiceMock;

    
    NiceMock<MockP8CGCallAppearanceView> view;

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsInitialized();
    controller.setSession(session.data());
    
    session->state.setInt(SessionDM::kTerminated);
    EXPECT_EQ(0, controller.session()) << "Session should be nulled after disconnection";
}

TEST(CallAppearanceClrTests, shouldSetDisplayNameWhenAvailable)
{
    using ::testing::NiceMock;

    
    const QString DISPLAY_NAME = "Fred";

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setLabel(QStringEq(DISPLAY_NAME)));
   
    SessionPtr session = New.Session().thatIsRingingRemotely()
                                       .thatHasRemoteDisplayName(DISPLAY_NAME);
    P8CGCallAppearanceClr controller(0, &view);

    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldUseExtensionWhenDisplayNameUnavailable)
{
    using ::testing::NiceMock;

    
    const QString EXTENSION = "1111";
    SipUri uri;
    uri.setUserName(EXTENSION);
    uri.setHost("example.com");
    uri.setScheme("sip");

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setLabel(QStringEq(EXTENSION)));

    SessionPtr session = New.Session().thatIsRingingRemotely()
                                       .thatHasRemoteUri(uri);
    P8CGCallAppearanceClr controller(0, &view);

    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldClearViewWhenTerminatedWhileConnected)
{
    using ::testing::NiceMock;
    using ::testing::AtLeast;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, clear()).Times(AtLeast(2));

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsConnected();
    
    controller.setSession(session.data());
    session->state.setInt(SessionDM::kTerminated);
}

TEST(CallAppearanceClrTests, shouldDisplayOffHookIfSessionIsRingingRemote)
{
    using ::testing::NiceMock;
    
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::DIALING)));
    EXPECT_CALL(view, activate());
    
    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsRingingRemotely();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldDisplayIncomingCallIconAndActivateOnIncomingCall)
{
    using ::testing::NiceMock;

    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::INCOMING)));
    EXPECT_CALL(view, activate());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsRingingLocally();

    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldShowCallTimerWhenConnected)
{
    using ::testing::NiceMock;

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, showCallTimer());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsConnected()
                                       .thatHasStartedAt(QDateTime::fromString("1970-01-01T00:00:00", Qt::ISODate));

    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldShowConnectedIconWhenConnected)
{
    using ::testing::NiceMock;

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::CONNECTED)));
    
    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsConnected()
                                       .thatHasStartedAt(QDateTime::fromString("1970-01-01T00:00:00", Qt::ISODate));

    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldUpdateCallTimeOncePerSecond)
{
    using ::testing::NiceMock;
    using ::testing::AtLeast;
    using ::testing::_;

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, updateCallTime(_)).Times(AtLeast(2));


    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsConnected()
                                       .thatHasStartedAt(QDateTime::fromString("1970-01-01T00:00:00", Qt::ISODate));

    controller.setSession(session.data());
    sleep(1);
    QCoreApplication::processEvents();
    sleep(1);
    QCoreApplication::processEvents();
}

TEST(CallAppearanceClrTests, shouldShowLocalHoldIconWhenOnLocalHold)
{
    using ::testing::NiceMock;

    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::LOCAL_HOLD)));

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsOnLocalHold();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldDeactivateWhenOnLocalHold)
{
    using ::testing::NiceMock;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, deactivate());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsOnLocalHold();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldShowLocalHoldIconWhenBothSidesAreHeld)
{
    using ::testing::NiceMock;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(QStringEq(Icons::LOCAL_HOLD)));

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsHeldOnBothSides();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldDeactivateWhenBothSidesAreHeld)
{
    using ::testing::NiceMock;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, deactivate());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsHeldOnBothSides();
    
    controller.setSession(session.data());
}

TEST(CallAppearanceClrTests, shouldReturnToConnectedWhenALocalHoldIsRemoved)
{
    using ::testing::NiceMock;
    using ::testing::_;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(_));
    EXPECT_CALL(view, deactivate());
    EXPECT_CALL(view, setIcon(QStringEq(Icons::CONNECTED)));
    EXPECT_CALL(view, activate());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsOnLocalHold();
    
    controller.setSession(session.data());

    session->streamList().at(0)->direction.setInt(StreamDM::kDirection_SendReceive);
}

TEST(CallAppearanceClrTests, shouldReturnToConnectedWhenAllHoldsAreRemoved)
{
    using ::testing::NiceMock;
    using ::testing::_;
    
    NiceMock<MockP8CGCallAppearanceView> view;
    EXPECT_CALL(view, setIcon(_));
    EXPECT_CALL(view, deactivate());
    EXPECT_CALL(view, setIcon(QStringEq(Icons::CONNECTED)));
    EXPECT_CALL(view, activate());

    P8CGCallAppearanceClr controller(0, &view);
    SessionPtr session = New.Session().thatIsHeldOnBothSides();
    
    controller.setSession(session.data());

    session->streamList().at(0)->direction.setInt(StreamDM::kDirection_SendReceive);
}
