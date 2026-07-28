# Danganronpa-Online-Client
This is the a forked version of the offical DRO client last updated by Crystal Warrior. it is a derivative of [Attorney-Online-Client-Remake](https://github.com/AttorneyOnline/AO2-Client). 

## TO DO LIST

Remove all calls from BASS.DLL and replace it with a more open source dependency instead of properiety. 

Replace BASS.DLL with miniaudio https://miniaud.io/

Project Cleanup

Bug Fixes

Bug Investigation:
BGs are black upon entering a room until someone speaks and if someone sends a long message, the beginning parts are cut off in the main view.

Text cutting off issues, probably related to the new queue implementation in 1.8.2. Scrap it back to 1.8.0's way of doing it for better parity for the time being.

Flash  effects not appearing visual

Not a replacement for DRO client, just a fork intended to clean-up some legacy crust and fix some issues.

## Qt
This project uses Qt6.8.3, which is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.txt) with [certain licensing restrictions and exceptions](https://www.qt.io/qt-licensing-terms/). To comply with licensing requirements for static linking, object code is available if you would like to relink with an alternative version of Qt, and the source code for Qt may be found at https://github.com/qt/qtbase, http://code.qt.io/cgit/, or at https://qt.io.

Copyright (C) 2026 The Qt Company Ltd.

## BASS
This project uses [BASS shared library](http://www.un4seen.com/). 

Copyright (c) 1999-2026 Un4seen Developments Ltd. All rights reserved.

