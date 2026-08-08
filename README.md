# Danganronpa-Online-Client
This is the a forked version of the offical DRO client last updated by Crystal Warrior. it is a derivative of [Attorney-Online-Client-Remake](https://github.com/AttorneyOnline/AO2-Client). Created by SyntaxNyah not a replacement all credits to the upstream forks from each interation for laying the ground work.

## Roadmap

Main goal get people upgrade from 1.8.0 to 1.8.4 by addressing bugs so crystal warriors work doesn't go to waste. 


Currently fixed, changed or Added!
---------------------

Audio engine Bass.DLL removed and changed to miniaudio. Replaced a propriety audio engine with an AGPLv3 compatible one.

Backgrounds no longer are blank in the view port upon entering areas.  

Text no longer cuts off at all. It always follows the text wraps perfectly and doesn't go off screen.

Current Bug reports so far that I will work on in the future
-----------------------------

Fix Additive. DRO's implementation is borked. Just port over AO2's for now the text rolls over itself. - To Do 🚧 

Feature requests or quality of life improvement requests
----------------------------------------------------------------------
A toggle to disable blankposts so if you press ENTER, nothing sends. :construction:

Softer toggle called soft blankpost option where u have to press ENTER at least three times on an empty message to send a BLANKPOST. Purpose is to prevent accidental blankposts. :construction:

Make the additive text function a UI element a check box of sorts

Add Asset streaming capabilities and add a compatibility mode of sorts so DRO client can connect to modern servers just like AO2 client. This includes base and asset streaming. AO2 compat mode so modern servers like akashi can accept DRO client without issue, and if it shows no files should ask you for an option to stream the assets. :construction::construction: :construction:

Add HTTPS/WSS support for DRO client and TsuserverDR so server owners can have the OPTION utilize TLS and modern https if they so want to to protect against snooping MITM attacks(this is funny to write but yeah you get the point, https is just more secure and faster) and even cloudflare caching. This will mainly benefit servers that use websockets though and have a web server, for most DRO users this won't really change much. :construction: :construction:

Write prooject documentation with detailed easy to understand guides such as building the project with qt 6 maybe cross platform etc :construction:

Not a replacement for DRO client, just a fork intended to clean-up some legacy crust and fix some issues.

## Qt
This project uses Qt6.8.3, which is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.txt) with [certain licensing restrictions and exceptions](https://www.qt.io/qt-licensing-terms/). To comply with licensing requirements for static linking, object code is available if you would like to relink with an alternative version of Qt, and the source code for Qt may be found at https://github.com/qt/qtbase, http://code.qt.io/cgit/, or at https://qt.io.

Copyright (C) 2026 The Qt Company Ltd.

## miniaudio
This client runs on [miniaudio](https://miniaud.io/). It is released into the public domain, or MIT-0 at your option.

