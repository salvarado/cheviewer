/*
 * ui.h
 *
 *  Created on: Nov 3, 2009
 *      Author: x0095969
 */

#ifndef UI_H_
#define UI_H_

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
    /*common*/
    OmxArea *omx_area;
    GtkWidget *file_selector; /**< The file selection dialog widget. */
    GtkWidget *slideshow_button;
    GtkWidget *delete_button;
    GtkWidget *thumbnails_button;
    /*window1*/
    GtkWidget *fileopen_button;
    GtkWidget *options_button;
    GtkWidget *iconview;
    GtkWidget *scrolledwindow1;
    GtkListStore  *list_store;
    GtkWidget *window1;

    /*window3*/
    GtkWidget *rright_button;
    GtkWidget *rleft_button;
    GtkWidget *zoomin_button;
    GtkWidget *zoomout_button;
    GtkWidget *back_button;
    GtkWidget *fordward_button;

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


#endif /* UI_H_ */

