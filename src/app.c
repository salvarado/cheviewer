#include "main.h"
#include "app.h"

AppData *
app_data_new ()
{
	AppData *app_data;

	app_data = g_new (AppData, 1);

	app_data->core_data = core_data_new (app_data);
	app_data->shared_data = shared_data_new (app_data);
	app_data->ui_data = ui_data_new (app_data);


	return app_data;
}

void
app_data_destroy (AppData *app_data)
{
	ui_data_destroy (app_data->ui_data);
	shared_data_destroy (app_data->shared_data);
	core_data_destroy (app_data->core_data);

	g_free (app_data);
}
