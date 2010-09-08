#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <MButton>
#include <QPointer>

class MStatusIndicatorMenuInterface;
class ProfileDataInterface;
class ProfileButtons;
class ProfilePlugin;
class MDialog;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class ProfileWidget : public MButton
{
    Q_OBJECT

public:
    ProfileWidget (ProfilePlugin *plugin,
                   QGraphicsItem *parent = NULL);
    virtual ~ProfileWidget ();
    /*!
     * \brief A method for retranslating the UI
     */
    void retranslateUi ();

private slots:
    /*!
     * \brief Shows the profile dialog
     */
    void showProfileDialog();

    /*!
     * \brief Changes the button according to current profile
     */
    void profileChanged();

private:
    /*!
     * \brief A method for initializing the profile buttons container
     */
    void initProfileButtons ();

    /*!
     * \brief A method for loading the needed translation catalogue
     */
    void loadTranslation ();

private:
    //! Interface for controlling the status indicator menu
    ProfilePlugin                   *plugin;

    //! Profile data If to receive information about possible profiles
    ProfileDataInterface            *dataIf;

    //! Profile buttongroup container
    ProfileButtons                  *profileButtons;

    //! The profile selection dialog
    QPointer<MDialog>                profileDialog;

};

#endif // PROFILEWIDGET_H

