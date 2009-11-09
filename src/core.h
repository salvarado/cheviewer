#ifndef __CORE_H_
#define __CORE_H_

#include <gst/gst.h>

typedef struct CoreData_ CoreData;

#include "app.h"

#define VIDEOSINK_LCDFULLSCREEN_ROTATION 90
#define VIDEOSINK_LCDFULLSCREEN_TOP 0
#define VIDEOSINK_LCDFULLSCREEN_LEFT 0
#define VIDEOSINK_LCDFULLSCREEN_WIDTH 320
#define VIDEOSINK_LCDFULLSCREEN_HEIGHT 240
#define VIDEOSINK_DEFAULT_TOP 88
#define VIDEOSINK_DEFAULT_LEFT 0
#define VIDEOSINK_TVOUT_ROTATION 0
#define VIDEOSINK_TVOUT_TOP 0
#define VIDEOSINK_TVOUT_LEFT 0
#define VIDEOSINK_TVOUT_WIDTH 320
#define VIDEOSINK_TVOUT_HEIGHT 240

#undef USE_GSTOMX

/**
 * Holds core data (gstreamer stuff).
 */
struct CoreData_
{
    GstElement *pipeline; /**< The main pipeline. */
    GstElement *filesrc; /**< The source element. */
    GstElement *decoder; /**< The image decoder element. */
    GstElement *videocolorspace; /**< The video colorspace element. */
    GstElement *imagesink; /**< The image sink element. */
    gboolean lcdfullscreen; /**< TRUE if doing playback in fullscreen*/
};

/**
 * Creates the core.
 * 
 * @param app_data The application data.
 * 
 * @return Thew new core data.
 */
CoreData *core_data_new (AppData *app_data);

/**
 * Destroys the core.
 * 
 * @param core_data The core data.
 */
void core_data_destroy (CoreData *core_data);

/**
 * Open file.
 * 
 * Opens a file and sets up gstreamer to read it.
 * 
 * @param core_data The core data.
 * @param filename The name of the file to open.
 */
void core_data_open_file (CoreData *core_data, const gchar *filename);

#endif /* __CORE_H_ */
