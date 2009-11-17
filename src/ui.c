/*
 * ui.c
 *
 *  Created on: Nov 3, 2009
 *      Author: x0095969
 */

#include "main.h"
#include "ui.h"

static void cb_button_file_open (GtkWidget *, gpointer);
static void cb_button_slideshow_execute (GtkWidget *, gpointer);
static void cb_button_options_image (GtkWidget *, gpointer);
static void cb_button_delete (GtkWidget *, gpointer);
static void cb_hide_window (GtkWidget *);
static void create_file_open_dialog (AppData *);


UiData *
ui_data_new (AppData *app_data)
{
    UiData *ui_data;
    /*window1*/
    GtkWidget *main_hbox1;
    GtkWidget *buttons_vbox1;

    /*window2*/
    GtkWidget *window2;
    GtkWidget *main_hbox2;

    /*window3*/
    GtkWidget *window3;
    GtkWidget *main_hbox3;
    GtkWidget *buttons_vbox3;

    ui_data = g_new (UiData, 1);

#if 0
    /* Make a window2. */
    window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW(window2), "Cheviewer");
    gtk_window_set_default_size (GTK_WINDOW (window2), 854, 480);
    g_signal_connect (G_OBJECT (window2), "destroy", G_CALLBACK (cb_hide_window), NULL);

    /* Make a hbox to put omx_area in. */
    main_hbox2 = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (main_hbox2), 1); /*Verificar si esto es necesario*/
    gtk_container_add (GTK_CONTAINER (window2), main_hbox2);

        ui_data->omx_area = omx_area_new();
        omx_area_set_color (ui_data->omx_area, 0x0);
        /*omx_area will be used in 3rd window, so we need to add a reference*/
        g_object_ref (ui_data->omx_area);

        gtk_container_set_border_width (GTK_CONTAINER (ui_data->omx_area), 1);
        gtk_box_pack_start (GTK_BOX (main_hbox2), GTK_WIDGET (ui_data->omx_area), TRUE, TRUE, 0);

    gtk_widget_hide (window2);

    /* Make a window3. */
    window3 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW(window1), "Cheviewer");
    gtk_window_set_default_size (GTK_WINDOW (window1), 854, 480);
    g_signal_connect (G_OBJECT (window3), "destroy", G_CALLBACK (cb_hide_window), NULL);

    /* Make a hbox to put the buttons and scrolledwindow in. */
    main_hbox3 = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (main_hbox3), 1); /*Verificar si esto es necesario*/
    gtk_container_add (GTK_CONTAINER (window3), main_hbox3);
/*
    gtk_container_set_border_width (GTK_CONTAINER (ui_data->omx_area), 1);
    gtk_box_pack_start (GTK_BOX (main_hbox3), GTK_WIDGET (ui_data->omx_area), TRUE, TRUE, 0);
*/
    /* Make a vbox1 to put the buttons in. */
    buttons_vbox3 = gtk_vbox_new (TRUE, 1);
/*
    GtkWidget *rright_button;
    GtkWidget *rleft_button;
    GtkWidget *zoomin_button;
    GtkWidget *zoomout_button;
    GtkWidget *back_button;
    GtkWidget *fordward_button;
*/
        /* Make a slideshow button */
        ui_data->slideshow_button = gtk_button_new_from_stock (GTK_STOCK_EXECUTE);
        g_signal_connect (G_OBJECT (ui_data->slideshow_button), "clicked",
        G_CALLBACK (cb_button_slideshow_execute), app_data);

        gtk_box_pack_start (GTK_BOX (buttons_vbox3),
                GTK_WIDGET (ui_data->slideshow_button), TRUE, TRUE, 0);

        gtk_box_pack_start (GTK_BOX (main_hbox3), GTK_WIDGET (buttons_vbox3),
                FALSE, FALSE, 0);

        gtk_widget_hide (window3);
