                       GatorQue Engines (GQE)

Mission
=======
To create, as a community, the easiest and most extendable game engines that
will run on all major platforms and provide the corner stone for game creation
for all game genres through open-component based APIs and file formats. 

License
=======
For license information, please see license.txt.

Contact
=======
For reporting bugs, feature requests, see the GoogleCode project listed below
or contact the author.  For donations, contact the author.

Author(s)
=========
Ryan Lindeman (GatorQue) <ryanlindeman@gmail.com>

Source
======
Get a local copy of the gqe repository with this command:
hg clone https://gqe.googlecode.com/hg/ gqe 

Browse online:
http://code.google.com/p/gqe/source/browse/

WWW
===
http://code.google.com/p/gqe/
http://www.sfml-dev.org/wiki/en/tutoriels/gqe_en
http://www.sfml-dev.org/wiki/en/sources/gqe_en

Translations
============
I could use a french translator for the tutorial wiki on SFML's website:
http://www.sfml-dev.org/wiki/en/tutoriels/gqe_en

Building
========
  Windows (MSVC command line)
    mkdir build
    cd build
    cmake -G "NMake Makefiles" ..
    nmake
  Windows (CodeBlocks/Mingw32)
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    mingw32-make
  Linux/MacOS
    mkdir build
    cd build
    cmake -G "Unix Makefiles" ..
    make
  
The libraries will be created in <build-folder>/lib
The examples will be created in <build-folder>/examples
The documentation will be created in <build-folder>/doc

Packaging
=========
To be added


Who uses GQE (send me your links, I'll add your project here...)
================


Thanks
======
	Laurent Gomila for SFML
	fixus971 for linux compiler fixes
	eXpl0it3r for recommendations
