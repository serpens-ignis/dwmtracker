/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef TRACKER_H
#define TRACKER_H

#include "breeding.h"

#include <list>

const int gFULL = -1;
const int gFULL_HATCH = -2;

class tracker
{
protected:
    monster *monsters[16];
	monster *monsters_asleep[16];
	monster *eggs[16];
	monster *eggs_asleep[16];

    std::array<match*, 256> matches;

	std::array<dex_entry, 215> monsterdex;
	std::array<std::list<combo>, 4> combos;

public:
	tracker();
	~tracker();
	void load_farm(std::string filename);
	void save_farm(std::string filename);
	void load_dex();
	void load_combos();

	int first_free_key(monster **m_array);
    int find_monster_family(std::string species);
    int calculate_plus_value(match *m);
	monster *add_monster(monster **m_array,std::string name, std::string species,
                         int sex, int plus_value, int level);
	void remove_monster(monster **m_array, monster *m);
    void edit_monster(monster *m, std::string name, std::string species,
                      int sex, int plus_value, int level);
	int move_monster(monster *m, monster **source, monster **destination);
	void swap_monsters();
	void swap_eggs();

	match *create_match(monster *pedigree, monster *mate);
	void calculate_matches();
	monster *breed(match *m);
	int hatch(monster *egg, std::string name, int sex);
	int breed_and_hatch(match *m, std::string name, int sex);
};

#endif
