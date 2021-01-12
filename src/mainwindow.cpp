/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    add_window = new add_monster_window(nullptr, &monsterdex);
    breed_hatch_window = new breed_window(nullptr);
    unsavedChanges = false;

    //FARM page
    //buttons
    QObject::connect(ui->save_farm_button, SIGNAL(clicked()), this, SLOT(save_farm_onclick()));
    QObject::connect(ui->load_farm_button, SIGNAL(clicked()), this, SLOT(load_farm_onclick()));
    QObject::connect(ui->add_monster_button, SIGNAL(clicked()), this, SLOT(add_monster_onclick()));
    QObject::connect(ui->remove_monster_button, SIGNAL(clicked()), this, SLOT(remove_monster_onclick()));
    QObject::connect(ui->hatch_button, SIGNAL(clicked()), this, SLOT(hatch_onclick()));
    QObject::connect(ui->edit_monster_button, SIGNAL(clicked()), this, SLOT(edit_monster_onclick()));
    QObject::connect(ui->swap_monsters_button, SIGNAL(clicked()), this, SLOT(swap_monsters_onclick()));
    QObject::connect(ui->swap_eggs_button, SIGNAL(clicked()), this, SLOT(swap_eggs_onclick()));
    QObject::connect(ui->sleep_wake_button, SIGNAL(clicked()), this, SLOT(sleep_wake_onclick()));
    QObject::connect(ui->see_in_dex_button, SIGNAL(clicked()), this, SLOT(see_in_dex_onclick()));
    //list
    QObject::connect(ui->monster_list, SIGNAL(itemSelectionChanged()), this, SLOT(monster_list_on_selection_change()));
    load_monster_list();

    //BREED page
    //buttons
    QObject::connect(ui->breed_button, SIGNAL(clicked()), this, SLOT(breed_onclick()));
    QObject::connect(ui->breed_and_hatch_button, SIGNAL(clicked()), this, SLOT(breed_and_hatch_onclick()));
    QObject::connect(ui->see_result_in_dex_button, SIGNAL(clicked()), this, SLOT(see_result_in_dex_onclick()));
    //list
    QObject::connect(ui->match_list, SIGNAL(itemSelectionChanged()), this, SLOT(match_list_on_selection_change()));
    load_match_list();

    //MONSTERDEX page
    QObject::connect(ui->sort_dex, SIGNAL(clicked()), this, SLOT(sort_dex_onclick()));
    //list
    dex_alph = false;
    monsterdex_alph = monsterdex;
    std::sort(std::begin(monsterdex_alph), std::end(monsterdex_alph),
                                [](dex_entry const a, dex_entry const b)->bool
                                {return a.species < b.species;});
    load_dex_list();
    QObject::connect(ui->dex_list, SIGNAL(itemSelectionChanged()), this, SLOT(dex_list_on_selection_change()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//FARM page
//buttons

void MainWindow::save_farm_onclick()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save"), QDir::currentPath() + "/save/farm.sav", tr("Farm Save Files (*.sav)"));
    if(file != "")
    {
        save_farm(file.toStdString());
        unsavedChanges = false;
    }
    return;
}

void MainWindow::load_farm_onclick()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Save"), QDir::currentPath() + "/save", tr("Farm Save Files (*.sav)"));
    if(file != "")
    {
        load_farm(file.toStdString());
        unsavedChanges = false;
    }
    refresh_monster_list();
    refresh_match_list();
    return;
}

void MainWindow::add_monster_onclick()
{
    add_window->setWindowTitle("Add Monster");
    add_window->show_egg_monster_buttons(true);

    std::string *name = new std::string;
    std::string *species = new std::string;
    int *sex = new int;
    int *plus_value = new int;
    int *level = new int;
    int *type = new int;

    add_window->clear();

    if(parse_add_monster_window_input(name, species, sex, plus_value, level, type))
    {
        if(*type == -2)
        {
            if(add_monster(monsters, *name, *species, *sex, *plus_value, *level) == nullptr)
            {
                if(add_monster(monsters_asleep, *name, *species, *sex, *plus_value, *level) == nullptr)
                {
                    QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space for monsters!");
                }
                else
                {
                    unsavedChanges = true;
                }
            }
            else
            {
                unsavedChanges = true;
            }
        }
        else
        {
            if(add_monster(eggs, *species, *species, *sex, *plus_value, *level) == nullptr)
            {
                if(add_monster(eggs_asleep, *species, *species, *sex, *plus_value, *level) == nullptr)
                {
                    QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space for eggs!");
                }
                else
                {
                    unsavedChanges = true;
                }
            }
            else
            {
                unsavedChanges = true;
            }
        }
    }

    refresh_monster_list();
    refresh_match_list();

    delete name;
    delete species;
    delete sex;
    delete plus_value;
    delete level;
    delete type;
    return;
}

