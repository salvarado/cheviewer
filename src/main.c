#include <gtk/gtk.h>
#include <gst/gst.h>

#include "main.h"
#include "app.h"

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
	AppData *app_data;

	/* Initialize gstreamer. */
	gst_init (&argc, &argv);

	/* Initialize GTK+. */
	gtk_init (&argc, &argv);

	/* Create the app. */
	app_data = app_data_new ();

	/* Run the app. */
	gtk_main ();

	/* Clean up the app. */
	app_data_destroy (app_data);

	return 0;
}
