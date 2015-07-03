#include "actions.h"
#include <QAction>
#include <QActionGroup>

/***********************
 * BASE ACTTIONS CLASS *
 ***********************/
Actions::Actions(bool multiselect, bool checkable) :
    m_multiselect(multiselect), m_checkable(checkable)
{
}

Actions::~Actions()
{
    for(std::pair<const QString, QAction*> a : m_actions)
        delete a.second;
}

QAction * Actions::operator()(QString key)
{
    return m_actions[key];
}

QActionGroup * Actions::getActionGroup()
{
    return m_action_group;
}

void Actions::finalise()
{
    m_action_group = new QActionGroup(this);
    m_action_group->setExclusive(!m_multiselect);

    for(std::pair<const QString, QAction*> a : m_actions)
    {
        a.second->setActionGroup(m_action_group);
        a.second->setCheckable(m_checkable);
    }
}


/*****************
 * BASE ACTTIONS *
 *****************/
const QString BaseActions::_CLOSE_APP = "Close";
const QString BaseActions::_LOAD_TERRAIN = "Load Terrain";
const QString BaseActions::_OPEN_SETTINGS = "Open Settings";
BaseActions::BaseActions() : Actions(true, false)
{
    init_actions();
    finalise();
}

BaseActions::~BaseActions()
{

}

void BaseActions::init_actions()
{
    m_actions[BaseActions::_CLOSE_APP] = new QAction(BaseActions::_CLOSE_APP, NULL);
    m_actions[BaseActions::_CLOSE_APP]->setShortcut(QKeySequence::Close);

    m_actions[BaseActions::_LOAD_TERRAIN] = new QAction(BaseActions::_LOAD_TERRAIN, NULL);
    m_actions[BaseActions::_OPEN_SETTINGS] = new QAction(BaseActions::_OPEN_SETTINGS, NULL);
}

/*******************
 * RENDER ACTTIONS *
 *******************/
const QString RenderActions::_GRID = "Grid";
const QString RenderActions::_TERRAIN = "Terrain";
const QString RenderActions::_ACCELERATION_STRUCTURE = "Acceleration Structure";
const QString RenderActions::_RAYS = "Rays";
const QString RenderActions::_SUN = "Sun";
RenderActions::RenderActions() :  Actions(true, true)
{
    init_actions();
    finalise();

    m_actions[RenderActions::_GRID]->setChecked(true);
    m_actions[RenderActions::_TERRAIN]->setChecked(true);
}

RenderActions::~RenderActions()
{

}

void RenderActions::init_actions()
{
    m_actions[RenderActions::_GRID] = new QAction(RenderActions::_GRID, NULL);
    m_actions[RenderActions::_TERRAIN] = new QAction(RenderActions::_TERRAIN, NULL);
    m_actions[RenderActions::_ACCELERATION_STRUCTURE] = new QAction(RenderActions::_ACCELERATION_STRUCTURE, NULL);
    m_actions[RenderActions::_RAYS] = new QAction(RenderActions::_RAYS, NULL);
    m_actions[RenderActions::_SUN] = new QAction(RenderActions::_SUN, NULL);
}

/********************
 * CONTROL ACTTIONS *
 ********************/
const QString ControlActions::_SOFTIMAGE = "Softimage";
const QString ControlActions::_FPS = "FPS";
ControlActions::ControlActions() : Actions(false, true)
{
    init_actions();
    finalise();

    m_actions[ControlActions::_SOFTIMAGE]->setChecked(true);
}

ControlActions::~ControlActions()
{

}

void ControlActions::init_actions()
{
    m_actions[ControlActions::_SOFTIMAGE] = new QAction(ControlActions::_SOFTIMAGE, NULL);
    m_actions[ControlActions::_FPS] = new QAction(ControlActions::_FPS, NULL);
}

/********************
 * OVERLAY ACTTIONS *
 ********************/