void MainWindow::edit_monster_onclick()
{
    add_window->setWindowTitle("Edit Monster");
    add_window->show_egg_monster_buttons(false);

    int row = ui->monster_list->currentRow();
    int select = row/17;
    row = row % 17 - 1;

    std::string *name = new std::string;
    std::string *species = new std::string;
    int *sex = new int;
    int *plus_value = new int;
    int *level = new int;
    int *type = new int;

    switch(select)
    {
    case 0:
        add_window->set_starting_values(monsters[row]->name, monsters[row]->species, monsters[row]->sex,
                                        monsters[row]->plus_value, monsters[row]->level, false);
        break;
    case 1:
        add_window->set_starting_values(monsters_asleep[row]->name, monsters_asleep[row]->species, monsters_asleep[row]->sex,
                                        monsters_asleep[row]->plus_value, monsters_asleep[row]->level, false);
        break;
    case 2:
        add_window->set_starting_values(eggs[row]->name, eggs[row]->species, eggs[row]->sex,
                                        eggs[row]->plus_value, eggs[row]->level, true);
        break;
    case 3:
        add_window->set_starting_values(eggs_asleep[row]->name, eggs_asleep[row]->species, eggs_asleep[row]->sex,
                                        eggs_asleep[row]->plus_value, eggs_asleep[row]->level, true);
        break;
    }

    if(parse_add_monster_window_input(name, species, sex, plus_value, level, type))
    {
        switch(select)
        {
        case 0:
            edit_monster(monsters[row], *name, *species, *sex, *plus_value, *level);
            break;
        case 1:
            edit_monster(monsters_asleep[row], *name, *species, *sex, *plus_value, *level);
            break;
        case 2:
            edit_monster(eggs[row], *name, *species, *sex, *plus_value, *level);
            break;
        case 3:
            edit_monster(eggs_asleep[row], *name, *species, *sex, *plus_value, *level);
            break;
        }
        unsavedChanges = true;
    }
    refresh_monster_list();
    refresh_match_list();

    delete name;
    delete species;
    delete sex;
    delete plus_value;
    delete level;
    delete type;

    return;
}

int MainWindow::parse_add_monster_window_input(std::string *name, std::string *species,
                                               int *sex, int *plus_value, int *level, int *type)
{
    int ok = add_window->exec();
    if(ok)
    {
        *name = add_window->get_name().toStdString();
        *species = add_window->get_species().toStdString();
        *sex = add_window->get_sex();
        *plus_value = add_window->get_plus();
        *level = add_window->get_level();
        *type = add_window->get_type();

        if(*name == "" && *type == -2)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Please insert a name");
            errormsg.exec();
            return 0;
        }
        if(name->size() > 4 && *type == -2)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Names must be up to 4 characters long");
            errormsg.exec();
            return 0;
        }
        if(*species == "")
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Please choose a species");
            errormsg.exec();
            return 0;
        }
        if(*sex == -1)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Please pick a sex");
            errormsg.exec();
            return 0;
        }
        if(*plus_value < 0)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Plus value must be a value equal to or greater than 0");
            errormsg.exec();
            return 0;
        }
        if(*level < 1 || *level > 99)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Level must be a value between 1 and 99");
            errormsg.exec();
            return 0;
        }
    }

    return ok;
}

void MainWindow::remove_monster_onclick()
{
    int row = ui->monster_list->currentRow();
    int select = row/17;
    row = row % 17 - 1;

    switch(select)
    {
    case 0:
        remove_monster(monsters, monsters[row]);
        break;
    case 1:
        remove_monster(monsters_asleep, monsters_asleep[row]);
        break;
    case 2:
        remove_monster(eggs, eggs[row]);
        break;
    case 3:
        remove_monster(eggs_asleep, eggs_asleep[row]);
        break;
    }

    unsavedChanges = true;

    refresh_monster_list();
    refresh_match_list();

    return;
}

void MainWindow::hatch_onclick()
{
    int row = ui->monster_list->currentRow();
    int select = row/17;
    row = row % 17 - 1;

    breed_hatch_window->clear();
    breed_hatch_window->show_unknown_button(false);

    if(breed_hatch_window->exec())
    {
        QString name = breed_hatch_window->get_name();
        int sex = breed_hatch_window->get_sex();

        if(name == "")
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Please insert a name");
            errormsg.exec();
            return;
        }
        if(name.size() > 4)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Names must be up to 4 characters long");
            errormsg.exec();
            return;
        }

        int retcode = 0;

        switch(select)
        {
        case 2:
            retcode = hatch(eggs[row], name.toStdString(), sex);
            break;

        case 3:
            retcode = hatch(eggs_asleep[row], name.toStdString(), sex);
            break;
        }

        if(retcode == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No space to hatch!");
            errormsg.exec();
            return;
        }

        unsavedChanges = true;
        refresh_monster_list();
        refresh_match_list();
    }
    return;
}

void MainWindow::swap_monsters_onclick()
{
    swap_monsters();
    unsavedChanges = true;
    refresh_monster_list();

    return;
}

void MainWindow::swap_eggs_onclick()
{
    swap_eggs();
    unsavedChanges = true;
    refresh_monster_list();

    return;
}

