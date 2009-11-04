#ifndef __APP_H_
#define __APP_H_

typedef struct AppData_ AppData;

#include "core.h"
#include "shared.h"
#include "ui.h"

/**
 * Holds application data.
 */
struct AppData_
{
	CoreData *core_data; /**< The core data. */
	SharedData *shared_data; /**< The shared data. */
	UiData *ui_data; /**< The UI data. */
};

/**
 * Create application.
 *
 * @return The application data.
 */
AppData *app_data_new ();

/**
 * Destroy application.
 *
 * @param app_data The application data.
 */
void app_data_destroy (AppData *app_data);

#endif /* __APP_H_ */
