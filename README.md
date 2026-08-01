# Danganronpa-Online-Client
This is the a forked version of the offical DRO client last updated by Crystal Warrior. it is a derivative of [Attorney-Online-Client-Remake](https://github.com/AttorneyOnline/AO2-Client). Created by SyntaxNyah not a replacement all credits to the upstream forks from each interation for laying the ground work.

## TO DO LIST

Project Cleanup

Feature list

A toggle to disable blankposts so if you press ENTER, nothing sends. 

Softer toggle called soft blankpost option where u have to press ENTER at least three times on an empty message to send a BLANKPOST. Purpose is to prevent accidental blankposts.

Bug Fixes

Bug Investigation:
BGs are black upon entering a room until someone speaks and if someone sends a long message, the beginning parts are cut off in the main view.

Text cutting off issues, probably related to the new queue implementation in 1.8.2. Scrap it back to 1.8.0's way of doing it for better parity for the time being.

Flash  effects not appearing visual

Fix Additive. DRO's implementation is borked. Just port over AO2's for now the text rolls over itself.

Not a replacement for DRO client, just a fork intended to clean-up some legacy crust and fix some issues.

## Qt
This project uses Qt6.8.3, which is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl-3.0.txt) with [certain licensing restrictions and exceptions](https://www.qt.io/qt-licensing-terms/). To comply with licensing requirements for static linking, object code is available if you would like to relink with an alternative version of Qt, and the source code for Qt may be found at https://github.com/qt/qtbase, http://code.qt.io/cgit/, or at https://qt.io.

Copyright (C) 2026 The Qt Company Ltd.

## miniaudio
This client runs on [miniaudio](https://miniaud.io/). It is released into the public domain, or MIT-0 at your option.

