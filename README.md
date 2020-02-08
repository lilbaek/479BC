# Julius ![](res/julius_48.png)
  
|             | Latest release |
|-------------|----------------|
| Windows     | [![Download](https://api.bintray.com/packages/keriew/JuliusGC/windows_release/download.svg)](https://bintray.com/keriew/JuliusGC/windows_release/_latestVersion) ||


![](https://github.com/bvschaik/julius/blob/master/res/sce_sys/livearea/contents/bg.png?raw=true)

Julius GC is a fork of the Julius project that intends to incorporate gameplay changes.
=======
Julius is an open source re-implementation of Caesar III.

The aim of this project is to create an open-source version of Caesar 3, with the same logic
as the original, but with some UI enhancements, that is able to be played on multiple platforms.
The same logic means that the saved games are 100% compatible, and any gameplay bugs
present in the original Caesar 3 game will also be present in Julius.
It is meant as a drop-in replacement for Caesar 3, with the same look and feel, so there will be no major UI changes.

UI enhancements include:
- Support for widescreen resolutions
- Windowed mode support for 32-bit desktops
- Several small in-game quality of life improvements

Julius requires the original assets (graphics, sounds, etc) from Caesar 3 to run.
It optionally [supports the high-quality MP3 files](doc/mp3_support.md) once provided on the
Sierra website.

## Running the game

First, download the game for your platform from the list above.

Alternatively, you can build Julius yourself. Check [Building Julius](doc/BUILDING.md)
for details.

Then you can either copy the game to the Caesar 3 folder, or run the game from an independent
folder, in which case the game will ask you to point to the Caesar 3 folder.

Note that you must have permission to write in the game data directory as the saves will be
stored there.

See [Running](doc/RUNNING.md) for further instructions and startup options.

## Bugs

Julius recreates many bugs that you can find in original Caesar 3 game, to preserve 100% saves compatibility. See the list of [Bugs & idiosyncrasies](doc/bugs.md) to find out more.