const QString OverlayActions::_NONE = "None";
const QString OverlayActions::_SLOPE = "Slope";
const QString OverlayActions::_ALTITUDE = "Altitude";
const QString OverlayActions::_SHADE = "Shade";
const QString OverlayActions::_MIN_TEMP = "Min. Temperature";
const QString OverlayActions::_MAX_TEMP = "Max. Temperature";
const QString OverlayActions::_MIN_DAILY_ILLUMINATION = "Min. Daily Illumination";
const QString OverlayActions::_MAX_DAILY_ILLUMINATION = "Max. Daily Illumination";
OverlayActions::OverlayActions() : Actions(false, true)
{
    init_actions();
    finalise();

    m_actions[OverlayActions::_NONE]->setChecked(true);
}

OverlayActions::~OverlayActions()
{

}

void OverlayActions::init_actions()
{
    m_actions[OverlayActions::_NONE] = new QAction(OverlayActions::_NONE, NULL);
    m_actions[OverlayActions::_SLOPE] = new QAction(OverlayActions::_SLOPE, NULL);
    m_actions[OverlayActions::_ALTITUDE] = new QAction(OverlayActions::_ALTITUDE, NULL);
    m_actions[OverlayActions::_SHADE] = new QAction(OverlayActions::_SHADE, NULL);
    m_actions[OverlayActions::_MIN_TEMP] = new QAction(OverlayActions::_MIN_TEMP, NULL);
    m_actions[OverlayActions::_MAX_TEMP] = new QAction(OverlayActions::_MAX_TEMP, NULL);
    m_actions[OverlayActions::_MIN_DAILY_ILLUMINATION] = new QAction(OverlayActions::_MIN_DAILY_ILLUMINATION, NULL);
    m_actions[OverlayActions::_MAX_DAILY_ILLUMINATION] = new QAction(OverlayActions::_MAX_DAILY_ILLUMINATION, NULL);
}

/****************
 * SHOW ACTIONS *
 ****************/
const QString ShowActions::_POINTER_INFO = "Pointer Info";
ShowActions::ShowActions() : Actions(true, true)
{
    init_actions();
    finalise();

    m_actions[ShowActions::_POINTER_INFO]->setChecked(false);
}

ShowActions::~ShowActions()
{

}

void ShowActions::init_actions()
{
    m_actions[ShowActions::_POINTER_INFO] = new QAction(ShowActions::_POINTER_INFO, NULL);
}


/*****************
 * EDIT ACTTIONS *
 *****************/
const QString EditActions::_TEMPERATURE = "Temperature";
const QString EditActions::_ORIENTATION = "Orientation";
const QString EditActions::_HUMIDITY = "Humidity";
const QString EditActions::_TIME_OF_DAY = "Time of day";
const QString EditActions::_MONTH_OF_YEAR = "Month of year";
const QString EditActions::_LATITUDE = "Latitude";
EditActions::EditActions() : Actions(true, true)
{
    init_actions();
    finalise();
}

EditActions::~EditActions()
{

}

void EditActions::init_actions()
{
    m_actions[EditActions::_TEMPERATURE] = new QAction(EditActions::_TEMPERATURE, NULL);
    m_actions[EditActions::_ORIENTATION] = new QAction(EditActions::_ORIENTATION, NULL);
    m_actions[EditActions::_HUMIDITY] = new QAction(EditActions::_HUMIDITY, NULL);
    m_actions[EditActions::_TIME_OF_DAY] = new QAction(EditActions::_TIME_OF_DAY, NULL);
    m_actions[EditActions::_MONTH_OF_YEAR] = new QAction(EditActions::_MONTH_OF_YEAR, NULL);
    m_actions[EditActions::_LATITUDE] = new QAction(EditActions::_LATITUDE, NULL);
}

/*****************
 * TMP ACTTIONS *
 *****************/
const QString TmpActions::_ACTION1 = "ACTION 1";
TmpActions::TmpActions() :  Actions(true, false)
{
    init_actions();
    finalise();
}

TmpActions::~TmpActions()
{

}

void TmpActions::init_actions()
{
    m_actions[TmpActions::_ACTION1] = new QAction(TmpActions::_ACTION1, NULL);
    m_actions[TmpActions::_ACTION1]->setShortcut(QKeySequence::Copy);
}
