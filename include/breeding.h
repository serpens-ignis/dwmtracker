/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#ifndef BREEDING_H
#define BREEDING_H

#include "monster.h"

struct match
{
	monster *pedigree;
	monster *mate;
	std::string result;
};

struct combo
{
	std::string result;
	int pedigree_family;
	int mate_family;
	std::string pedigree_species;
	std::string mate_species;
};

#endif
