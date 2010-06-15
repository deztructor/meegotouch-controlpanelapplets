/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <MWidget>

class MButtonGroup;

class ProfileButtons : public MWidget
{
    Q_OBJECT

public:
    ProfileButtons (MWidget *parent = 0);
    virtual ~ProfileButtons();

    void init (QMap<int, QPair<QString, QString> > data, int currentId);
    QString selectedProfileName();

    void retranslate (QMap<int, QString> data);

public slots:
    bool selectProfile(int id);

signals:
    void profileSelected(int id);

private:
    void addButton(QString iconID, QString name, int id, bool checked);
    void createLayout();

private:
    MButtonGroup *m_Buttons;
    #ifdef UNIT_TEST
    friend class Ut_ProfileButtons;
    #endif
};

#endif 