#endif

    /* Make a window1. */
    ui_data->window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW(ui_data->window1), "Cheviewer");
    gtk_window_set_default_size (GTK_WINDOW (ui_data->window1), 854, 480);
    g_signal_connect (G_OBJECT (ui_data->window1), "destroy",
    G_CALLBACK (gtk_main_quit), NULL);

    /* Make a hbox to put the buttons and scrolledwindow in. */
    main_hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (main_hbox1), 1);
    gtk_container_add (GTK_CONTAINER (ui_data->window1), main_hbox1);

    /* Make a vbox1 to put the buttons in. */
    buttons_vbox1 = gtk_vbox_new (TRUE, 1);

        /* Make a file open button */
        ui_data->fileopen_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
        g_signal_connect (G_OBJECT (ui_data->fileopen_button), "clicked",
        G_CALLBACK (cb_button_file_open), app_data);

        /* Make a slideshow button */
        ui_data->slideshow_button = gtk_button_new_from_stock (GTK_STOCK_EXECUTE);
        g_signal_connect (G_OBJECT (ui_data->slideshow_button), "clicked",
        G_CALLBACK (cb_button_slideshow_execute), app_data);

        /* Make a options button */
        ui_data->options_button = gtk_button_new_from_stock (GTK_STOCK_PREFERENCES);
        g_signal_connect (G_OBJECT (ui_data->options_button), "clicked",
        G_CALLBACK (cb_button_options_image), app_data);

        /* Make a delete button */
        ui_data->delete_button = gtk_button_new_from_stock (GTK_STOCK_DELETE);
        g_signal_connect (G_OBJECT (ui_data->delete_button), "clicked",
        G_CALLBACK (cb_button_delete), app_data);

    gtk_box_pack_start (GTK_BOX (buttons_vbox1),
            GTK_WIDGET (ui_data->fileopen_button), TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (buttons_vbox1),
            GTK_WIDGET (ui_data->slideshow_button), TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (buttons_vbox1),
            GTK_WIDGET (ui_data->options_button), TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (buttons_vbox1),
            GTK_WIDGET (ui_data->delete_button), TRUE, TRUE, 0);

    /*Make a scrolledwindow*/

    ui_data->scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ui_data->scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(ui_data->scrolledwindow1),GTK_SHADOW_IN);

    /*Icon view will be added and fill on main.c*/

    gtk_box_pack_start (GTK_BOX (main_hbox1), GTK_WIDGET (ui_data->scrolledwindow1),
            TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (main_hbox1), GTK_WIDGET (buttons_vbox1),
            FALSE, FALSE, 0);

    gtk_widget_set_sensitive (ui_data->fileopen_button, TRUE);
    gtk_widget_set_sensitive (ui_data->slideshow_button, TRUE);
    gtk_widget_set_sensitive (ui_data->options_button, TRUE);
    gtk_widget_set_sensitive (ui_data->delete_button, FALSE);

    return ui_data;
}

void
ui_data_destroy (UiData *ui_data)
{
    g_return_if_fail (ui_data != NULL);

    g_free (ui_data);
}

/**
 * File open ok (callback).
 *
 * Called when _ok_ is pressed in the file selection dialog.
 *
 * @param w The widget.
 * @param cb_data The callback data.
 */
static void
cb_file_ok_sel (GtkWidget *w, gpointer cb_data)
{
    AppData *app_data;
    const gchar *filename;

    app_data = cb_data;

    filename = gtk_file_selection_get_filename (
            GTK_FILE_SELECTION (app_data->ui_data->file_selector));

    g_print ("FIlename : % s \n", filename);

    /*core_data_open_file (app_data->core_data, filename);*/

    gtk_widget_destroy (app_data->ui_data->file_selector);

    /*gtk_widget_set_sensitive (app_data->ui_data->play_button, TRUE);*/

    chdir("/");
}

/**
 * File open cancel (callback).
 *
 * Called when _cancel_ is pressed in the file selection dialog.
 *
 * @param w The widget.
 * @param cb_data The callback data.
 */
static void
cb_file_cancel_sel (GtkWidget *w, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;
    gtk_widget_destroy (app_data->ui_data->file_selector);
    chdir("/data/cheviewer-0.1/images");
}

/**
 * File open (callback).
 *
 * Called when File->Open is activated.
 *
 * @param cb_data The calllback data.
 * @param action The action executed.
 * @param w The widget.
 */
static void
cb_menu_file_open (gpointer cb_data, guint action, GtkWidget *w)
{
    AppData *app_data;
    app_data = cb_data;
    create_file_open_dialog (app_data);
}

/**
 * Opens a file dialog
 */
static void
create_file_open_dialog (AppData *app_data)
{
    GtkWidget *file_selector;

        chdir("/data/cheviewer-0.1/images");
    file_selector = gtk_file_selection_new ("File selection");
    app_data->ui_data->file_selector = file_selector;

    g_signal_connect (
            G_OBJECT (GTK_FILE_SELECTION (file_selector)->ok_button),
            "clicked", G_CALLBACK (cb_file_ok_sel), (gpointer) app_data);

    g_signal_connect (
            G_OBJECT (GTK_FILE_SELECTION (file_selector)->cancel_button),
            "clicked", G_CALLBACK (cb_file_cancel_sel), (gpointer) app_data);

    gtk_widget_show (file_selector);
}

/**
 * File open button (callback).
 *
 * Called when file open button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_hide_window (GtkWidget *widget)
{
    gtk_widget_hide (widget);

}

/**
 * File open button (callback).
 *
 * Called when file open button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_button_file_open (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    create_file_open_dialog (app_data);
}

/**
 * Slideshow button (callback).
 *
 * Called when slideshow button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_button_slideshow_execute (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    gtk_widget_set_sensitive (app_data->ui_data->slideshow_button, FALSE);
}

/**
 * options button (callback).
 *
 * Called when options button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_button_options_image (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    gtk_widget_set_sensitive (app_data->ui_data->options_button, FALSE);
}

/**
 * options delete (callback).
 *
 * Called when delete button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_button_delete (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;
}
