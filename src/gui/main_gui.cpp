#include <QApplication>
#include "qt_mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ai_slop::QtMainWindow window;
    window.show();
    return app.exec();
} 