void MainWindow::sleep_wake_onclick()
{
    int row = ui->monster_list->currentRow();
    int select = row/17;
    row = row % 17 - 1;
    switch(select)
    {
    case 0:
        if(move_monster(monsters[row], monsters, monsters_asleep) == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space in monsters!");
            errormsg.exec();
        }
        else
        {
            unsavedChanges = true;
        }
        break;
    case 1:
        if(move_monster(monsters_asleep[row], monsters_asleep, monsters) == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space in monsters asleep!");
            errormsg.exec();
        }
        else
        {
            unsavedChanges = true;
        }
        break;
    case 2:
        if(move_monster(eggs[row], eggs, eggs_asleep) == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space in eggs!");
            errormsg.exec();
        }
        else
        {
            unsavedChanges = true;
        }
        break;
    case 3:
        if(move_monster(eggs_asleep[row], eggs_asleep, eggs) == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No more space in eggs asleep!");
            errormsg.exec();
        }
        else
        {
            unsavedChanges = true;
        }
        break;
    }

    refresh_monster_list();

    return;
}

void MainWindow::see_in_dex_onclick()
{
    int row = ui->monster_list->currentRow();
    int select = row/17;
    row = row % 17 - 1;

    switch(select)
    {
    case 0:
        for(int i = 0; i < 225; i++)
        {
            ui->dex_list->setCurrentRow(i);
            if(ui->dex_list->currentItem()->text() == QString::fromStdString(monsters[row]->species))
            {
                ui->tabWidget->setCurrentIndex(2);
                return;
            }
        }
        break;
    case 1:
        for(int i = 0; i < 225; i++)
        {
            ui->dex_list->setCurrentRow(i);
            if(ui->dex_list->currentItem()->text() == QString::fromStdString(monsters_asleep[row]->species))
            {
                ui->tabWidget->setCurrentIndex(2);
                return;
            }
        }
        break;
    case 2:
        for(int i = 0; i < 225; i++)
        {
            ui->dex_list->setCurrentRow(i);
            if(ui->dex_list->currentItem()->text() == QString::fromStdString(eggs[row]->species))
            {
                ui->tabWidget->setCurrentIndex(2);
                return;
            }
        }
        break;
    case 3:
        for(int i = 0; i < 225; i++)
        {
            ui->dex_list->setCurrentRow(i);
            if(ui->dex_list->currentItem()->text() == QString::fromStdString(eggs_asleep[row]->species))
            {
                ui->tabWidget->setCurrentIndex(2);
                return;
            }
        }
        break;
    }

    return;
}

//list

void MainWindow::monster_list_on_selection_change()
{
    int row = ui->monster_list->currentRow();

    int select = row/17;
    row %= 17;

    QPixmap pixmap;
    bool is_null = row == 0;
    row -= 1;

    if(!is_null)
    {
        switch(select)
        {
        case 0:
            if(monsters[row] == nullptr)
            {
                is_null = true;
                break;
            }

            pixmap.load("img/" + QString::fromStdString(monsters[row]->species) + ".png");
            ui->name->setText(QString::fromStdString(monsters[row]->name));
            ui->species->setText(QString::fromStdString(monsters[row]->species));
            ui->family->setText(get_family_string(monsters[row]->family));
            ui->sex->setText(get_sex_string(monsters[row]));
            ui->level->setText(QString::number(monsters[row]->level));
            ui->plus->setText(QString::number(monsters[row]->plus_value));
            break;
        case 1:
            if(monsters_asleep[row] == nullptr)
            {
                is_null = true;
                break;
            }

            pixmap.load("img/" + QString::fromStdString(monsters_asleep[row]->species) + ".png");
            ui->name->setText(QString::fromStdString(monsters_asleep[row]->name));
            ui->species->setText(QString::fromStdString(monsters_asleep[row]->species));
            ui->family->setText(get_family_string(monsters_asleep[row]->family));
            ui->sex->setText(get_sex_string(monsters_asleep[row]));
            ui->level->setText(QString::number(monsters_asleep[row]->level));
            ui->plus->setText(QString::number(monsters_asleep[row]->plus_value));
            break;
        case 2:
            if(eggs[row] == nullptr)
            {
                is_null = true;
                break;
            }

            pixmap.load("img/" + QString::fromStdString(eggs[row]->species) + ".png");
            ui->name->setText(QString::fromStdString(eggs[row]->name));
            ui->species->setText(QString::fromStdString(eggs[row]->species));
            ui->family->setText(get_family_string(eggs[row]->family));
            ui->sex->setText(get_sex_string(eggs[row]));
            ui->level->setText(QString::number(eggs[row]->level));
            ui->plus->setText(QString::number(eggs[row]->plus_value));
            break;
        case 3:
            if(eggs_asleep[row] == nullptr)
            {
                is_null = true;
                break;
            }

            pixmap.load("img/" + QString::fromStdString(eggs_asleep[row]->species) + ".png");
            ui->name->setText(QString::fromStdString(eggs_asleep[row]->name));
            ui->species->setText(QString::fromStdString(eggs_asleep[row]->species));
            ui->family->setText(get_family_string(eggs_asleep[row]->family));
            ui->sex->setText(get_sex_string(eggs_asleep[row]));
            ui->level->setText(QString::number(eggs_asleep[row]->level));
            ui->plus->setText(QString::number(eggs_asleep[row]->plus_value));
            break;
        }
    }

    ui->sleep_wake_button->setEnabled(true);
    ui->see_in_dex_button->setEnabled(true);
    ui->remove_monster_button->setEnabled(true);
    ui->edit_monster_button->setEnabled(true);
    ui->hatch_button->setEnabled(true);

    if(select < 2)
    {
        ui->hatch_button->setEnabled(false);
    }
    if(is_null)
    {
        pixmap.load("null");
        ui->name->setText("-");
        ui->species->setText("-");
        ui->family->setText("-");
        ui->sex->setText("-");
        ui->level->setText("-");
        ui->plus->setText("-");

        ui->sleep_wake_button->setEnabled(false);
        ui->see_in_dex_button->setEnabled(false);
        ui->remove_monster_button->setEnabled(false);
        ui->edit_monster_button->setEnabled(false);
        ui->hatch_button->setEnabled(false);
    }

    ui->sprite->setPixmap(pixmap);
    return;
}

