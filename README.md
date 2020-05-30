# Augustus ![](res/julius_48.png)

[![Travis](https://api.travis-ci.org/keriew/julius.svg?branch=master)](https://travis-ci.org/Keriew/julius)

  
| Platform | Latest release |
|----------|----------------|
| Windows  | [![Download](https://api.bintray.com/packages/keriew/Augustus/windows_release/images/download.svg)](https://bintray.com/keriew/Augustus/windows_release/_latestVersion) | [![Download](https://api.bintray.com/packages/keriew/julius/windows-unstable/images/download.svg)](https://bintray.com/keriew/julius/windows-unstable/_latestVersion#files) |
| Linux AppImage | [![Download](https://api.bintray.com/packages/keriew/Augustus/linux-unstable/images/download.svg)](https://bintray.com/keriew/Augustus/linux-unstable/_latestVersion#files) |
| Mac | [![Download](https://api.bintray.com/packages/keriew/Augustus/mac-unstable/images/download.svg)](https://bintray.com/keriew/Augustus/mac-unstable/_latestVersion#files) |
| PS Vita | [![Download](https://api.bintray.com/packages/keriew/Augustus/vita-unstable/images/download.svg)](https://bintray.com/keriew/Augustus/vita-unstable/_latestVersion#files) |
| Nintendo Switch | [![Download](https://api.bintray.com/packages/keriew/Augustus/switch-unstable/images/download.svg)](https://bintray.com/keriew/Augustus/switch-unstable/_latestVersion#files) |

<<<<<<< HEAD
=======
| Platform | Latest release | Latest build (may be unstable) |
|----------|----------------|-----------------|
| Windows  | [![Download](https://api.bintray.com/packages/bvschaik/julius/windows/images/download.svg)](https://bintray.com/bvschaik/julius/windows/_latestVersion) | [![Download](https://api.bintray.com/packages/bvschaik/julius-dev/windows/images/download.svg)](https://bintray.com/bvschaik/julius-dev/windows/_latestVersion#files) |
| Linux AppImage | [![Download](https://api.bintray.com/packages/bvschaik/julius/linux/images/download.svg)](https://bintray.com/bvschaik/julius/linux/_latestVersion#files) | [![Download](https://api.bintray.com/packages/bvschaik/julius-dev/linux/images/download.svg)](https://bintray.com/bvschaik/julius-dev/linux/_latestVersion#files) |
| Mac | [![Download](https://api.bintray.com/packages/bvschaik/julius/mac/images/download.svg)](https://bintray.com/bvschaik/julius/mac/_latestVersion) | [![Download](https://api.bintray.com/packages/bvschaik/julius-dev/mac/images/download.svg)](https://bintray.com/bvschaik/julius-dev/mac/_latestVersion#files) |
| PS Vita | [![Download](https://api.bintray.com/packages/bvschaik/julius/vita/images/download.svg)](https://bintray.com/bvschaik/julius/vita/_latestVersion) | [![Download](https://api.bintray.com/packages/bvschaik/julius-dev/vita/images/download.svg)](https://bintray.com/bvschaik/julius-dev/vita/_latestVersion#files) |
| Nintendo Switch | [![Download](https://api.bintray.com/packages/bvschaik/julius/switch/images/download.svg)](https://bintray.com/bvschaik/julius/switch/_latestVersion) | [![Download](https://api.bintray.com/packages/bvschaik/julius-dev/switch/images/download.svg)](https://bintray.com/bvschaik/julius-dev/switch/_latestVersion#files) |
>>>>>>> 26d787acb528c28df01fc9cdb9ce4687829f0574

![](https://github.com/bvschaik/julius/blob/master/res/sce_sys/livearea/contents/bg.png?raw=true)

Augustus is a fork of the Julius project that intends to incorporate gameplay changes.
=======
The aim of this project is to provide enhanced, customizable gameplay to Caesar 3 using project Julius UI enhancements.

Augustus is able to load Caesar 3 and Julius saves, however saves made with Augustus are unlikely to work in either original C3 or project Julius.

<<<<<<< HEAD
Gameplay enhancements include:
- Roadblocks
- Market special orders
- Global labour pool
- Partial warehouse storage
- And more!
=======
However, a fork of Julius, named [Augustus](https://github.com/Keriew/julius), is implementing many long-wanted gameplay changes, such as roadblocks.
Beware: Augustus does not support the original Caesar 3 save files, while Julius does. So use Julius for a more vanilla experience with some visual improvements, or Augustus for a different gameplay experience.

UI enhancements for Julius include:
- Support for widescreen resolutions
- Windowed mode support for 32-bit desktops
- Several small in-game quality of life improvements
>>>>>>> 26d787acb528c28df01fc9cdb9ce4687829f0574

Augustus, like project Julius requires the original assets (graphics, sounds, etc) from Caesar 3 to run.
It optionally [supports the high-quality MP3 files](https://github.com/bvschaik/julius/wiki/MP3-Support) once provided on the Sierra website.

![](https://github.com/bvschaik/julius/blob/master/res/sce_sys/livearea/contents/bg.png?raw=true)

## Running the game

First, download the game for your platform from the list above.

Alternatively, you can build Augustus yourself. Check [Building Julius](doc/BUILDING.md)
for details.

Then you can either copy the game to the Caesar 3 folder, or run the game from an independent
folder, in which case the game will ask you to point to the Caesar 3 folder.

Note that you must have permission to write in the game data directory as the saves will be
stored there.

See [Running](doc/RUNNING.md) for further instructions and startup options.

## Bugs

Julius recreates many bugs that you can find in original Caesar 3 game, to preserve 100% saves compatibility. See the list of [Bugs & idiosyncrasies](doc/bugs.md) to find out more.
