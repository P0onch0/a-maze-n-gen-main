
#include "gui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    QFont globalFont("Centaur", 14);
    window.setFont(globalFont);

    window.show();
    return app.exec();
}
