/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tracker.h"
#include "add_monster_window.h"
#include "breed_window.h"

#include <algorithm>

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QAbstractItemView>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//error codes


class MainWindow : public QMainWindow, public tracker
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString get_family_string(int f);
    QString get_sex_string(monster *m);
    int parse_add_monster_window_input(std::string *name, std::string *species,
                                       int *sex, int *plus_value, int *level, int *type);
    void load_monster_list();
    void refresh_monster_list();
    void load_match_list();
    void refresh_match_list();
    void load_dex_list();
    bool unsaved_changes_msg();
    void closeEvent(QCloseEvent *event);

public slots:
    //FARM page
    //buttons
    void save_farm_onclick();
    void load_farm_onclick();
    void add_monster_onclick();
    void edit_monster_onclick();
    void remove_monster_onclick();
    void hatch_onclick();
    void swap_monsters_onclick();
    void swap_eggs_onclick();
    void sleep_wake_onclick();
    void see_in_dex_onclick();

    //list
    void monster_list_on_selection_change();

    //BREED page
    //buttons
    void breed_onclick();
    void breed_and_hatch_onclick();
    void see_result_in_dex_onclick();

    //list
    void match_list_on_selection_change();

    //MONSTERDEX page
    void sort_dex_onclick();
    //list
    void dex_list_on_selection_change();
    void load_combos_list(int row);
    void load_location_list(int row);

private:
    Ui::MainWindow *ui;
    add_monster_window *add_window;
    breed_window *breed_hatch_window;
    bool unsavedChanges;
    bool dex_alph;
    std::array<dex_entry, 215> monsterdex_alph;
};
#endif
