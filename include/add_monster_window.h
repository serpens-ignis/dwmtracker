/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef ADD_MONSTER_WINDOW_H
#define ADD_MONSTER_WINDOW_H

#include "monster.h"

#include <QDialog>

namespace Ui {
class add_monster_window;
}

class add_monster_window : public QDialog
{
    Q_OBJECT

public:
    add_monster_window(QWidget *parent = nullptr, std::array<dex_entry, 215> *monsterdex = nullptr);
    ~add_monster_window();

    void clear();
    void set_starting_values(std::string name, std::string species, int sex, int plus_value, int level, bool egg);
    void show_egg_monster_buttons(bool show);

    QString get_name();
    QString get_species();
    int get_sex();
    int get_level();
    int get_plus();
    int get_type();

public slots:
    void check_egg_button(bool checked);

private:
    Ui::add_monster_window *ui;
};

#endif
