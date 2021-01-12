/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
