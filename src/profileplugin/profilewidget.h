#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <MButton>

class MStatusIndicatorMenuInterface;
class ProfileDataInterface;
class ProfileButtons;

/*!
 * The Profile widget makes it possible to select the currently
 * active profile.
 */
class ProfileWidget : public MButton
{
    Q_OBJECT

public:
    ProfileWidget (MStatusIndicatorMenuInterface *statusIndicatorMenu,
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
     * \brief A slot for reloading the translations on locale-change
     */
    void loadTranslation ();

private:
    /*!
     * \brief A method for initializing the profile buttons container
     */
    void initProfileButtons ();

private:
    //! Interface for controlling the status indicator menu
    MStatusIndicatorMenuInterface *statusIndicatorMenu;

    //! Profile data If to receive information about possible profiles
    ProfileDataInterface            *dataIf;

    //! Profile buttongroup container
    ProfileButtons                  *profileButtons;

};

#endif // PROFILEWIDGET_H

