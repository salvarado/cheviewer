#include "main.h"
#include "shared.h"

SharedData *
shared_data_new (AppData *app_data)
{
    SharedData *shared_data;

    shared_data = g_new (SharedData, 1);

    return shared_data;
}

void
shared_data_destroy (SharedData *shared_data)
{
    g_return_if_fail (shared_data != NULL);

    g_free (shared_data);
}
