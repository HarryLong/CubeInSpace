#include <QApplication>
#include <QDesktopWidget>

#include "window.h"
#include <QSurfaceFormat>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("UCT");
    QCoreApplication::setOrganizationDomain("UCT.co.za");
    QCoreApplication::setApplicationName("HarrysMasters");

    QLocale::setDefault(QLocale(QLocale::Language::English, QLocale::Country::UnitedKingdom));

    QSurfaceFormat format;
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    // To run on laptop
//    QGLFormat glFormat;
//    glFormat.setVersion( 4, 0 );
//    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
//    glFormat.setSampleBuffers( true );
    MainWindow w;
    w.resize(w.sizeHint());

//    int desktopArea = QApplication::desktop()->width() *
//                     QApplication::desktop()->height();
//    int widgetArea = w.width() * w.height();
//    if (((float)widgetArea / (float)desktopArea) < 0.75f)
//        w.show();
//    else
    w.showMaximized();

    return app.exec();
}
