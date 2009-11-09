
#include "main.h"

#include <assert.h>


enum
{
  COL_DISPLAY_NAME,
  COL_PIXBUF,
  NUM_COLS
};

void
fill_model (AppData *app_data)
{
    GtkTreeIter    iter;
    GdkPixbuf *icon;
    GdkPixbuf *image;
    GError *err = NULL;
    gboolean valid;

    g_print ("fill_model : ENTER \n");

    chdir("/data/cheviewer-0.1/images");

        g_print ("DAL_134.jpg \n");
        image = gdk_pixbuf_new_from_file("DAL_134.jpg", &err);
        assert(err==NULL);

        gtk_list_store_append(app_data->ui_data->list_store, &(iter));
        gtk_list_store_set(app_data->ui_data->list_store, &(iter), COL_DISPLAY_NAME,
            "desktop", COL_PIXBUF, image, -1);

        valid = gtk_tree_model_iter_next (GTK_TREE_MODEL (app_data->ui_data->list_store), &iter);

        g_print ("app_data->shared_data->thumbnail \n");
        icon = app_data->shared_data->thumbnail;

        gtk_list_store_append(app_data->ui_data->list_store, &(iter));
        gtk_list_store_set(app_data->ui_data->list_store, &(iter), COL_DISPLAY_NAME,
                  "OK", COL_PIXBUF, icon, -1);


    g_print ("fill_model : EXIT \n");
}

static GtkWidget *
create_view_and_model (AppData *app_data)
{
    GtkTreeModel   *model;
    GtkWidget      *view;

    view = gtk_icon_view_new();

    app_data->ui_data->list_store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, GDK_TYPE_PIXBUF);

    model = GTK_TREE_MODEL (app_data->ui_data->list_store);

    gtk_icon_view_set_model (GTK_ICON_VIEW (view), model);

  /* The tree view has acquired its own reference to the
   *  model, so we can drop ours. That way the model will
   *  be freed automatically when the tree view is destroyed */

    g_object_unref (model);

    return view;
}

/**
 * You have to start somewhere.
 *
 * @param argc The argument's count.
 * @param argv The arguments.
 *
 * @return The application return's code.
 */
int
main (int argc, char *argv[])
{
    const gchar *filename;
    AppData *app_data;

    /*
     g_thread_init (NULL);
     gtk_init (&argc, &argv);
     gdk_threads_init ();
     gst_init (&argc, &argv);
    */

    /* Initialize gstreamer. */
    gst_init (&argc, &argv);

    /* Initialize GTK+. */
    gtk_init (&argc, &argv);

    /* Create the app. */
    app_data = app_data_new ();

    /**********************************************/
    /* GST STUF DISPLAY ICONS*/

    chdir("/data/cheviewer-0.1/images");

    filename = "DAL_135.jpg";

    core_data_open_file (app_data->core_data, filename);

    core_data_start_display (app_data->core_data);

    /**********************************************/

    app_data->ui_data->iconview = create_view_and_model (app_data);

    gtk_container_add (GTK_CONTAINER (app_data->ui_data->scrolledwindow1), app_data->ui_data->iconview);

    gtk_icon_view_set_text_column(GTK_ICON_VIEW(app_data->ui_data->iconview),
             COL_DISPLAY_NAME);
    gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(app_data->ui_data->iconview),
               COL_PIXBUF);
    gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(app_data->ui_data->iconview),
               GTK_SELECTION_MULTIPLE);

    gtk_widget_show_all (app_data->ui_data->window1);


    /* Run the app. */
    gtk_main ();

    /* Clean up the app. */
    app_data_destroy (app_data);

    return 0;
}
