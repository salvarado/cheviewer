#include "main.h"
#include "ui.h"

static GtkWidget *create_menubar_menu (AppData *, GtkWidget  *);
static void cb_start_playback (GtkWidget *, gpointer);
static void cb_stop_playback (GtkWidget *, gpointer);
static void cb_button_file_open (GtkWidget *, gpointer);
static void cb_tvout_toggled (GtkWidget *, gpointer);
static void cb_fullscreen_toggled (GtkWidget *, gpointer);
static gboolean cb_keypress_handler (GtkWidget *, GdkEventKey *, gpointer);
static void create_file_open_dialog (AppData *);

UiData *
ui_data_new (AppData *app_data)
{
	UiData *ui_data;
	GtkWidget *window;
	GtkWidget *main_vbox;
	GtkWidget *buttons_hbox1;
	GtkWidget *buttons_hbox2;
	GtkWidget *menubar;

	ui_data = g_new (UiData, 1);

	/* Make a window. */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_title (GTK_WINDOW(window), "DPlayer");
        g_signal_connect (G_OBJECT (window), "key_press_event",
            G_CALLBACK (cb_keypress_handler), app_data);

	/* Make a vbox to put the menus in. */
	main_vbox = gtk_vbox_new (FALSE, 1);
	gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 1);
	gtk_container_add (GTK_CONTAINER (window), main_vbox);

	/* Make a hbox to put the buttons. */
	buttons_hbox1 = gtk_hbox_new (TRUE, 0);
	buttons_hbox2 = gtk_hbox_new (TRUE, 0);


        /* Make a stop button */
        ui_data->play_button = gtk_button_new_with_label (">");
        g_signal_connect (G_OBJECT (ui_data->play_button), "clicked",
            G_CALLBACK (cb_start_playback), app_data);

        /* Make a stop button */
        ui_data->stop_button = gtk_button_new_with_label ("[]");
        g_signal_connect (G_OBJECT (ui_data->stop_button), "clicked",
            G_CALLBACK (cb_stop_playback), app_data);

        /* Make a file open button */
        ui_data->fileopen_button = gtk_button_new_from_stock (GTK_STOCK_OPEN);
        g_signal_connect (G_OBJECT (ui_data->fileopen_button), "clicked",
            G_CALLBACK (cb_button_file_open), app_data);

        ui_data->lcdout_radio = gtk_radio_button_new_with_label(NULL,
            "LCD");
        ui_data->tvout_radio = gtk_radio_button_new_with_label_from_widget(
            GTK_RADIO_BUTTON (ui_data->lcdout_radio), "TV");
        g_signal_connect (G_OBJECT (ui_data->tvout_radio), "toggled",
            G_CALLBACK (cb_tvout_toggled), app_data);

        ui_data->fullscreen_check = gtk_check_button_new_with_label (
            "Fullscreen");
        g_signal_connect (G_OBJECT (ui_data->fullscreen_check), "toggled",
            G_CALLBACK (cb_fullscreen_toggled), app_data);


        ui_data->omx_area = omx_area_new();
	omx_area_set_color (ui_data->omx_area, 0x0);
	menubar = create_menubar_menu (app_data, window);

	gtk_box_pack_start (GTK_BOX (buttons_hbox1),
            GTK_WIDGET (ui_data->play_button), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (buttons_hbox1),
            GTK_WIDGET (ui_data->stop_button), TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (buttons_hbox2),
            GTK_WIDGET (ui_data->fileopen_button), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (buttons_hbox2),
            GTK_WIDGET (ui_data->lcdout_radio), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (buttons_hbox2),
            GTK_WIDGET (ui_data->tvout_radio), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (buttons_hbox2),
            GTK_WIDGET (ui_data->fullscreen_check), TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (main_vbox), GTK_WIDGET (menubar),
            FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (main_vbox),
            GTK_WIDGET (ui_data->omx_area), TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (main_vbox), GTK_WIDGET (buttons_hbox1),
            FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (main_vbox), GTK_WIDGET (buttons_hbox2),
            FALSE, FALSE, 0);

	gtk_widget_show_all (window);

        gtk_widget_set_sensitive (ui_data->play_button, FALSE);
        gtk_widget_set_sensitive (ui_data->stop_button, FALSE);

        omx_area_set_listener (ui_data->omx_area,
            G_OBJECT(app_data->core_data->videosink));
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

	core_data_open_file (app_data->core_data, filename);

	gtk_widget_destroy (app_data->ui_data->file_selector);

        gtk_widget_set_sensitive (app_data->ui_data->play_button, TRUE);

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
        chdir("/");
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

        chdir("/3gp_samples");
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
 * Our menu, an array of @c GtkItemFactoryEntry structures that defines each menu item.
 */
static GtkItemFactoryEntry menu_items[] = {
	{ "/_File",         NULL,         NULL,           0, "<Branch>" },
	{ "/File/_Open",    "<control>O", cb_menu_file_open,   1, "<StockItem>", GTK_STOCK_OPEN },
	{ "/File/sep1",     NULL,         NULL,           0, "<Separator>" },
	{ "/File/_Quit",    "<CTRL>Q",    gtk_main_quit,  0, "<StockItem>", GTK_STOCK_QUIT },
#if 0
	{ "/_Help",         NULL,         NULL,           0, "<LastBranch>" },
	{ "/_Help/About",   NULL,         NULL,           0, "<Item>" },
#endif
};

