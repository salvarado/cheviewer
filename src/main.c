
#include "main.h"

#include <assert.h>

enum
{
  COL_DISPLAY_NAME,
  COL_PIXBUF,
  NUM_COLS
};


void create_list_dir (SharedData *shared_data);
void fill_model (AppData *app_data);
static GtkWidget *create_view_and_model (AppData *app_data);

int
main (int argc, char *argv[])
{
    AppData *app_data;

    gint   ind_valid=1;
    const gchar *filename;

    /* Initialize gstreamer. */
    g_thread_init (NULL);
    gst_init (&argc, &argv);

    /* Initialize GTK+. */
    gdk_threads_init ();
    gtk_init (&argc, &argv);

    /* Create the app. */
    app_data = app_data_new ();

    /*
    We begin to open the path directory and generate the list of files,
    images are decoding and we got the tree of thumbnails
    */
    app_data->shared_data->path= "/data/cheviewer-0.1/images/";

    app_data->shared_data->done = TRUE;

    create_list_dir(app_data->shared_data);


    while ((app_data->shared_data->done == TRUE) )//  &&
           //(ind_valid < app_data->shared_data->index))
    {
        g_print("main GST_STATE: %d \n",GST_ELEMENT(app_data->core_data->filesrc)->current_state);

        /*Needed to create a signal and conect to a callback funtion*/
        app_data->shared_data->done = FALSE;

        app_data->shared_data->image_name = g_array_index (app_data->shared_data->file_names, gchar *, ind_valid);

        filename = app_data->shared_data->image_name;

        chdir(app_data->shared_data->path);

        core_data_open_file (app_data->core_data, filename);

        core_data_start_display (app_data->core_data);

        g_print("main start display exit\n");

        ind_valid ++;
    }

    /*g_array_free (app_data->shared_data->file_names, TRUE);*/

    /**********************************************/

    app_data->ui_data->iconview = create_view_and_model (app_data);

    gtk_container_add (GTK_CONTAINER (app_data->ui_data->scrolledwindow1), app_data->ui_data->iconview);

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

void
create_list_dir (SharedData *shared_data)
{
    gchar *filen;

    shared_data->index = 0;

    shared_data->name_dir = opendir (shared_data->path);

    if (shared_data->name_dir != NULL)
    {
        shared_data->file_names = g_array_new (FALSE, TRUE, sizeof(gchar *));
        while ((shared_data->st_dir = readdir (shared_data->name_dir)) != NULL)
        {
            filen= (shared_data->st_dir)->d_name;

            if ( ( strstr(filen, ".jpg") != NULL) ||
                 ( strstr(filen, ".JPG") != NULL) )
            {
                g_array_append_val(shared_data->file_names, filen );
                shared_data->index++;
                g_printf("%s : %d\n", filen, shared_data->index );

            }
            else
            {
                g_printf("NO VALIDO: %s : %d\n", filen, shared_data->index );
            }
        }
        if ( closedir(shared_data->name_dir) == -1)
        {
            g_printf("Error trying to close the directory\n");
        }
    }
}

void
fill_model (AppData *app_data)
{
    GtkTreeIter    iter;
    GdkPixbuf *icon;
    GError *err = NULL;

        icon = app_data->shared_data->thumbnail;

        gtk_list_store_append(app_data->ui_data->list_store, &(iter));
        gtk_list_store_set(app_data->ui_data->list_store, &(iter), COL_DISPLAY_NAME,
                  "OK", COL_PIXBUF, icon, -1);

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
