SUBDIRS = src \
          pixmaps \
          doc

desktopdir = $(datadir)/applications
desktop_in_files = dplayer.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
 
EXTRA_DIST = dplayer.desktop.in

clean-local:
	rm -f dplayer.desktop
