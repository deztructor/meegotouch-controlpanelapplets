/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILECONTAINER_H
#define PROFILECONTAINER_H

#include <MWidget>

class MButton;
class MLabel;

/*!
 * A widget that shows and controls a profile's vibration on/off
 * property.
 */
class ProfileContainer: public MWidget
{
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)

public:
    ProfileContainer(
            int            id, 
            const QString &title, 
            bool           vibra, 
            MWidget     *parent = 0);
    ~ProfileContainer ();

    int id ();
    bool isChecked() const;
    void setChecked(bool buttonChecked);
    void setText(const QString &text);
    QString text() const;

signals:
    void toggled(bool checked);

private:
    const int      m_ProfileId;
    MButton        *m_Button;
    MLabel         *m_Label;
};

#endif
