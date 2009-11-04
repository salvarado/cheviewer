#ifndef __UI_H_
#define __UI_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#if HAVE_CONFIG_H
#include <config.h>
#if HAVE_OMAPGTK
#include <omap-legacy-dialog.h>
#else
#endif
#endif

typedef struct UiData_ UiData;

#include "app.h"

#include "omx_area.h"

/**
 * Hols UI data (GTK+ stuff).
 */
struct UiData_
{
	GtkWidget *file_selector; /**< The file selection dialog widget. */
	OmxArea *omx_area;
        GtkWidget *stop_button;
        GtkWidget *play_button;
        GtkWidget *fileopen_button;
        GtkWidget *lcdout_radio;
        GtkWidget *tvout_radio;
        GtkWidget *fullscreen_check;

};

/**
 * Creates the UI.
 * 
 * @param app_data The application data.
 * 
 * @return The UI data.
 */
UiData *ui_data_new (AppData *app_data);

/**
 * Destroys the UI.
 * 
 * @param ui_data The UI data.
 */
void ui_data_destroy (UiData *ui_data);


#endif /* __UI_H_ */