/**
 * The count of menu items.
 */
static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

/**
 * Creates menubar widget made from the @c menu_items array.
 *
 * @param app_data The internal application data.
 * @param window The main window.
 *
 * @return The menu widget.
 */
static GtkWidget *
create_menubar_menu (AppData *app_data, GtkWidget  *window)
{
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group;

	accel_group = gtk_accel_group_new ();
	item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);

	gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, app_data);
	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

	return gtk_item_factory_get_widget (item_factory, "<main>");
}

/**
 * keypress handler (callback).
 *
 * Called when any key is pressed pressed.
 *
 * @param widget The widget.
 * @param event The event.
 * @param cb_data The calllback data.
 */
static gboolean cb_keypress_handler (GtkWidget *widget, GdkEventKey *event,
    gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    g_print("Pressed key 0x%X\n", event->keyval);

    switch (event->keyval)
    {
        case GDK_slash:
        {
            gtk_widget_activate (GTK_WIDGET (app_data->ui_data->play_button));
            break;
        }
        case GDK_0:
        {
            gtk_widget_activate (GTK_WIDGET (app_data->ui_data->stop_button));
            break;
        }
        case GDK_7:
        {
            gtk_widget_activate (
                GTK_WIDGET (app_data->ui_data->fileopen_button));
            break;
        }
        case GDK_8:
        {
            gtk_widget_activate (
                GTK_WIDGET (app_data->ui_data->lcdout_radio));
            break;
        }
        case GDK_9:
        {
            gtk_widget_activate (
                GTK_WIDGET (app_data->ui_data->tvout_radio));
            break;
        }
        case GDK_L2:
        {
            gtk_widget_activate (
                GTK_WIDGET (app_data->ui_data->fullscreen_check));
            break;
        }
        case GDK_Escape:
        {
            gtk_widget_destroy (widget);
            break;
        }
        default:
        /*unhandled*/
            break;
    }

    return FALSE;
}

/**
 * Play button (callback).
 *
 * Called when play button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_start_playback (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    if (gtk_toggle_button_get_active(
            GTK_TOGGLE_BUTTON(app_data->ui_data->lcdout_radio)))
    {
        if (gtk_toggle_button_get_active(
                GTK_TOGGLE_BUTTON(app_data->ui_data->fullscreen_check)))
        {
            omx_area_set_listener (app_data->ui_data->omx_area,
                NULL);
            core_data_enable_lcdfullscreen (app_data);
        }
        else
        {
            core_data_disable_lcdfullscreen (app_data);
            omx_area_set_listener (app_data->ui_data->omx_area,
                G_OBJECT(app_data->core_data->videosink));
        }
    }
    core_data_start_playback (app_data->core_data);
    gtk_widget_set_sensitive (app_data->ui_data->play_button, FALSE);
    gtk_widget_set_sensitive (app_data->ui_data->stop_button, TRUE);
    gtk_widget_set_sensitive (app_data->ui_data->fileopen_button, FALSE);
    gtk_widget_set_sensitive (app_data->ui_data->tvout_radio, FALSE);
    gtk_widget_set_sensitive (app_data->ui_data->lcdout_radio, FALSE);
}

/**
 * TV out button (callback).
 *
 * Called when tv button changes is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_tvout_toggled (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
    {
        gtk_toggle_button_set_active (
            GTK_TOGGLE_BUTTON (app_data->ui_data->fullscreen_check), FALSE);
        gtk_widget_set_sensitive (app_data->ui_data->fullscreen_check, FALSE);
        core_data_disable_lcdfullscreen (app_data);
        omx_area_set_listener (app_data->ui_data->omx_area,
            NULL);

        core_data_set_tvout (app_data);
    }
    else
    {
        core_data_set_lcdout (app_data->core_data);
        omx_area_set_listener (app_data->ui_data->omx_area,
            G_OBJECT(app_data->core_data->videosink));
        gtk_widget_set_sensitive (app_data->ui_data->fullscreen_check, TRUE);
    }
}

/**
 * Fullscreen button (callback).
 *
 * Called when fullscreen button changes is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_fullscreen_toggled (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
    {
        omx_area_set_listener (app_data->ui_data->omx_area,
            NULL);
        core_data_enable_lcdfullscreen (app_data);
    }
    else
    {
        core_data_disable_lcdfullscreen (app_data);
        omx_area_set_listener (app_data->ui_data->omx_area,
            G_OBJECT(app_data->core_data->videosink));
    }
}

/**
 * Stop button (callback).
 *
 * Called when stop button is pressed.
 *
 * @param widget The widget.
 * @param cb_data The calllback data.
 */
static void cb_stop_playback (GtkWidget *widget, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;

    core_data_stop_playback (app_data->core_data);
    gtk_widget_set_sensitive (app_data->ui_data->play_button, TRUE);
    gtk_widget_set_sensitive (app_data->ui_data->stop_button, FALSE);
    gtk_widget_set_sensitive (app_data->ui_data->fileopen_button, TRUE);
    gtk_widget_set_sensitive (app_data->ui_data->tvout_radio, TRUE);
    gtk_widget_set_sensitive (app_data->ui_data->lcdout_radio, TRUE);
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