void MainWindow::load_monster_list()
{
    ui->monster_list->addItem("----------MONSTERS-----------");
    for(int i = 0; i < 16; i++)
    {
        if(monsters[i] != nullptr)
        {
            ui->monster_list->addItem(QString::fromStdString(monsters[i]->name));
        }
        else
        {
            ui->monster_list->addItem("-");
        }
    }

    ui->monster_list->addItem("-------MONSTERS ASLEEP-------");
    for(int i = 0; i < 16; i++)
    {
        if(monsters_asleep[i] != nullptr)
        {
            ui->monster_list->addItem(QString::fromStdString(monsters_asleep[i]->name));
        }
        else
        {
            ui->monster_list->addItem("-");
        }
    }

    ui->monster_list->addItem("------------EGGS-------------");
    for(int i = 0; i < 16; i++)
    {
        if(eggs[i] != nullptr)
        {
            ui->monster_list->addItem(QString::fromStdString(eggs[i]->name));
        }
        else
        {
            ui->monster_list->addItem("-");
        }
    }

    ui->monster_list->addItem("---------EGGS ASLEEP---------");
    for(int i = 0; i < 16; i++)
    {
        if(eggs_asleep[i] != nullptr)
        {
            ui->monster_list->addItem(QString::fromStdString(eggs_asleep[i]->name));
        }
        else
        {
            ui->monster_list->addItem("-");
        }
    }

    return;
}

void MainWindow::refresh_monster_list()
{
    ui->monster_list->clear();
    load_monster_list();
    ui->monster_list->setCurrentRow(0);
    return;
}

QString MainWindow::get_family_string(int f)
{
    QString family = "";

    switch(f)
    {
    case slime:
        family = "Slime";
        break;
    case dragon:
        family = "Dragon";
        break;
    case beast:
        family = "Beast";
        break;
    case bird:
        family = "Bird";
        break;
    case plant:
        family = "Plant";
        break;
    case bug:
        family = "Bug";
        break;
    case devil:
        family = "Devil";
        break;
    case zombie:
        family = "Zombie";
        break;
    case material:
        family = "Material";
        break;
    case boss:
        family = "Boss";
        break;
    }

    return family;
}

QString MainWindow::get_sex_string(monster *m)
{
    QString sex = "";

    switch(m->sex)
    {
    case male:
        sex = "Male";
        break;
    case female:
        sex = "Female";
        break;
    case unknown:
        sex = "Unknown";
        break;
    }

    return sex;
}

bool MainWindow::unsaved_changes_msg()
{
    QMessageBox msg;
    msg.setText("There are unsaved changes.");
    msg.setInformativeText("Would you really like to quit?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg.exec() == 16384)
    {
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool closing = true;
    if(unsavedChanges)
    {
        closing = unsaved_changes_msg();
    }
    if(closing)
    {
        QWidget::closeEvent(event);
    }
    else
    {
        event->ignore();
    }
    return;
}

//BREED page
//buttons

void MainWindow::breed_onclick()
{
    int row = ui->match_list->currentRow();
    if(breed(matches[row]) == nullptr)
    {
        QMessageBox errormsg(QMessageBox::Warning, "Error", "No space to breed!");
        errormsg.exec();
        return;
    }

    unsavedChanges = true;
    refresh_monster_list();
    refresh_match_list();

    return;
}

void MainWindow::breed_and_hatch_onclick()
{
    int row = ui->match_list->currentRow();

    breed_hatch_window->clear();
    breed_hatch_window->show_unknown_button(false);

    if(breed_hatch_window->exec())
    {
        QString name = breed_hatch_window->get_name();
        int sex = breed_hatch_window->get_sex();

        if(name == "")
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Please insert a name");
            errormsg.exec();
            return;
        }
        if(name.size() > 4)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "Names must be up to 4 characters long");
            errormsg.exec();
            return;
        }

        int retcode = breed_and_hatch(matches[row], name.toStdString(), sex);

        if(retcode == gFULL)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No space to breed!");
            errormsg.exec();
            return;
        }

        unsavedChanges = true;

        if(retcode == gFULL_HATCH)
        {
            QMessageBox errormsg(QMessageBox::Warning, "Error", "No space to hatch!");
            errormsg.exec();
            return;
        }

        refresh_monster_list();
        refresh_match_list();
    }
    return;
}

void MainWindow::see_result_in_dex_onclick()
{
    int row = ui->match_list->currentRow();

    for(int i = 0; i < 225; i++)
    {
        ui->dex_list->setCurrentRow(i);
        if(ui->dex_list->currentItem()->text() == QString::fromStdString(matches[row]->result))
        {
            ui->tabWidget->setCurrentIndex(2);
            return;
        }
    }

    return;
}

