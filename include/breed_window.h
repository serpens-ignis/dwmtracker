/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef BREED_WINDOW_H
#define BREED_WINDOW_H

#include "monster.h"

#include <QDialog>

namespace Ui {
class breed_window;
}

class breed_window : public QDialog
{
    Q_OBJECT

public:
    breed_window(QWidget *parent = nullptr);
    ~breed_window();

    void clear();
    void show_unknown_button(bool show);

    QString get_name();
    int get_sex();

private:
    Ui::breed_window *ui;
};

#endif
