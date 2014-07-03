#include <QApplication>
#include <QDesktopWidget>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window w;
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
