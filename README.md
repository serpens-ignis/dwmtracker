# Dragon Warrior Monsters Tracker

The Dragon Warrior Monsters Tracker is a desktop application made to assist in DWM playthroughs.

![sample](/res/preview.png?raw=true)

- Keep track of your farm, check your monsters and their stats at any given time;
- See your farm's breeding possibilities by result;
- Consult an index which contains all of the game's 215 monsters, how to breed them, where they can be found, and their in-depth properties, many of which are hidden from you by the game.

Most of the information loaded by the program were found in [Alan W. Jackson's strategy guide](https://gamefaqs.gamespot.com/gbc/197155-dragon-warrior-monsters/faqs/6849).

## Download

Head over to the [releases](https://www.github.com/Snakek/dwmtracker/releases) page.
Download, extract and have fun!

## Building from Source

Install/Build Qt 5.15.2 (other Qt 5 versions probably work as well), preferably statically so that the executable will run in any machine regardless of dependencies.

From the project's root, execute qmake -config release and then make (alternatively, compile in release mode using Qt Creator).
