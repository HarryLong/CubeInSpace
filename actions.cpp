#include "actions.h"

/***********************
 * BASE ACTTIONS CLASS *
 ***********************/
ActionFamily::ActionFamily(bool multiselect, bool checkable) :
    m_multiselect(multiselect), m_checkable(checkable)
{
}

ActionFamily::~ActionFamily()
{
    for(QAction* a : m_actions.values())
        delete a;
}

QAction * ActionFamily::operator[](QString key)
{
    return m_actions[key];
}

std::vector<QAction*> ActionFamily::allActions()
{
    return m_all_actions;
}

QActionGroup * ActionFamily::getActionGroup()
{
    return m_action_group;
}

void ActionFamily::finalise()
{
    m_action_group = new QActionGroup(this);
    m_action_group->setExclusive(!m_multiselect);

    for(QAction * a : m_actions.values())
    {
        a->setActionGroup(m_action_group);
        a->setCheckable(m_checkable);
        m_all_actions.push_back(a);
    }
}

/*****************
 * BASE ACTTIONS *
 *****************/
const QString BaseActionFamily::_CLOSE_APP = "Close";
const QString BaseActionFamily::_LOAD_TERRAIN = "Load Terrain";
const QString BaseActionFamily::_OPEN_SETTINGS = "Open Settings";
BaseActionFamily::BaseActionFamily() : ActionFamily(true, false)
{
    init_actions();
    finalise();
}

BaseActionFamily::~BaseActionFamily()
{

}

void BaseActionFamily::init_actions()
{
    m_actions[BaseActionFamily::_CLOSE_APP] = new QAction(BaseActionFamily::_CLOSE_APP, NULL);
    m_actions[BaseActionFamily::_CLOSE_APP]->setShortcut(QKeySequence::Close);
    m_actions[BaseActionFamily::_LOAD_TERRAIN] = new QAction(BaseActionFamily::_LOAD_TERRAIN, NULL);
    m_actions[BaseActionFamily::_OPEN_SETTINGS] = new QAction(BaseActionFamily::_OPEN_SETTINGS, NULL);
}

/*****************
 * MODE ACTTIONS *
 *****************/
const QString ModeActionFamily::_RESOURCE_EDIT = "Resource edit";
const QString ModeActionFamily::_CLUSTERING = "Clustering";
const QString ModeActionFamily::_PLANT_EDIT = "Plant edit";
ModeActionFamily::ModeActionFamily() : ActionFamily(false, true)
{
    init_actions();
    finalise();
}

ModeActionFamily::~ModeActionFamily()
{

}

void ModeActionFamily::init_actions()
{
    m_actions[ModeActionFamily::_RESOURCE_EDIT] = new QAction(ModeActionFamily::_RESOURCE_EDIT, NULL);
    m_actions[ModeActionFamily::_CLUSTERING] = new QAction(ModeActionFamily::_CLUSTERING, NULL);
    m_actions[ModeActionFamily::_PLANT_EDIT] = new QAction(ModeActionFamily::_PLANT_EDIT, NULL);
}

/*******************
 * RENDER ACTTIONS *
 *******************/
const QString RenderActionFamily::_GRID = "Grid";
const QString RenderActionFamily::_TERRAIN = "Terrain";
const QString RenderActionFamily::_RAYS = "Rays";
const QString RenderActionFamily::_SUN = "Sun";
RenderActionFamily::RenderActionFamily() :  ActionFamily(true, true)
{
    init_actions();
    finalise();

    m_actions[RenderActionFamily::_GRID]->setChecked(true);
    m_actions[RenderActionFamily::_TERRAIN]->setChecked(true);
}

RenderActionFamily::~RenderActionFamily()
{

}

void RenderActionFamily::init_actions()
{
    m_actions[RenderActionFamily::_GRID] = new QAction(RenderActionFamily::_GRID, NULL);
    m_actions[RenderActionFamily::_TERRAIN] = new QAction(RenderActionFamily::_TERRAIN, NULL);
    m_actions[RenderActionFamily::_RAYS] = new QAction(RenderActionFamily::_RAYS, NULL);
    m_actions[RenderActionFamily::_SUN] = new QAction(RenderActionFamily::_SUN, NULL);
}

/********************
 * CONTROL ACTTIONS *
 ********************/
const QString ControlActionFamily::_SOFTIMAGE = "Softimage";
const QString ControlActionFamily::_FPS = "FPS";
ControlActionFamily::ControlActionFamily() : ActionFamily(false, true)
{
    init_actions();
    finalise();

    m_actions[ControlActionFamily::_SOFTIMAGE]->setChecked(true);
}

ControlActionFamily::~ControlActionFamily()
{

}

void ControlActionFamily::init_actions()
{
    m_actions[ControlActionFamily::_SOFTIMAGE] = new QAction(ControlActionFamily::_SOFTIMAGE, NULL);
    m_actions[ControlActionFamily::_FPS] = new QAction(ControlActionFamily::_FPS, NULL);
}

/********************
 * OVERLAY ACTTIONS *
 ********************/
const QString OverlayActionFamily::_NONE = "None";
const QString OverlayActionFamily::_SLOPE = "Slope";
const QString OverlayActionFamily::_ALTITUDE = "Altitude";
const QString OverlayActionFamily::_SHADE = "Shade";
const QString OverlayActionFamily::_TEMPERATURE = "Temperature";
const QString OverlayActionFamily::_ILLUMINATION = "Illumination";
const QString OverlayActionFamily::_SOIL_INFILTRATION_RATE = "Soil Infiltration Rate";
const QString OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY = "Monthly";
const QString OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY = "Weighted average";
const QString OverlayActionFamily::_CLUSTERS = "Clusters";
OverlayActionFamily::OverlayActionFamily() : ActionFamily(false, true)
{
    init_actions();
    finalise();

    m_actions[OverlayActionFamily::_NONE]->setChecked(true);
}

