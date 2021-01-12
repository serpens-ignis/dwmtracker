/*
 * Dragon Warrior Monsters Tracker
 * By Snakek github.com/Snakek
 * Released under the GNU GPL
 * See LICENSING for more details
*/

#include "tracker.h"

tracker::tracker()
{
	for(int i = 0; i < 16; i++)
	{
		monsters[i] = nullptr;
		monsters_asleep[i] = nullptr;
		eggs[i] = nullptr;
		eggs_asleep[i] = nullptr;
	}

	load_dex();
	load_combos();

	return;
}

tracker::~tracker()
{
	for(int i = 0; i < 16; i++)
	{
		delete monsters[i];
		delete monsters_asleep[i];
		delete eggs[i];
		delete eggs_asleep[i];
	}
}

void tracker::load_farm(std::string filename)
{
	std::ifstream savefile(filename);

	for(int i = 0; i < 16; i++)
	{
		delete monsters[i];
		monsters[i] = nullptr;

		std::string line;
		std::getline(savefile, line);
		if(line != "NULL")
		{
			monsters[i] = new monster;
			std::istringstream line_stream(line);
			line_stream >> monsters[i]->name >> monsters[i]->species >> monsters[i]->family >> monsters[i]->sex
					 >> monsters[i]->plus_value >> monsters[i]->level;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		delete monsters_asleep[i];
		monsters_asleep[i] = nullptr;

		std::string line;
		std::getline(savefile, line);
		if(line != "NULL")
		{
			monsters_asleep[i] = new monster;
			std::istringstream line_stream(line);
            line_stream >> monsters_asleep[i]->name >> monsters_asleep[i]->species >> monsters_asleep[i]->family >> monsters_asleep[i]->sex
					 >> monsters_asleep[i]->plus_value >> monsters_asleep[i]->level;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		delete eggs[i];
		eggs[i] = nullptr;

		std::string line;
		std::getline(savefile, line);
		if(line != "NULL")
		{
			eggs[i] = new monster;
			std::istringstream line_stream(line);
            line_stream >> eggs[i]->name >> eggs[i]->species >> eggs[i]->family >> eggs[i]->sex
					 >> eggs[i]->plus_value >> eggs[i]->level;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		delete eggs_asleep[i];
		eggs_asleep[i] = nullptr;

		std::string line;
		std::getline(savefile, line);
		if(line != "NULL")
		{
			eggs_asleep[i] = new monster;
			std::istringstream line_stream(line);
            line_stream >> eggs_asleep[i]->name >> eggs_asleep[i]->species >> eggs_asleep[i]->family >> eggs_asleep[i]->sex
					 >> eggs_asleep[i]->plus_value >> eggs_asleep[i]->level;
		}
	}
	return;
}

void tracker::save_farm(std::string filename)
{
	std::ofstream savefile(filename);

	for(int i = 0; i < 16; i++)
	{
		if(monsters[i] == nullptr)
		{
			savefile << "NULL" << std::endl;
		}
		else
		{
			savefile << monsters[i]->name << " " << monsters[i]->species << " " << monsters[i]->family << " "
					 << monsters[i]->sex << " " << monsters[i]->plus_value << " " << monsters[i]->level << std::endl;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		if(monsters_asleep[i] == nullptr)
		{
			savefile << "NULL" << std::endl;
		}
		else
		{
			savefile << monsters_asleep[i]->name << " " << monsters_asleep[i]->species << " " << monsters_asleep[i]->family << " " 
					 << monsters_asleep[i]->sex << " " << monsters_asleep[i]->plus_value << " " << monsters_asleep[i]->level << std::endl;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		if(eggs[i] == nullptr)
		{
			savefile << "NULL" << std::endl;
		}
		else
		{
			savefile << eggs[i]->name << " " << eggs[i]->species << " " << eggs[i]->family << " " 
					 << eggs[i]->sex << " " << eggs[i]->plus_value << " " << eggs[i]->level << std::endl;
		}
	}

	for(int i = 0; i < 16; i++)
	{
		if(eggs_asleep[i] == nullptr)
		{
			savefile << "NULL" << std::endl;
		}
		else
		{
			savefile << eggs_asleep[i]->name << " " << eggs_asleep[i]->species << " " << eggs_asleep[i]->family << " " 
					 << eggs_asleep[i]->sex << " " << eggs_asleep[i]->plus_value << " " << eggs_asleep[i]->level << std::endl;
		}
	}
	return;
}

void tracker::load_dex()
{
	std::ifstream dexfile("data/dex.txt");
    std::ifstream notesfile("data/dex_notes.txt");
    std::ifstream locationsfile("data/locations.txt");

	for(int i = 0; i < 215; i++)
	{
		dexfile >> monsterdex[i].species >> monsterdex[i].family >> monsterdex[i].max_level >> monsterdex[i].xp_speed
				>> monsterdex[i].sex_probability >> monsterdex[i].flying >> monsterdex[i].metal_slime >> monsterdex[i].hp_growth
				>> monsterdex[i].mp_growth >> monsterdex[i].atk_growth >> monsterdex[i].def_growth >> monsterdex[i].agl_growth
				>> monsterdex[i].int_growth >> monsterdex[i].skills[0] >> monsterdex[i].skills[1] >> monsterdex[i].skills[2]
				>> monsterdex[i].frizz_resistance >> monsterdex[i].sizz_resistance >> monsterdex[i].bang_resistance >> monsterdex[i].woosh_resistance
				>> monsterdex[i].zap_resistance >> monsterdex[i].crack_resistance >> monsterdex[i].dazzle_resistance >> monsterdex[i].snooze_resistance
				>> monsterdex[i].whack_resistance >> monsterdex[i].drain_magic_resistance >> monsterdex[i].fizzle_resistance >> monsterdex[i].fuddle_resistance
				>> monsterdex[i].sap_resistance >> monsterdex[i].decelerate_resistance >> monsterdex[i].kamikazee_resistance >> monsterdex[i].megamagic_resistance
				>> monsterdex[i].fire_breath_resistance >> monsterdex[i].ice_breath_resistance >> monsterdex[i].poison_resistance >> monsterdex[i].paralysis_resistance
				>> monsterdex[i].curse_resistance >> monsterdex[i].stun_resistance >> monsterdex[i].danceshut_resistance >> monsterdex[i].mouthshut_resistance
				>> monsterdex[i].rock_resistance >> monsterdex[i].gigaslash_resistance;

		std::getline(notesfile, monsterdex[i].note);

        std::string line, nullstr;
        std::getline(locationsfile, line);
        std::istringstream line_stream(line);
        line_stream >> nullstr;
        while(line_stream)
        {
            std::string location;
            line_stream >> location;
            if(!line_stream)
            {
                break;
            }
            monsterdex[i].locations.push_back(location);
        }
    }

	return;
}

void tracker::load_combos()
{
	std::ifstream ff_file("data/ff_combos.txt");
	std::ifstream fs_file("data/fs_combos.txt");
	std::ifstream sf_file("data/sf_combos.txt");
	std::ifstream ss_file("data/ss_combos.txt");

	std::string line;
	while(getline(ff_file, line))
	{
		std::istringstream line_stream(line);
		std::string result;
		int pedigree_family;
		int mate_family;
		line_stream >> result >> pedigree_family >> mate_family;
		combos[0].push_back({result, pedigree_family, mate_family, "", ""});
	}

	while(getline(fs_file, line))
	{
		std::istringstream line_stream(line);
		std::string result;
		int pedigree_family;
		std::string mate_species;
		line_stream >> result >> pedigree_family >> mate_species;
		combos[1].push_back({result, pedigree_family, 0, "", mate_species});
	}

	while(getline(sf_file, line))
	{
		std::istringstream line_stream(line);
		std::string result;
		std::string pedigree_species;
		int mate_family;
		line_stream >> result >> pedigree_species >> mate_family;
		combos[2].push_back({result, 0, mate_family, pedigree_species, ""});
	}

	while(getline(ss_file, line))
	{
		std::istringstream line_stream(line);
		std::string result;
		std::string pedigree_species;
		std::string mate_species;
		line_stream >> result >> pedigree_species >> mate_species;
		combos[3].push_back({result, 0, 0, pedigree_species, mate_species});
	}

	return;
}

int tracker::first_free_key(monster **m_array)
{
	for(int i = 0; i < 16; i++)
	{
		if(m_array[i] == nullptr)
		{
			return i;
		}
	}

	return gFULL;
}

int tracker::find_monster_family(std::string species)
{
    for(int i = 0; i < 215; i++)
    {
        if(species == monsterdex[i].species)
        {
            return monsterdex[i].family;
        }
    }
    return -1;
}

int tracker::calculate_plus_value(match *m)
{
    int plus_value = 0;
    int level_sum = (m->pedigree->level + m->mate->level);
    switch(level_sum)
    {
        case 2 ... 19:
            break;
        case 20 ... 39:
            plus_value = 1;
            break;
        case 40 ... 59:
            plus_value = 2;
            break;
        case 60 ... 75:
            plus_value = 3;
            break;
        case 76 ... 99:
            plus_value = 4;
            break;
        default:
            plus_value = 5;
            break;
    }
    if(m->pedigree->plus_value > m->mate->plus_value)
    {
        plus_value += m->pedigree->plus_value;
    }
    else
    {
        plus_value += m->mate->plus_value;
    }

    return plus_value;
}

monster *tracker::add_monster(monster** m_array, std::string name, std::string species,
					int sex, int plus_value, int level)
{
	int key = first_free_key(m_array);
	if(key == gFULL)
	{
		return nullptr;
	}

    int family = find_monster_family(species);

	m_array[key] = new monster;
	m_array[key]->name = name;
	m_array[key]->species = species;
	m_array[key]->family = family;
	m_array[key]->sex = sex;
	m_array[key]->plus_value = plus_value;
	m_array[key]->level = level;

	return m_array[key];
}

void tracker::remove_monster(monster **m_array, monster *m)
{
	for(int i = 0; i < 16; i++)
	{
		if(m_array[i] == m)
		{
			delete m_array[i];
			m_array[i] = nullptr;
			break;
		}
	}
	return;
}

void tracker::edit_monster(monster *m, std::string name, std::string species,
          int sex, int plus_value, int level)
{
    m->name = name;
    m->species = species;
    m->family = find_monster_family(species);
    m->sex = sex;
    m->plus_value = plus_value;
    m->level = level;

    return;
}

int tracker::move_monster(monster *m, monster **source, monster **destination)
{
	int key = first_free_key(destination);
	if(key == gFULL)
	{
		return gFULL;
	}
	destination[key] = m;

	for(int i = 0; i < 16; i++)
	{
		if(source[i] == m)
		{
			source[i] = nullptr;
			break;
		}
	}

	return 0;

}

void tracker::swap_monsters()
{
	std::swap(monsters, monsters_asleep);
	return;
}

void tracker::swap_eggs()
{
	std::swap(eggs, eggs_asleep);
	return;
}

match * tracker::create_match(monster *pedigree, monster *mate)
{
    if(pedigree == nullptr || mate == nullptr || pedigree->sex == mate->sex ||
       pedigree->level < 10 || mate->level < 10)
	{
		return nullptr;
	}

	std::string result = "";

	for(auto i = combos[3].begin(); i != combos[3].end(); i++)
	{
        if(i->pedigree_species == pedigree->species &&
                i->mate_species == mate->species)
		{
			result = i->result;
			break;
		}
	}
    if(result == "")
	{
		for(auto i = combos[2].begin(); i != combos[2].end(); i++)
		{	
            if(i->pedigree_species == pedigree->species &&
					i->mate_family == mate->family)
			{
				result = i->result;
				break;
			}
		}
	}
    if(result == "")
	{
		{
		for(auto i = combos[1].begin(); i != combos[1].end(); i++)
			if(i->pedigree_family == pedigree->family &&
                i->mate_species == mate->species)
			{
				result = i->result;
				break;
			}
		}
	}
    if(result == "")
	{
		for(auto i = combos[0].begin(); i != combos[0].end(); i++)
		{
			if(i->pedigree_family == pedigree->family &&
					i->mate_family == mate->family)
			{
				result = i->result;
				break;
			}
		}
	}

    if(result == "")
	{
		return nullptr;
	}

	match *m = new match;
	m->pedigree = pedigree;
	m->mate = mate;
	m->result = result;

	return m;
}

void tracker::calculate_matches()
{
    for(int i = 0; i < 256; i++)
    {
        delete matches[i];
        matches[i] = nullptr;
    }

    int counter = 0;
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			match *m1 = create_match(monsters[i],monsters[j]);
			if(m1 != nullptr)
			{
                matches[counter] = m1;
                counter++;
			}

			match *m2 = create_match(monsters[i], monsters_asleep[j]);
			if(m2 != nullptr)
			{
                matches[counter] = m2;
                counter++;
            }

			match *m3 = create_match(monsters_asleep[i], monsters[j]);
			if(m3 != nullptr)
			{
                matches[counter] = m3;
                counter++;
            }

			match *m4 = create_match(monsters_asleep[i], monsters_asleep[j]);
			if(m4 != nullptr)
			{
                matches[counter] = m4;
                counter++;
            }
		}
	}

	return;
}

monster *tracker::breed(match *m)
{
    int plus_value = calculate_plus_value(m);

    monster *egg = nullptr;
	egg = add_monster(eggs, m->result, m->result,
			unknown, plus_value, 1);
	if(egg == nullptr)
	{
		egg = add_monster(eggs_asleep, m->result, m->result,
				unknown, plus_value, 1);
		if(egg == nullptr)
		{
			return nullptr;
		}
	}

	remove_monster(monsters, m->pedigree);
	remove_monster(monsters, m->mate);
	remove_monster(monsters_asleep, m->pedigree);
	remove_monster(monsters_asleep, m->mate);

	return egg;
}

int tracker::hatch(monster *egg, std::string name, int sex)
{
	for(int i = 0; i < 16; i++)
	{
		if(eggs[i] == egg)
		{
			if(move_monster(egg, eggs, monsters) == gFULL)
			{
				if(move_monster(egg, eggs, monsters_asleep) == gFULL)
				{
					return gFULL;
				}
			}
        }

        if(eggs_asleep[i] == egg)
        {
            if(move_monster(egg, eggs_asleep, monsters) == gFULL)
            {
                if(move_monster(egg, eggs_asleep, monsters_asleep) == gFULL)
                {
                    return gFULL;
                }
            }
        }
	}
	egg->name = name;
	egg->sex = sex;
	
	return 0;
}

int tracker::breed_and_hatch(match *m, std::string name, int sex)
{
	monster *newmonster = breed(m);
	if(newmonster == nullptr)
	{
		return gFULL;
	}
	if(hatch(newmonster, name, sex))
	{
		return gFULL_HATCH;
	}

	return 0;
}