//list

void MainWindow::match_list_on_selection_change()
{
    int row = ui->match_list->currentRow();

    QPixmap pixmap;

    pixmap.load("img/" + QString::fromStdString(matches[row]->pedigree->species) + ".png");
    ui->pedigree_sprite->setPixmap(pixmap);
    ui->pedigree_name->setText(QString::fromStdString(matches[row]->pedigree->name));
    ui->pedigree_species->setText(QString::fromStdString(matches[row]->pedigree->species));
    ui->pedigree_family->setText(get_family_string(matches[row]->pedigree->family));
    ui->pedigree_sex->setText(get_sex_string(matches[row]->pedigree));
    ui->pedigree_level->setText(QString::number(matches[row]->pedigree->level));
    ui->pedigree_plus->setText(QString::number(matches[row]->pedigree->plus_value));

    pixmap.load("img/" + QString::fromStdString(matches[row]->mate->species) + ".png");
    ui->mate_sprite->setPixmap(pixmap);
    ui->mate_name->setText(QString::fromStdString(matches[row]->mate->name));
    ui->mate_species->setText(QString::fromStdString(matches[row]->mate->species));
    ui->mate_family->setText(get_family_string(matches[row]->mate->family));
    ui->mate_sex->setText(get_sex_string(matches[row]->mate));
    ui->mate_level->setText(QString::number(matches[row]->mate->level));
    ui->mate_plus->setText(QString::number(matches[row]->mate->plus_value));

    pixmap.load("img/" + QString::fromStdString(matches[row]->result) + ".png");
    ui->result_sprite->setPixmap(pixmap);
    ui->result_species->setText(QString::fromStdString(matches[row]->result));
    ui->result_family->setText(get_family_string(find_monster_family(matches[row]->result)));
    ui->result_plus->setText(QString::number(calculate_plus_value(matches[row])));

    return;
}

void MainWindow::load_match_list()
{
    calculate_matches();
    for(int i = 0; i < 256 && matches[i] != nullptr; i++)
    {
        ui->match_list->addItem(QString::fromStdString(matches[i]->result) + "+" +
                                QString::number(calculate_plus_value(matches[i])) + " = " +
                                QString::fromStdString(matches[i]->pedigree->name) + " x " +
                                QString::fromStdString(matches[i]->mate->name));
    }

    return;
}

void MainWindow::refresh_match_list()
{
    //signals need to be blocked in order to avoid a crash
    //this avoids an infamous bug that the clear method introduces
    ui->match_list->blockSignals(true);
    ui->match_list->clear();
    ui->match_list->blockSignals(false);
    load_match_list();
    ui->match_list->setCurrentRow(0);

    if(matches[0] == nullptr)
    {
        QPixmap pixmap("NULL");

        ui->pedigree_sprite->setPixmap(pixmap);
        ui->pedigree_name->setText("-");
        ui->pedigree_species->setText("-");
        ui->pedigree_family->setText("-");
        ui->pedigree_sex->setText("-");
        ui->pedigree_level->setText("-");
        ui->pedigree_plus->setText("-");

        ui->mate_sprite->setPixmap(pixmap);
        ui->mate_name->setText("-");
        ui->mate_species->setText("-");
        ui->mate_family->setText("-");
        ui->mate_sex->setText("-");
        ui->mate_level->setText("-");
        ui->mate_plus->setText("-");

        ui->result_sprite->setPixmap(pixmap);
        ui->result_species->setText("-");
        ui->result_family->setText("-");
        ui->result_plus->setText("-");
    }

    return;
}

//MONSTERDEX page
//buttons
void MainWindow::sort_dex_onclick()
{
    dex_alph = !dex_alph;
    if(dex_alph)
    {
        ui->sort_dex->setText("Sort By Family");
    }
    else
    {
        ui->sort_dex->setText("Sort Alphabetically");
    }
    load_dex_list();

    return;
}

//list
void MainWindow::load_dex_list()
{
    ui->dex_list->clear();
    if(dex_alph)
    {
        for(int i = 0; i < 215; i++)
        {
            ui->dex_list->addItem(QString::fromStdString(monsterdex_alph[i].species));
        }

    }
    else
    {
        for(int i = 0; i < 215; i++)
        {
            if(i == 0)
            {
                ui->dex_list->addItem("------------SLIME------------");
            }
            if(i == 20)
            {
                ui->dex_list->addItem("------------DRAGON-----------");
            }
            if(i == 45)
            {
                ui->dex_list->addItem("------------BEAST------------");
            }
            if(i == 70)
            {
                ui->dex_list->addItem("------------BIRD-------------");
            }
            if(i == 90)
            {
                ui->dex_list->addItem("------------PLANT------------");
            }
            if(i == 110)
            {
                ui->dex_list->addItem("-------------BUG-------------");
            }
            if(i == 130)
            {
                ui->dex_list->addItem("------------DEVIL------------");
            }
            if(i == 155)
            {
                ui->dex_list->addItem("------------ZOMBIE-----------");
            }
            if(i == 175)
            {
                ui->dex_list->addItem("----------MATERIAL-----------");
            }
            if(i == 200)
            {
                ui->dex_list->addItem("------------BOSS-------------");
            }
            ui->dex_list->addItem(QString::fromStdString(monsterdex[i].species));
        }
    }
    return;
}

