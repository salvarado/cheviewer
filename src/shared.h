#ifndef __SHARED_H_
#define __SHARED_H_

#include <glib.h>
#include <gtk/gtk.h>

typedef struct SharedData_ SharedData;


#include "app.h"

/**
 * Holds shared data between the UI and core.
 */
struct SharedData_
{
    int filler;
    void *thumbnail;
};


/**
 * Creates shared data holder.
 * 
 * @param app_data The application data.
 * 
 * @return The new shared data holder.
 */
SharedData *shared_data_new (AppData *app_data);

/**
 * Destroys the shared data holder.
 * 
 * @param shared_data The shared data.
 */
void shared_data_destroy (SharedData *shared_data);

#endif /* __SHARED_H_ */
