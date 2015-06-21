#ifndef ACTIONS_H
#define ACTIONS_H

#include <map>
#include <QString>
#include <QObject>

class QAction;
class QActionGroup;
/***********************
 * BASE ACTTIONS CLASS *
 ***********************/
class Actions : public QObject {
    Q_OBJECT
public:
    Actions(bool multiselect, bool checkable);
    virtual ~Actions();

    QActionGroup * getActionGroup();
    QAction * operator()(QString key);

protected:
    void finalise();
    std::map<const QString, QAction*> m_actions;

private:
    virtual void init_actions() = 0;

    QActionGroup * m_action_group;
    bool m_multiselect;
    bool m_checkable;
};

/*****************
 * BASE ACTTIONS *
 *****************/
class BaseActions : public Actions{
public:
    const static QString _CLOSE_APP;
    const static QString _LOAD_TERRAIN;
    const static QString _OPEN_SETTINGS;
    BaseActions();
    ~BaseActions();

private:
    virtual void init_actions();
};

/*******************
 * RENDER ACTTIONS *
 *******************/
class RenderActions : public Actions{
public:
    const static QString _GRID;
    const static QString _TERRAIN;
    const static QString _ACCELERATION_STRUCTURE;
    const static QString _RAYS;
    const static QString _SUN;
    RenderActions();
    ~RenderActions();

private:
    virtual void init_actions();
};

/********************
 * CONTROL ACTTIONS *
 ********************/
class ControlActions : public Actions{
public:
    const static QString _SOFTIMAGE;
    const static QString _FPS;
    ControlActions();
    ~ControlActions();

private:
    virtual void init_actions();
};


/********************
 * OVERLAY ACTTIONS *
 ********************/
class OverlayActions : public Actions{
public:
    const static QString _NONE;
    const static QString _SLOPE;
    const static QString _ALTITUDE;
    const static QString _SHADE;
    const static QString _MIN_TEMP;
    const static QString _MAX_TEMP;
    const static QString _MIN_DAILY_ILLUMINATION;
    const static QString _MAX_DAILY_ILLUMINATION;
    OverlayActions();
    ~OverlayActions();

private:
    virtual void init_actions();
};


/****************
 * SHOW ACTIONS *
 ****************/
class ShowActions : public Actions{
public:
    const static QString _POINTER_INFO;
    ShowActions();
    ~ShowActions();

private:
    virtual void init_actions();
};

/*****************
 * EDIT ACTTIONS *
 *****************/
class EditActions : public Actions{
public:
    const static QString _TEMPERATURE;
    const static QString _ORIENTATION;
    const static QString _HUMIDITY;
    const static QString _TIME;
    const static QString _LATITUDE;

    EditActions();
    ~EditActions();

private:
    virtual void init_actions();
};

#endif // ACTIONS_H