void MainWindow::dex_list_on_selection_change()
{
    int row = ui->dex_list->currentRow();

    QPixmap pixmap;

    if(dex_alph)
    {
        pixmap.load("img/" + QString::fromStdString(monsterdex_alph[row].species) + ".png");
        ui->dex_species->setText(QString::fromStdString(monsterdex_alph[row].species));
        ui->skill1->setText(QString::fromStdString(monsterdex_alph[row].skills[0]));
        ui->skill2->setText(QString::fromStdString(monsterdex_alph[row].skills[1]));
        ui->skill3->setText(QString::fromStdString(monsterdex_alph[row].skills[2]));
        ui->note->setText(QString::fromStdString(monsterdex_alph[row].note));
        ui->dex_family->setText(get_family_string(monsterdex_alph[row].family));
        ui->dex_max_level->setText(QString::number(monsterdex_alph[row].max_level));
        if(monsterdex_alph[row].xp_speed < 8)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex_alph[row].xp_speed) + " (Fast)");
        }
        else if(monsterdex_alph[row].xp_speed < 16)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex_alph[row].xp_speed) + " (Normal)");
        }
        else if(monsterdex_alph[row].xp_speed < 24)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex_alph[row].xp_speed) + " (Slow)");
        }
        else if(monsterdex_alph[row].xp_speed < 32)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex_alph[row].xp_speed) + " (V.Slow)");
        }
        switch(monsterdex_alph[row].sex_probability)
        {
        case always_male:
            ui->dex_sex_probability->setText("Always Male");
            break;
        case high_chance_male:
            ui->dex_sex_probability->setText("High Male");
            break;
        case even_chance:
            ui->dex_sex_probability->setText("Even");
            break;
        case high_chance_female:
            ui->dex_sex_probability->setText("High Female");
            break;
        }
        if(monsterdex_alph[row].flying)
        {
            ui->dex_flying->setText("Yes");
        }
        else
        {
            ui->dex_flying->setText("No");
        }
        if(monsterdex_alph[row].metal_slime)
        {
            ui->dex_metal_slime->setText("Yes");
        }
        else
        {
            ui->dex_metal_slime->setText("No");
        }
        ui->dex_hp_growth->setText(QString::number(monsterdex_alph[row].hp_growth));
        ui->dex_mp_growth->setText(QString::number(monsterdex_alph[row].mp_growth));
        ui->dex_atk_growth->setText(QString::number(monsterdex_alph[row].atk_growth));
        ui->dex_def_growth->setText(QString::number(monsterdex_alph[row].def_growth));
        ui->dex_agl_growth->setText(QString::number(monsterdex_alph[row].agl_growth));
        ui->dex_int_growth->setText(QString::number(monsterdex_alph[row].int_growth));
        ui->dex_frizz_res->setText(QString::number(monsterdex_alph[row].frizz_resistance));
        ui->dex_sizz_res->setText(QString::number(monsterdex_alph[row].sizz_resistance));
        ui->dex_bang_res->setText(QString::number(monsterdex_alph[row].bang_resistance));
        ui->dex_woosh_res->setText(QString::number(monsterdex_alph[row].woosh_resistance));
        ui->dex_zap_res->setText(QString::number(monsterdex_alph[row].zap_resistance));
        ui->dex_crack_res->setText(QString::number(monsterdex_alph[row].crack_resistance));
        ui->dex_dazzle_res->setText(QString::number(monsterdex_alph[row].dazzle_resistance));
        ui->dex_snooze_res->setText(QString::number(monsterdex_alph[row].snooze_resistance));
        ui->dex_whack_res->setText(QString::number(monsterdex_alph[row].whack_resistance));
        ui->dex_drain_magic_res->setText(QString::number(monsterdex_alph[row].drain_magic_resistance));
        ui->dex_fizzle_res->setText(QString::number(monsterdex_alph[row].fizzle_resistance));
        ui->dex_fuddle_res->setText(QString::number(monsterdex_alph[row].fuddle_resistance));
        ui->dex_sap_res->setText(QString::number(monsterdex_alph[row].sap_resistance));
        ui->dex_decelerate_res->setText(QString::number(monsterdex_alph[row].decelerate_resistance));
        ui->dex_kamikazee_res->setText(QString::number(monsterdex_alph[row].kamikazee_resistance));
        ui->dex_megamagic_res->setText(QString::number(monsterdex_alph[row].megamagic_resistance));
        ui->dex_fire_breath_res->setText(QString::number(monsterdex_alph[row].fire_breath_resistance));
        ui->dex_ice_breath_res->setText(QString::number(monsterdex_alph[row].ice_breath_resistance));
        ui->dex_poison_res->setText(QString::number(monsterdex_alph[row].poison_resistance));
        ui->dex_paralysis_res->setText(QString::number(monsterdex_alph[row].paralysis_resistance));
        ui->dex_curse_res->setText(QString::number(monsterdex_alph[row].curse_resistance));
        load_combos_list(row);
        load_location_list(row);
    }
    else if(row == 0 || row == 21 || row == 47 || row == 73 || row == 94 ||
           row == 115 || row == 136 || row == 162 || row == 183 || row == 209)
    {
        pixmap.load("NULL");
        ui->dex_species->setText("-");
        ui->skill1->setText("-");
        ui->skill2->setText("-");
        ui->skill3->setText("-");
        ui->note->setText("-");
        ui->dex_family->setText("-");
        ui->dex_max_level->setText("-");
        ui->dex_xp_speed->setText("-");
        ui->dex_sex_probability->setText("-");
        ui->dex_flying->setText("-");
        ui->dex_flying->setText("-");
        ui->dex_metal_slime->setText("-");
        ui->dex_metal_slime->setText("-");
        ui->dex_hp_growth->setText("-");
        ui->dex_mp_growth->setText("-");
        ui->dex_atk_growth->setText("-");
        ui->dex_def_growth->setText("-");
        ui->dex_agl_growth->setText("-");
        ui->dex_int_growth->setText("-");
        ui->dex_frizz_res->setText("-");
        ui->dex_sizz_res->setText("-");
        ui->dex_bang_res->setText("-");
        ui->dex_woosh_res->setText("-");
        ui->dex_zap_res->setText("-");
        ui->dex_crack_res->setText("-");
        ui->dex_dazzle_res->setText("-");
        ui->dex_snooze_res->setText("-");
        ui->dex_whack_res->setText("-");
        ui->dex_drain_magic_res->setText("-");
        ui->dex_fizzle_res->setText("-");
        ui->dex_fuddle_res->setText("-");
        ui->dex_sap_res->setText("-");
        ui->dex_decelerate_res->setText("-");
        ui->dex_kamikazee_res->setText("-");
        ui->dex_megamagic_res->setText("-");
        ui->dex_fire_breath_res->setText("-");
        ui->dex_ice_breath_res->setText("-");
        ui->dex_poison_res->setText("-");
        ui->dex_paralysis_res->setText("-");
        ui->dex_curse_res->setText("-");
        load_combos_list(-1);
        load_location_list(-1);
    }
    else
    {

        if(row > 209)
        {
            row--;
        }
        if(row > 183)
        {
            row--;
        }
        if(row > 162)
        {
            row--;
        }
        if(row > 136)
        {
            row--;
        }
        if(row > 115)
        {
            row--;
        }
        if(row > 94)
        {
            row--;
        }
        if(row > 73)
        {
            row--;
        }
        if(row > 47)
        {
            row--;
        }
        if(row > 21)
        {
            row--;
        }
        if(row > 0)
        {
            row--;
        }

        pixmap.load("img/" + QString::fromStdString(monsterdex[row].species) + ".png");
        ui->dex_species->setText(QString::fromStdString(monsterdex[row].species));
        ui->skill1->setText(QString::fromStdString(monsterdex[row].skills[0]));
        ui->skill2->setText(QString::fromStdString(monsterdex[row].skills[1]));
        ui->skill3->setText(QString::fromStdString(monsterdex[row].skills[2]));
        ui->note->setText(QString::fromStdString(monsterdex[row].note));
        ui->dex_family->setText(get_family_string(monsterdex[row].family));
        ui->dex_max_level->setText(QString::number(monsterdex[row].max_level));
        if(monsterdex[row].xp_speed < 8)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex[row].xp_speed) + " (Fast)");
        }
        else if(monsterdex[row].xp_speed < 16)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex[row].xp_speed) + " (Normal)");
        }
        else if(monsterdex[row].xp_speed < 24)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex[row].xp_speed) + " (Slow)");
        }
        else if(monsterdex[row].xp_speed < 32)
        {
            ui->dex_xp_speed->setText(QString::number(monsterdex[row].xp_speed) + " (V.Slow)");
        }
        switch(monsterdex[row].sex_probability)
        {
        case always_male:
            ui->dex_sex_probability->setText("Always Male");
            break;
        case high_chance_male:
            ui->dex_sex_probability->setText("High Male");
            break;
        case even_chance:
            ui->dex_sex_probability->setText("Even");
            break;
        case high_chance_female:
            ui->dex_sex_probability->setText("High Female");
            break;
        }
        if(monsterdex[row].flying)
        {
            ui->dex_flying->setText("Yes");
        }
        else
        {
            ui->dex_flying->setText("No");
        }
        if(monsterdex[row].metal_slime)
        {
            ui->dex_metal_slime->setText("Yes");
        }
        else
        {
            ui->dex_metal_slime->setText("No");
        }
        ui->dex_hp_growth->setText(QString::number(monsterdex[row].hp_growth));
        ui->dex_mp_growth->setText(QString::number(monsterdex[row].mp_growth));
        ui->dex_atk_growth->setText(QString::number(monsterdex[row].atk_growth));
        ui->dex_def_growth->setText(QString::number(monsterdex[row].def_growth));
        ui->dex_agl_growth->setText(QString::number(monsterdex[row].agl_growth));
        ui->dex_int_growth->setText(QString::number(monsterdex[row].int_growth));
        ui->dex_frizz_res->setText(QString::number(monsterdex[row].frizz_resistance));
        ui->dex_sizz_res->setText(QString::number(monsterdex[row].sizz_resistance));
        ui->dex_bang_res->setText(QString::number(monsterdex[row].bang_resistance));
        ui->dex_woosh_res->setText(QString::number(monsterdex[row].woosh_resistance));
        ui->dex_zap_res->setText(QString::number(monsterdex[row].zap_resistance));
        ui->dex_crack_res->setText(QString::number(monsterdex[row].crack_resistance));
        ui->dex_dazzle_res->setText(QString::number(monsterdex[row].dazzle_resistance));
        ui->dex_snooze_res->setText(QString::number(monsterdex[row].snooze_resistance));
        ui->dex_whack_res->setText(QString::number(monsterdex[row].whack_resistance));
        ui->dex_drain_magic_res->setText(QString::number(monsterdex[row].drain_magic_resistance));
        ui->dex_fizzle_res->setText(QString::number(monsterdex[row].fizzle_resistance));
        ui->dex_fuddle_res->setText(QString::number(monsterdex[row].fuddle_resistance));
        ui->dex_sap_res->setText(QString::number(monsterdex[row].sap_resistance));
        ui->dex_decelerate_res->setText(QString::number(monsterdex[row].decelerate_resistance));
        ui->dex_kamikazee_res->setText(QString::number(monsterdex[row].kamikazee_resistance));
        ui->dex_megamagic_res->setText(QString::number(monsterdex[row].megamagic_resistance));
        ui->dex_fire_breath_res->setText(QString::number(monsterdex[row].fire_breath_resistance));
        ui->dex_ice_breath_res->setText(QString::number(monsterdex[row].ice_breath_resistance));
        ui->dex_poison_res->setText(QString::number(monsterdex[row].poison_resistance));
        ui->dex_paralysis_res->setText(QString::number(monsterdex[row].paralysis_resistance));
        ui->dex_curse_res->setText(QString::number(monsterdex[row].curse_resistance));
        load_combos_list(row);
        load_location_list(row);
    }

    ui->dex_sprite->setPixmap(pixmap);

    return;
}

