#include "dialogs.h"

Dialogs::Dialogs(QWidget * parent) :
    m_settings_dlg(parent), m_temp_editor_dlg(parent), m_pointer_info_dlg(parent), m_monthly_rainfall_edit_dlg(parent),
    m_cluster_info_dlg(parent)
{

}

Dialogs::~Dialogs()
{

}

void Dialogs::showSettingsDialog()
{
    if(!m_settings_dlg.isVisible())
        m_settings_dlg.exec();
}

void Dialogs::showTempDialog()
{
    if(!m_temp_editor_dlg.isVisible())
        m_temp_editor_dlg.exec();
}

void Dialogs::showMonthlyRainfallDialog()
{
    if(!m_monthly_rainfall_edit_dlg.isVisible())
        m_monthly_rainfall_edit_dlg.exec();
}

void Dialogs::triggerPointerInfoDialog(bool show)
{
    if(show && !m_pointer_info_dlg.isVisible())
        m_pointer_info_dlg.show();
    else if(!show && m_pointer_info_dlg.isVisible())
        m_pointer_info_dlg.hide();
}

void Dialogs::triggerClusterInfoDialog(bool show)
{
    if(show && !m_cluster_info_dlg.isVisible())
        m_cluster_info_dlg.show();
    else if(!show && m_cluster_info_dlg.isVisible())
        m_cluster_info_dlg.hide();
}
