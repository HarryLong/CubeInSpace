#include "specie_suitability_list_widget.h"

/*******************************
 * SPECIE PROPERTIES LIST ITEM *
 *******************************/
SpeciePropertiesListItem::SpeciePropertiesListItem ( SpecieProperties specie_properties, TerrainSuitabilityScore suitability_score) :
    QListWidgetItem(), m_specie_properties(specie_properties), m_suitability_score(suitability_score)
{
//    setSpecieProperties(specie_properties);

    float green_ratio(suitability_score.getMaxSuitabilityScore()/100.f);
    float red_ratio(1-green_ratio);
    QColor color(red_ratio*255, green_ratio*255, 0);
    setBackground(color);
    setText(specie_properties.specie_name);
}

SpeciePropertiesListItem::~SpeciePropertiesListItem()
{
}

TerrainSuitabilityScore SpeciePropertiesListItem::getSuitabilityScore() const
{
    return m_suitability_score;
}

SpecieProperties SpeciePropertiesListItem::getSpecieProperties()
{
    return m_specie_properties;
}

bool SpeciePropertiesListItem::operator<(const QListWidgetItem & other) const
{
    const SpeciePropertiesListItem & _other = static_cast<const SpeciePropertiesListItem &>(other);
    return m_suitability_score.getMaxSuitabilityScore() < _other.getSuitabilityScore().getMaxSuitabilityScore();
}


/*********************************
 * SPECIE PROPERTIES LIST WIDGET *
 *********************************/
SpecieSuitabilityListWidget::SpecieSuitabilityListWidget(QWidget * parent) : QListWidget(parent)
{

}

SpecieSuitabilityListWidget::~SpecieSuitabilityListWidget()
{

}

void SpecieSuitabilityListWidget::unselect()
{
    int row(currentRow());
    if(row >= 0)
        item(row)->setSelected(false);
}

void SpecieSuitabilityListWidget::filter(QString filter_string)
{
    hide_all();

    QList<QListWidgetItem*> matches ( findItems(filter_string, Qt::MatchFlag::MatchContains) );
    for(QListWidgetItem* item : matches)
        item->setHidden(false);
}

void SpecieSuitabilityListWidget::hide_all()
{
    for(int row(0); row < count(); row++ )
        item(row)->setHidden(true);
}
