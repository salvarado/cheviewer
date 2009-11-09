#include <gtk/gtk.h>

#include "omx_area.h"

static OmxAreaClass *parent_class = NULL;

G_DEFINE_TYPE (OmxArea, omx_area, GTK_TYPE_DRAWING_AREA);

static void
changed (GtkWidget *widget)
{
    OmxArea *self;

    gint x;
    gint y;
    gint xscale;
    gint yscale;

    self = OMX_AREA (widget);

    x = self->win_x + self->wid_x;
    y = self->win_y + self->wid_y;

    if (self->pp_width > 0 && self->pp_height > 0)
    {
        xscale = (self->wid_width * 100) / (gint)(self->pp_width);
        yscale = (self->wid_height * 100) / (gint)(self->pp_height);
    }
    else
    {
        xscale = 0;
        yscale = 0;
    }
    if (self->listener != NULL)
    {
        g_object_set (self->listener,
            "x-pos", x,
            "y-pos", y,
            NULL);

        if (xscale >= 50 && yscale <= 800 && yscale >= 50 && yscale <= 800)
        {
            g_object_set (self->listener,
                "x-scale", xscale,
                "y-scale", yscale,
                NULL);
        }


    }
}

static gboolean
win_configure (GtkWidget *widget,
    GdkEventConfigure *event,
    gpointer user_data)
{
    OmxArea *self;

    self = OMX_AREA (user_data);

#ifdef DEBUG
    g_message ("win_configure");
#endif

    self->win_x = event->x;
    self->win_y = event->y;

    changed (GTK_WIDGET (self));

    return FALSE;
}

static gboolean
wid_configure (GtkWidget *widget,
    GdkEventConfigure *event)
{
    OmxArea *self;

    self = OMX_AREA (widget);

#ifdef DEBUG
    g_message ("wid_configure");
#endif

    self->wid_x = event->x;
    self->wid_y = event->y;
    self->wid_width = event->width;
    self->wid_height = event->height;

    if (self->pixmap)
        g_object_unref (self->pixmap);

    self->pixmap = gdk_pixmap_new (widget->window,
        event->width,
        event->height,
        -1);

    gdk_draw_rectangle (self->pixmap,
        widget->style->bg_gc[GTK_STATE_NORMAL],
        TRUE,
        0, 0,
        event->width, event->height);

    changed (GTK_WIDGET (self));

    return FALSE;
}

static void
parent_set (GtkWidget *widget,
    GtkWidget *previous_parent)
{
    GtkWidget *toplevel;
        OmxArea *self;

    toplevel = gtk_widget_get_toplevel (widget);
        self = OMX_AREA (widget);

    if (toplevel != NULL)
    {
        GtkWindow *window;

        if (GTK_IS_WINDOW (toplevel))
        {
            window = GTK_WINDOW (toplevel);
            gtk_signal_connect (GTK_OBJECT (window),
                "configure_event",
                                GTK_SIGNAL_FUNC (win_configure),
                widget);
        }
    }
}

static gboolean
expose (GtkWidget *widget, GdkEventExpose *event)
{
    OmxArea *self = OMX_AREA (widget);

    gdk_draw_drawable (widget->window,
        widget->style->bg_gc[GTK_STATE_NORMAL],
        self->pixmap,
        event->area.x, event->area.y,
        event->area.x, event->area.y,
        event->area.width, event->area.height);

    return FALSE;
}

static void
finalize (GObject *obj)
{
    OmxArea *self = OMX_AREA (obj);

    if (self->listener != NULL)
        g_object_unref (self->listener);

    if (self->pixmap != NULL)
        g_object_unref (self->pixmap);

    /* Chain up to the parent class */
    G_OBJECT_CLASS (parent_class)->finalize (obj);
}

static void
omx_area_class_init (OmxAreaClass *class)
{
    parent_class = g_type_class_peek_parent (class);

    {
        GtkWidgetClass *widget_class;

        widget_class = GTK_WIDGET_CLASS (class);

        widget_class->parent_set = parent_set;
        widget_class->configure_event = wid_configure;

        widget_class->expose_event = expose;
    }

    {
        GObjectClass *gobject_class = G_OBJECT_CLASS (class);

        gobject_class->finalize = finalize;
    }
}

static void
omx_area_init (OmxArea *self)
{
}

OmxArea *
omx_area_new (void)
{
    return g_object_new (OMX_TYPE_AREA, NULL);
}

void
omx_area_set_listener (OmxArea *omx_area, GObject *object)
{
    if (omx_area->listener != NULL)
        g_object_unref (omx_area->listener);
    if (object != NULL)
    g_object_ref (object);
    omx_area->listener = object;
    changed (GTK_WIDGET (omx_area));
}

void
omx_area_set_color (OmxArea *omx_area, guint colorkey)
{
    GdkColor color;

    color.red = ((colorkey & 0xFF0000) >> 0) * 0x101;
    color.green = ((colorkey & 0x00FF00) >> 8) * 0x101;
    color.blue = ((colorkey & 0x0000FF) >> 16) * 0x101;

#ifdef DEBUG
    g_print ("red = %X\n", color.red);
    g_print ("green = %X\n", color.green);
    g_print ("blue = %X\n", color.blue);
#endif /* DEBUG */

    gtk_widget_modify_bg (GTK_WIDGET (omx_area), GTK_STATE_NORMAL, &color);

    if (omx_area->listener)
    {
        /* The 16 bit colorkey. */
        guint new_colorkey;

        new_colorkey = (color.red >> (16 - 5) << 11 )|
            (color.green >> (16 - 6) << 5 ) |
            (color.blue >> (16 - 5) << 0);

#ifdef DEBUG
        g_print ("colorkey = %X\n", new_colorkey);
#endif /* DEBUG */

        g_object_set (omx_area->listener, "colorkey", new_colorkey, NULL);
    }
}