OverlayActionFamily::~OverlayActionFamily()
{

}

void OverlayActionFamily::init_actions()
{
    m_actions[OverlayActionFamily::_NONE] = new QAction(OverlayActionFamily::_NONE, NULL);
    m_actions[OverlayActionFamily::_SLOPE] = new QAction(OverlayActionFamily::_SLOPE, NULL);
    m_actions[OverlayActionFamily::_ALTITUDE] = new QAction(OverlayActionFamily::_ALTITUDE, NULL);
    m_actions[OverlayActionFamily::_SHADE] = new QAction(OverlayActionFamily::_SHADE, NULL);
    m_actions[OverlayActionFamily::_TEMPERATURE] = new QAction(OverlayActionFamily::_TEMPERATURE, NULL);
    m_actions[OverlayActionFamily::_ILLUMINATION] = new QAction(OverlayActionFamily::_ILLUMINATION, NULL);
    m_actions[OverlayActionFamily::_SOIL_INFILTRATION_RATE] = new QAction(OverlayActionFamily::_SOIL_INFILTRATION_RATE, NULL);
    m_actions[OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY] = new QAction(OverlayActionFamily::_MONTHLY_SOIL_HUMIDITY, NULL);
    m_actions[OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY] = new QAction(OverlayActionFamily::_WEIGHTED_AVG_SOIL_HUMIDITY, NULL);
    m_actions[OverlayActionFamily::_CLUSTERS] = new QAction(OverlayActionFamily::_CLUSTERS, NULL);
}

/****************
 * SHOW ACTIONS *
 ****************/
const QString ShowActionFamily::_POINTER_INFO = "Pointer Info";
const QString ShowActionFamily::_CLUSTERS_INFO = "Clusters Info";
const QString ShowActionFamily::_PLANT_SELECTION = "Plant Selection";
ShowActionFamily::ShowActionFamily() : ActionFamily(true, true)
{
    init_actions();
    finalise();

    m_actions[ShowActionFamily::_POINTER_INFO]->setChecked(false);
    m_actions[ShowActionFamily::_CLUSTERS_INFO]->setChecked(false);
    m_actions[ShowActionFamily::_PLANT_SELECTION]->setChecked(false);
}

ShowActionFamily::~ShowActionFamily()
{

}

void ShowActionFamily::init_actions()
{
    m_actions[ShowActionFamily::_POINTER_INFO] = new QAction(ShowActionFamily::_POINTER_INFO, NULL);
    m_actions[ShowActionFamily::_CLUSTERS_INFO] = new QAction(ShowActionFamily::_CLUSTERS_INFO, NULL);
    m_actions[ShowActionFamily::_PLANT_SELECTION] = new QAction(ShowActionFamily::_PLANT_SELECTION, NULL);
}

/*****************
 * EDIT ACTTIONS *
 *****************/
const QString EditActionFamily::_TEMPERATURE = "Temperature";
const QString EditActionFamily::_ORIENTATION = "Orientation";
const QString EditActionFamily::_TIME = "Time";
const QString EditActionFamily::_LATITUDE = "Latitude";
const QString EditActionFamily::_MONTHLY_RAINFALL = "Monthly Rainfall";
const QString EditActionFamily::_SOIL_INFILTRATION_RATE = "Soil Infiltration Rate";
const QString EditActionFamily::_FLOOD_FILL = "Flood fill";
const QString EditActionFamily::_ABSOLUTE_AGGREGATE_HEIGHT = "Absolute Aggregate Height";
EditActionFamily::EditActionFamily() : ActionFamily(true, true)
{
    init_actions();
    finalise();

    m_actions[EditActionFamily::_TEMPERATURE]->setCheckable(false);
    m_actions[EditActionFamily::_MONTHLY_RAINFALL]->setCheckable(false);
}

EditActionFamily::~EditActionFamily()
{

}

void EditActionFamily::init_actions()
{
    m_actions[EditActionFamily::_TEMPERATURE] = new QAction(EditActionFamily::_TEMPERATURE, NULL);
    m_actions[EditActionFamily::_ORIENTATION] = new QAction(EditActionFamily::_ORIENTATION, NULL);
    m_actions[EditActionFamily::_TIME] = new QAction(EditActionFamily::_TIME, NULL);
    m_actions[EditActionFamily::_LATITUDE] = new QAction(EditActionFamily::_LATITUDE, NULL);
    m_actions[EditActionFamily::_MONTHLY_RAINFALL] = new QAction(EditActionFamily::_MONTHLY_RAINFALL, NULL);
    m_actions[EditActionFamily::_SOIL_INFILTRATION_RATE] = new QAction(EditActionFamily::_SOIL_INFILTRATION_RATE, NULL);
    m_actions[EditActionFamily::_FLOOD_FILL] = new QAction(EditActionFamily::_FLOOD_FILL, NULL);
    m_actions[EditActionFamily::_ABSOLUTE_AGGREGATE_HEIGHT] = new QAction(EditActionFamily::_ABSOLUTE_AGGREGATE_HEIGHT, NULL);
}
