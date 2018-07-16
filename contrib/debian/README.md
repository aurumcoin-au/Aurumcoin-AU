
Debian
====================
This directory contains files used to package aurumcoind/aurumcoin-qt
for Debian-based Linux systems. If you compile aurumcoind/aurumcoin-qt yourself, there are some useful files here.

## aurumcoin: URI support ##


aurumcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install aurumcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your aurumcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/aurumcoin128.png` to `/usr/share/pixmaps`

aurumcoin-qt.protocol (KDE)