void MainWindow::load_combos_list(int row)
{
    ui->combos_list->clear();

    if(row != -1)
    {
        if(dex_alph)
        {
            for(auto i = combos[0].begin(); i != combos[0].end(); i++)
            {
                if(i->result == monsterdex_alph[row].species)
                {
                    QString combostr = "[" + get_family_string(i->pedigree_family).toUpper() + " ] " +
                                       " x [" + get_family_string(i->mate_family).toUpper() + "] ";
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[1].begin(); i != combos[1].end(); i++)
            {
                if(i->result == monsterdex_alph[row].species)
                {
                    QString combostr = "[" + get_family_string(i->pedigree_family).toUpper() + "] " +
                                       " x " + QString::fromStdString(i->mate_species);
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[2].begin(); i != combos[2].end(); i++)
            {
                if(i->result == monsterdex_alph[row].species)
                {
                    QString combostr = QString::fromStdString(i->pedigree_species) +
                                       " x [" + get_family_string(i->mate_family).toUpper() + "] ";
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[3].begin(); i != combos[3].end(); i++)
            {
                if(i->result == monsterdex_alph[row].species)
                {
                    QString combostr = QString::fromStdString(i->pedigree_species) +
                                       " x " + QString::fromStdString(i->mate_species);
                    ui->combos_list->addItem(combostr);
                }
            }
        }
        else
        {
            for(auto i = combos[0].begin(); i != combos[0].end(); i++)
            {
                if(i->result == monsterdex[row].species)
                {
                    QString combostr = "[" + get_family_string(i->pedigree_family).toUpper() + "] " +
                                       " x [" + get_family_string(i->mate_family).toUpper() + "] ";
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[1].begin(); i != combos[1].end(); i++)
            {
                if(i->result == monsterdex[row].species)
                {
                    QString combostr = "[" + get_family_string(i->pedigree_family).toUpper() + "] " +
                                       " x " + QString::fromStdString(i->mate_species);
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[2].begin(); i != combos[2].end(); i++)
            {
                if(i->result == monsterdex[row].species)
                {
                    QString combostr = QString::fromStdString(i->pedigree_species) +
                                       " x [" + get_family_string(i->mate_family).toUpper() + "] ";
                    ui->combos_list->addItem(combostr);
                }
            }
            for(auto i = combos[3].begin(); i != combos[3].end(); i++)
            {
                if(i->result == monsterdex[row].species)
                {
                    QString combostr = QString::fromStdString(i->pedigree_species) +
                                       " x " + QString::fromStdString(i->mate_species);
                    ui->combos_list->addItem(combostr);
                }
            }
        }
    }

    return;
}

void MainWindow::load_location_list(int row)
{
    ui->location_list->clear();
    if(row != -1)
    {
       if(dex_alph)
       {
           for(auto i = monsterdex_alph[row].locations.begin(); i != monsterdex_alph[row].locations.end(); i++)
           {
               ui->location_list->addItem(QString::fromStdString(*i));
           }
       }
       else
       {
           for(auto i = monsterdex[row].locations.begin(); i != monsterdex[row].locations.end(); i++)
           {
               ui->location_list->addItem(QString::fromStdString(*i));
           }
       }
    }
    return;
}
