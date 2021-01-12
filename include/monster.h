/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef MONSTER_H
#define MONSTER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <list>

//families
const int slime = 0;
const int dragon = 1;
const int beast = 2;
const int bird = 3;
const int plant = 4;
const int bug = 5;
const int devil = 6;
const int zombie = 7;
const int material = 8;
const int boss = 9;

//sex probabilities
const int always_male = 0;
const int high_chance_male = 1;
const int even_chance = 2;
const int high_chance_female = 3;

//sexes
const int male = 0;
const int female = 1;
const int unknown = 2;

//resistances
const int no_res = 0;
const int low_res = 1;
const int high_res = 2;
const int immune = 3;

struct monster
{
	std::string name;
	std::string species;

	int family;
	int sex;
	int plus_value;
	int level;
};

struct dex_entry
{
	std::string species;
	std::string skills[3];
	std::string note;

    std::list<std::string> locations;

	int family;
	int max_level;
	int xp_speed;
	int sex_probability;
	int flying;
	int metal_slime;

	int hp_growth;
	int mp_growth;
	int atk_growth;
	int def_growth;
	int agl_growth;
	int int_growth;

	int frizz_resistance;
	int sizz_resistance;
	int bang_resistance;
	int woosh_resistance;
	int zap_resistance;
	int crack_resistance;
	int dazzle_resistance;
	int snooze_resistance;
	int whack_resistance;
	int drain_magic_resistance;
	int fizzle_resistance;
	int fuddle_resistance;
	int sap_resistance;
	int decelerate_resistance;
	int kamikazee_resistance;
	int megamagic_resistance;
	int fire_breath_resistance;
	int ice_breath_resistance;
	int poison_resistance;
	int paralysis_resistance;
	int curse_resistance;
	int stun_resistance;
	int danceshut_resistance;
	int mouthshut_resistance;
	int rock_resistance;
	int gigaslash_resistance;
};

#endif
