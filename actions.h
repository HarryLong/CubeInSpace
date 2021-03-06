#ifndef ACTIONS_H
#define ACTIONS_H

#include <map>
#include <QString>
#include <QObject>
#include <QHash>
#include <QAction>
#include <QActionGroup>

/****************************
 * BASE FAMILY ACTION CLASS *
 ****************************/
class ActionFamily : public QObject {
    Q_OBJECT
public:
    ActionFamily(bool multiselect, bool checkable);
    virtual ~ActionFamily();

    QActionGroup * getActionGroup();
    QAction * operator[](QString key);
    std::vector<QAction*> allActions();

protected:
    void finalise();
    QHash<const QString, QAction*> m_actions;

private:
    virtual void init_actions() = 0;

    QActionGroup * m_action_group;
    std::vector<QAction*> m_all_actions;
    bool m_multiselect;
    bool m_checkable;
};

/*****************
 * BASE ACTTIONS *
 *****************/
class BaseActionFamily : public ActionFamily{
public:
    const static QString _CLOSE_APP;
    const static QString _LOAD_TERRAIN;
    const static QString _OPEN_SETTINGS;
    BaseActionFamily();
    ~BaseActionFamily();

private:
    virtual void init_actions();
};

/*******************
 * RENDER ACTTIONS *
 *******************/
class RenderActionFamily : public ActionFamily{
public:
    const static QString _GRID;
    const static QString _TERRAIN;
    const static QString _RAYS;
    const static QString _SUN;
    RenderActionFamily();
    ~RenderActionFamily();

private:
    virtual void init_actions();
};

/********************
 * OVERLAY ACTTIONS *
 ********************/
class OverlayActionFamily : public ActionFamily{
public:
    const static QString _NONE;
    const static QString _SLOPE;
    const static QString _ALTITUDE;
    const static QString _SHADE;
    const static QString _TEMPERATURE;
    const static QString _ILLUMINATION;
    const static QString _SOIL_INFILTRATION_RATE;
    const static QString _MONTHLY_SOIL_HUMIDITY;
    const static QString _WEIGHTED_AVG_SOIL_HUMIDITY;
    const static QString _CLUSTERS;
    OverlayActionFamily();
    ~OverlayActionFamily();

private:
    virtual void init_actions();
};


/****************
 * SHOW ACTIONS *
 ****************/
class ShowActionFamily : public ActionFamily{
public:
    const static QString _POINTER_INFO;

    ShowActionFamily();
    ~ShowActionFamily();

private:
    virtual void init_actions();
};

/*****************
 * EDIT ACTTIONS *
 *****************/
class EditActionFamily : public ActionFamily{
public:
    const static QString _TEMPERATURE;
    const static QString _ORIENTATION;
    const static QString _TIME;
    const static QString _LATITUDE;
    const static QString _MONTHLY_RAINFALL;
    const static QString _SOIL_INFILTRATION_RATE;
    const static QString _FLOOD_FILL;
    const static QString _ABSOLUTE_AGGREGATE_HEIGHT;

    EditActionFamily();
    ~EditActionFamily();

private:
    virtual void init_actions();
};

class AllActions
{
public:
    AllActions() {}
    ~AllActions() {}

    BaseActionFamily m_base_actions;
    RenderActionFamily m_render_actions;
    OverlayActionFamily m_overlay_actions;
    ShowActionFamily m_show_actions;
    EditActionFamily m_edit_actions;
};

#endif // ACTIONS_H
