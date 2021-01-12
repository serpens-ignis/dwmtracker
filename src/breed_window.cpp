/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#include "breed_window.h"
#include "ui_breed_window.h"

breed_window::breed_window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::breed_window)
{
    ui->setupUi(this);
}

breed_window::~breed_window()
{
    delete ui;
}

void breed_window::clear()
{
    ui->name->clear();
    ui->male->setChecked(true);
    return;
}

void breed_window::show_unknown_button(bool show)
{
    ui->unknown->setVisible(show);
    return;
}

QString breed_window::get_name()
{
    return ui->name->text();
}

int breed_window::get_sex()
{
    int sex = ui->sex_buttons->checkedId();
    if(sex == -2)
    {
        sex = male;
    }
    if(sex == -3)
    {
        sex = female;
    }
    if(sex == -4)
    {
        sex = unknown;
    }
    return sex;
}
