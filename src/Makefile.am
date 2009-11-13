bin_PROGRAMS = cheviewer
dplayer_SOURCES = app.c core.c main.c ui.c shared.c omx_area.c
dplayer_CFLAGS = $(GST_CFLAGS) $(GTK_CFLAGS)
dplayer_LDADD = $(GST_LIBS) $(GTK_LIBS)

AM_CFLAGS = -g3 $(GST_FLAGS) $(GTK_CFLAGS)

noinst_HEADERS = app.h core.h main.h ui.h shared.h omx_area.h
