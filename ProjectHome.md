# GatorQue Engines #
# Mission #
To create, as a community, the easiest and most extendable game engines that will run on all major platforms and provide the corner stone for game creation for all game genres through open-component based APIs and file formats.

---

## News/Welcome! ##
Jul 28, 2012
The GQE/GQP team has entered their first game into the Liberated Pixel Cup (LPC) competition at OpenGameArt.org. The name of the game is Traps and Treasures (look for gqp-tnt Google Code project for the source) and the competition ends on Tuesday, July 31st. Wish us luck on winning some of the prize money.
A few minor changes relating to this competition and being able to build under Linux were fixed in the 0.22.2 patch just uploaded.

Jul 6, 2012
The GQE team has improved a few key features with the handling of Prototype and Instance classes. This includes adding a new class to GQE core called EventManager. The purpose of this class is to enable you to make any class member method into an Event. We added this so that then an IEntity is "destroyed" it can be safely removed at the end of the Game Loop (see the Prototype::HandleCleanup and StateManager::Cleanup methods that are called). The next goal for the GQE team is to add a NetworkManager and associated classes to allow for multiplayer game support.

May 26, 2012
The GQE team has radically changed the way AssetManager works. AssetManager now manages AssetHandler classes. Now you can create your own custom AssetHandler and Asset classes and have them managed by the AssetManager. We have also added additional documentation to the Entity library. We hope to have a great example for the Entity library soon. You should still check out my sister project GQP where I have added 3 new external libraries that you can include in your projects bringing the list to the following external libraries: Thor, SFML, GQE, Box2D, TinyXML, zlib, and TmxParser.

Mar 23, 2012
I have added another example to GQE (in version 0.17) called SpaceDots.  It is a really great example that uses the SplashState, FontAsset, ImageAsset, and SoundAsset classes. Please let me know what you think!

I have also merged in a new library called gqe-entities. It was created by Jacob Dix and will be the basis upon which you can manage game objects better in GQE. As an optional GQE library you can choose to use this implementation or ignore it as you desire.

Feb 11, 2012
I have been working hard on a sister project that will make using GQE easier. The project is called [GatorQue Projects](http://code.google.com/p/gqp) or GQP for short. The purpose of this project is to make it easier for you to download, configure, and build various 3rd party/external libraries you want to use for your project. It currently supports SFML, GQE, and Thor. Are there other 3rd party/external libraries you are using? Please create an issue for them on the GQP project site and I will work on getting them added.

I am now going to focus my efforts on both GQE/GQP documentation and examples for GQE.

Thanks again for your patience and I look forward to working on making GQE something you can use to _quickly_ get your projects off the ground which is why I have now created GQP.

---

## Features/Goals ##
  * Basic Game Engine
  * Consistent Game Engine speed on both slow and fast platforms
  * Object Oriented Design
  * Well documented
  * Easily Extendable

---

## Roadmap ##
The roadmap https://trello.com/gqe will now become more collaborative using a really cool free service called Trello (made by my hero Joel Spolsky). Trello allows you to visually track my development progress and participate by leaving comments, checklists, and other things on the development board.

This list of features/issues will be moved to the Trello board over the next few days.
  * Improve AssetManager features (resolve 1-2 issues) - Soon
  * Add Game Object classes (to assist with future examples of real games) - Soon
  * Add DialogManager and various basic DialogWidgets
  * Add new State classes (For loading assets and displaying a real _working_ menu screen)