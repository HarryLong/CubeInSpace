#ifndef SPECIE_PROPERTIES_LIST_WIDGET_H
#define SPECIE_PROPERTIES_LIST_WIDGET_H

#include <QListWidget>
#include "plantDB/plant_properties.h"
#include "../clustering/terrain_suitability_score.h"

class SpeciePropertiesListItem : public QListWidgetItem
{
public:
    SpeciePropertiesListItem ( SpecieProperties specie_properties, TerrainSuitabilityScore suitability_score);
    ~SpeciePropertiesListItem ();
    TerrainSuitabilityScore getSuitabilityScore() const;
    bool operator<(const QListWidgetItem & other) const override;

private:
    SpecieProperties m_specie_properties;
    const TerrainSuitabilityScore m_suitability_score;
};

/*********************************
 * SPECIE PROPERTIES LIST WIDGET *
 *********************************/
class SpecieSuitabilityListWidget : public QListWidget
{
Q_OBJECT
public:
    SpecieSuitabilityListWidget( QWidget * parent = 0 );
    ~SpecieSuitabilityListWidget();

    void filter(QString filter_string);

public slots:
    void unselect();

private:
    void hide_all();
    void init_layout(QString title);
};

#endif
