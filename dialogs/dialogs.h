#ifndef DIALOGS_H
#define DIALOGS_H

#include "settings_dlg.h"
#include "rainfall_edit_dlg.h"
#include "pointer_info_dlg.h"
#include "temp_edit_dlg.h"
#include "monthly_rainfall_edit_dlg.h"
#include "cluster_info_dialog.h"
#include <QObject>

class Dialogs : public QObject{
Q_OBJECT
public:
    Dialogs(QWidget * parent);
    ~Dialogs();

    SettingsDialog m_settings_dlg;
    TemperatureEditDialog m_temp_editor_dlg;
    PointerInformationDialog m_pointer_info_dlg;
    MonthlyRainfallEditDialog m_monthly_rainfall_edit_dlg;
    ClusterInfoDialog m_cluster_info_dlg;

public slots:
    void showSettingsDialog();
    void showTempDialog();
    void showMonthlyRainfallDialog();

    void triggerPointerInfoDialog(bool show);
    void triggerClusterInfoDialog(bool show);
};

#endif // DIALOGS_H
