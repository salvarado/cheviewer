#ifndef __OMX_AREA_H_
#define __OMX_AREA_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define OMX_TYPE_AREA (omx_area_get_type ())
#define OMX_AREA(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), OMX_TYPE_AREA, OmxArea))

typedef struct _OmxArea OmxArea;
typedef struct _OmxAreaClass OmxAreaClass;

struct _OmxArea
{
    GtkDrawingArea parent_object;

    GObject *listener; /**< The object that listens to the widget's status changes. */

    gint win_x; /**< The window's x position. */
    gint win_y; /**< The window's y position. */
    gint wid_x; /**< The widget's x position. */
    gint wid_y; /**< The widget's y position. */
    gint wid_width; /**< The widget's width. */
    gint wid_height; /**< The widget's height. */
    GdkPixmap *pixmap; /**< The widget's pixmap. */
    gint pp_width;
    gint pp_height;

};

struct _OmxAreaClass
{
    GtkDrawingAreaClass parent_class;
};

/**
 * Creates a new OmxArea.
 *
 * @return The new OmxArea.
 */
OmxArea *omx_area_new (void);

/**
 * Sets the OmxArea listener.
 *
 * The listener is a @c GObject that receives @c OmxArea's status changes, specifically,
 * @c Omap2VideoSink.
 *
 * @param omx_area The OmxArea.
 * @param object The object that receives status changes.
 */
void omx_area_set_listener (OmxArea *omx_area, GObject *object);

/**
 * Sets the OmxArea color key.
 *
 * @param omx_area The OmxArea.
 * @param colorkey The colorkey (24bit RGB).
 */
void omx_area_set_color (OmxArea *omx_area, guint color);

G_END_DECLS

#endif /* __OMX_AREA_H_ */
