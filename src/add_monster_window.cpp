/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#include "add_monster_window.h"
#include "ui_add_monster_window.h"

add_monster_window::add_monster_window(QWidget *parent, std::array<dex_entry, 215> *monsterdex) :
    QDialog(parent),
    ui(new Ui::add_monster_window)
{
    ui->setupUi(this);
    QObject::connect(ui->egg_choice, SIGNAL(toggled(bool)), this, SLOT(check_egg_button(bool)));

    std::string sorted_species[215];
    for(int i = 0; i < 215; i++)
    {
        sorted_species[i] = monsterdex->at(i).species;
    }

    int size_names = sizeof(sorted_species)/sizeof(sorted_species)[0];
    sort(sorted_species, sorted_species + size_names);

    for(int i = 0; i < 215; i++)
    {
        ui->species->addItem(QString::fromStdString(sorted_species[i]));
    }
}

add_monster_window::~add_monster_window()
{
    delete ui;
}

void add_monster_window::clear()
{
    ui->name->clear();
    ui->species->setCurrentIndex(-1);
    ui->level->clear();

    ui->monster_choice->setChecked(true);

    ui->sex_buttons->setExclusive(false);
    ui->male->setChecked(false);
    ui->female->setChecked(false);
    ui->unknown->setChecked(false);
    ui->sex_buttons->setExclusive(true);

    ui->plus_value->clear();
    return;
}

void add_monster_window::set_starting_values(std::string name, std::string species, int sex, int plus_value, int level, bool egg)
{
    switch(sex)
    {
    case male:
        ui->male->setChecked(true);
        break;
    case female:
        ui->female->setChecked(true);
        break;
    case unknown:
        ui->unknown->setChecked(true);
        break;
    }

    if(egg)
    {
        ui->egg_choice->setChecked(true);
    }
    else
    {
        ui->monster_choice->setChecked(true);
    }

    ui->name->setText(QString::fromStdString(name));
    ui->species->setCurrentText(QString::fromStdString(species));
    ui->plus_value->setText(QString::number(plus_value));
    ui->level->setText(QString::number(level));

    return;
}

QString add_monster_window::get_name()
{
    return ui->name->text();
}

QString add_monster_window::get_species()
{
    return ui->species->currentText();
}

int add_monster_window::get_sex()
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

int add_monster_window::get_level()
{
    if(!ui->level->isEnabled())
    {
        return 1;
    }
    return ui->level->text().toInt();
}

int add_monster_window::get_plus()
{
    QString plus_str = ui->plus_value->text();
    QRegExp re("\\d*");
    if(!re.exactMatch(plus_str))
        return -1;
    return plus_str.toInt();
}

int add_monster_window::get_type()
{
    return ui->type_buttons->checkedId();
}

void add_monster_window::show_egg_monster_buttons(bool show)
{
    ui->monster_choice->setVisible(show);
    ui->egg_choice->setVisible(show);
    return;
}

void add_monster_window::check_egg_button(bool checked)
{
    ui->unknown->setEnabled(checked);
    ui->name->setEnabled(!checked);
    ui->level->setEnabled(!checked);

    return;
}
