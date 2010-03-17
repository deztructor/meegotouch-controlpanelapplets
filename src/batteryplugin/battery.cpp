#include "battery.h"
#include "batterydbusinterface.h"
#include "batteryimage.h"

#undef DEBUG
#include "../debug.h"

#include <DuiContainer>
#include <DuiControlPanelIf>
#include <DuiGridLayoutPolicy>
#include <DuiGConfItem>
#include <DuiImageWidget>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>

#include <QGraphicsLinearLayout>

const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

Battery::Battery (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                  QGraphicsItem *parent) :
        DuiWidget (parent),
        dbusIf (0),
        statusIndicatorMenu (statusIndicatorMenu),
        modeLabel (0),
        timeLabel (0),
        batteryImage (0),
        container (0),
        PSMode (false),
        last_value (0),
        charging (false)
{
    SYS_DEBUG ("");

    DuiTheme::loadCSS (cssDir + "batteryplugin.css");

    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    // init widgets
    modeLabel = new DuiLabel;
    modeLabel->setObjectName ("batteryModeLabel");
    timeLabel = new DuiLabel;
    timeLabel->setObjectName ("batteryTimeLabel");
    batteryImage = new BatteryImage;
    batteryImage->setObjectName ("batteryImage");

    // Create a container for the battery
    container = new DuiContainer;
    DuiWidget *widget = new DuiWidget;
    DuiLayout *layout = new DuiLayout;
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy (layout);
    widget->setLayout (layout);
    //% "Battery"
    container->setTitle (qtTrId ("qtn_ener_battery"));
    container->setCentralWidget (widget);
    connect (container, SIGNAL (headerClicked ()),
             this, SLOT (showBatteryModificationPage ()));
    mainLayout->addItem (container);

    // insert widgets
    layoutPolicy->addItem(modeLabel, 0, 0, 1, 1);
    layoutPolicy->addItem(timeLabel, 0, 1, 1, 2);
    layoutPolicy->addItem(batteryImage, 1, 0, 1, 2);

    // get widget values
    dbusIf = new BatteryDBusInterface;

    connect (dbusIf, SIGNAL (PSMValueReceived (bool)),
            this, SLOT (updateModeLabel (bool)));
    connect (dbusIf, SIGNAL (remainingTimeValuesReceived (QStringList)),
            this, SLOT (updateTimeLabel (QStringList)));

    connect (dbusIf, SIGNAL (batteryBarValueReceived (int)),
             batteryImage, SLOT (updateBatteryLevel (int)));
    connect (dbusIf, SIGNAL (batteryCharging (int)),
             batteryImage, SLOT (startCharging (int)));
    connect (dbusIf, SIGNAL (batteryNotCharging ()),
             batteryImage, SLOT (stopCharging ()));

    connect (dbusIf, SIGNAL (batteryCharging (int)),
             this, SLOT (charge_start (int)));
    connect (dbusIf, SIGNAL (batteryNotCharging ()),
             this, SLOT (charge_stop ()));
    connect (dbusIf, SIGNAL (batteryFullyCharged ()),
             this, SLOT (charge_done ()));


    dbusIf->PSMValueRequired ();
    dbusIf->remainingTimeValuesRequired ();
    dbusIf->batteryBarValueRequired ();
    dbusIf->batteryChargingStateRequired ();

    SYS_DEBUG ("");
}

Battery::~Battery()
{
    delete dbusIf;
    dbusIf = 0;
}

void
Battery::charge_start (int rate)
{
    Q_UNUSED (rate);

    charging = true;

    //% "Charging"
    timeLabel->setText (qtTrId ("qtn_ener_charging"));
}

void
Battery::charge_done ()
{
    if (charging == true)
    {
        //% "Fully charged"
        timeLabel->setText (qtTrId ("qtn_ener_fullchar"));
    }
}

void
Battery::charge_stop ()
{
    charging = false;

    timeLabel->setText (timeValue (last_value));
}

void
Battery::updateModeLabel (bool toggle)
{
    SYS_DEBUG ("");

    PSMode = toggle;

    modeLabel->setText ((toggle ?
                         //% "Power save mode"
                         qtTrId ("qtn_ener_psmode") :
                         //% "Normal mode"
                         qtTrId ("qtn_ener_normal"))
                       );
}

void
Battery::updateTimeLabel (const QStringList &times)
{
    SYS_DEBUG ("");

    if (times.count () > 0 && times.at (1).toInt () != 0)
        last_value = times.at (1).toInt ();

    // Charging text should be there, not the time...
    if (charging == true)
        return;

    if (times.size () != 2)
        return;

    if (times.at (1).toInt () == 0) // a text, not minute value
        timeLabel->setText (times.at (1));
    else
        timeLabel->setText (timeValue (times.at (1).toInt ()));
}

QString
Battery::timeValue (int minutes)
{
    SYS_DEBUG ("minutes = %d", minutes);

    QString time;

    if (minutes < 15)
    {
        //% "Battery low"
        time = qtTrId ("qtn_ener_battlow");
    }
    else if (minutes < 60)
    {
        //% "Less than an hour"
        time = qtTrId ("qtn_ener_stest_1h");
    }
    else
    {
        //% "%1 hours"
        time = qtTrId ("qtn_ener_stest_hours").arg (minutes / 60);
    }

    return time;
}

void
Battery::showBatteryModificationPage ()
{
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid ())
        return;
    cpIf.appletPage ("Battery");
}

void
Battery::retranslateUi ()
{
    SYS_DEBUG ("");

    if (modeLabel != 0)
        updateModeLabel (PSMode);

    if (container)
        container->setTitle (qtTrId ("qtn_ener_battery"));

    if (timeLabel == 0)
        return;

    if (charging == true)
        charge_start (0);
    else
        timeLabel->setText (timeValue (last_value));
}

