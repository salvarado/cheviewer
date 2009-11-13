#include "main.h"
#include "core.h"

#include <string.h>

static void cb_newpad (GstElement *element, GstPad *pad, gpointer cb_data);
static gboolean cb_gstbus (GstBus *bus, GstMessage *message, gpointer data);
void core_data_enable_lcdfullscreen (AppData * app_data);

CoreData *
core_data_new (AppData *app_data)
{
    CoreData *core_data;
        GstCaps *filecaps;
        GstBus *bus;
        GstBin *bin;

    core_data = g_new (CoreData, 1);

    core_data->pipeline = gst_pipeline_new ("pipeline");
        bus = gst_pipeline_get_bus(GST_PIPELINE(core_data->pipeline));
        gst_bus_add_watch(bus, cb_gstbus, app_data);
#if 0
        gst_object_unref (bus);
#endif
    core_data->filesrc = gst_element_factory_make ("filesrc", "source");

#ifdef USE_GSTOMX
    core_data->decoder=
            gst_element_factory_make ("omx_jpegdec", "decoder");

    core_data->imagesink =
            gst_element_factory_make ("v4lsink", "imagesink");

#else
    core_data->decoder =
            gst_element_factory_make ("jpegdec", "decoder");
    core_data->videocolorspace =
            gst_element_factory_make ("ffmpegcolorspace", "videocolorspace");
    core_data->vscale =
                gst_element_factory_make ("videoscale", "videoscale");
    core_data->filter =
                    gst_element_factory_make ("capsfilter", "filter");
    core_data->imagesink =
                gst_element_factory_make ("gdkpixbufsink", "imagesink");

#endif

    bin = GST_BIN (core_data->pipeline);
    gst_bin_add (bin, core_data->filesrc);
    gst_bin_add (bin, core_data->decoder);
    gst_bin_add (bin, core_data->videocolorspace);
    gst_bin_add (bin, core_data->vscale);
    gst_bin_add (bin, core_data->filter);
    gst_bin_add (bin, core_data->imagesink);

    gst_element_link_many (core_data->filesrc, core_data->decoder, core_data->videocolorspace,
            core_data->vscale, core_data->filter, core_data->imagesink, NULL);

    /*setting properties*/
    g_object_set (core_data->vscale, "method", 1, NULL);
    g_object_set (core_data->filter, "caps",
            gst_caps_new_simple ("video/x-raw-rgb",
                    "width", G_TYPE_INT, 128,
                    "height", G_TYPE_INT, 96,
                    NULL),
            NULL);

    gst_element_set_state (core_data->pipeline, GST_STATE_NULL);

    /* Set the default video output to LCD */
    /*core_data_set_lcdout (core_data);*/

    return core_data;
}

static gboolean
cb_gstbus (GstBus *bus, GstMessage *message, gpointer cb_data)
{
    AppData *app_data;

    app_data = cb_data;


    switch (GST_MESSAGE_TYPE (message))
    {
        case GST_MESSAGE_ERROR:
        {
            GError *err;
            gchar *debug;
            gst_message_parse_error (message, &err, &debug);
            g_print ("Error: %s\n, err->message");
            g_error_free (err);
            g_free (debug);
        }
        case GST_MESSAGE_EOS:
        {
            gst_element_unlink (app_data->core_data->decoder,
                    app_data->core_data->videocolorspace);
            gst_element_unlink (app_data->core_data->videocolorspace,
                    app_data->core_data->imagesink);
            gst_element_set_state (app_data->core_data->pipeline,
                GST_STATE_NULL);
        }
        case GST_MESSAGE_ELEMENT:
        {
            /*Waiting for a message "pixbuf" from gdkpixbufsink */
            if ( message->src == GST_OBJECT(app_data->core_data->imagesink))
            {
                if (message->structure != NULL)
                {
                    GstStructure *s;
                    s = message->structure;
                    if( strcmp(gst_structure_get_name (s), "pixbuf")== 0 )
                    {
                        /*Getting the pixbuf from gdkpixbufsink*/
                        g_object_get (G_OBJECT (app_data->core_data->imagesink),
                                "last-pixbuf", &(app_data->shared_data->thumbnail), NULL);

                        /*Fill the icon tree to iconview*/
                        fill_model (app_data);

                    }
                }
            }
        }
        default:
        /* unhandled message */
        break;
    }
    /*gst_message_unref (message);*/
    return TRUE;
}

static void
cb_newpad(GstElement *element, GstPad *pad, gpointer cb_data)
{
    AppData *app_data;
    gchar *name;
    GstElement *queue;
    app_data = cb_data;
    name = gst_pad_get_name (pad);
    if (g_ascii_strncasecmp (name, "image", 5) == 0)
    {
        gst_element_link_pads (element, name,
            app_data->core_data->decoder, "sink");
#ifdef USE_GSTOMX
        guint width;
        guint height;
        GstStructure *structure;
        GstCaps *caps = NULL;
        OmxArea *omx_area;

        caps = gst_pad_get_caps(pad);
        structure = gst_caps_get_structure (caps, 0);

        gst_structure_get_int (structure, "width", &width);
        gst_structure_get_int (structure, "height", &height);
 /* */
       /* if (width >= 640 & height >= 480)*/
       /*When we are open a image , it always should the showed in fullscreen*/
            core_data_enable_lcdfullscreen(app_data);

        app_data->ui_data->omx_area->pp_width = width;
        app_data->ui_data->omx_area->pp_height = height;

        /* Set the PP display to the width of the GtkWindow */
        {
            OmxArea *self;

            gint x;
            gint y;
            gint xscale;
            gint yscale;

            self = app_data->ui_data->omx_area;

            x = self->win_x + self->wid_x;
            y = self->win_y + self->wid_y;

            if (self->pp_width > 0 && self->pp_height > 0)
            {
                xscale = (self->wid_width * 100) / (gint)(self->pp_width);
                yscale = (self->wid_height * 100) / (gint)(self->pp_height);
            }

            if (xscale >= 50 && yscale <= 800 && yscale >= 50 && yscale <= 800)
            {
                g_object_set (self->listener,
                    "x-scale", xscale,
                    "y-scale", yscale,
                    NULL);
            }

        }
        gst_caps_unref (caps);
#endif
    }
    g_free(name);
}

void
core_data_destroy (CoreData *core_data)
{
    g_return_if_fail (core_data != NULL);

    gst_element_set_state (core_data->pipeline, GST_STATE_NULL);

    gst_object_unref (GST_OBJECT (core_data->pipeline));

    g_free (core_data);
}

void
core_data_open_file (CoreData *core_data, const gchar *filename)
{
    gst_element_set_state (core_data->pipeline, GST_STATE_NULL);
    g_object_set (G_OBJECT (core_data->filesrc), "location",
            filename, NULL);
}

int
core_data_start_display (CoreData *core_data)
{
    if (core_data->pipeline)
    {
        if (gst_element_set_state (core_data->pipeline, GST_STATE_PLAYING))
        {
            /* Success */
            return 1;
        }
        else
        {
            /* Error */
            return 0;
        }

    }
    else
    {
        /* Error */
        return 0;
    }
}

int
core_data_stop_display (CoreData *core_data)
{
    if (core_data->pipeline)
    {
        if (gst_element_set_state (core_data->pipeline, GST_STATE_NULL))
        {
            /* Success */
            return 1;
        }
        else
        {
            /* Error */
            return 0;
        }

    }
    else
    {
        /* Error */
        return 0;
    }
}

int
core_data_set_tvout (AppData *app_data)
{
    g_print ("setting tvout\n");
    system ("echo tv > /sys/class/display_control/omap_disp_control/video1");
    g_print ("tvout set\n");

#ifdef USE_GSTOMX
    CoreData *core_data = app_data->core_data;
    if (core_data->imagesink)
    {

    /* Set the PP display to the width of the GtkWindow */

        OmxArea *self;

        gint x;
        gint y;
        gint xscale;
        gint yscale;

        self = app_data->ui_data->omx_area;

        x = 640;
        y = 480;

        if (self->pp_width > 0 && self->pp_height > 0)
        {
            xscale = (x * 100) / (gint)(self->pp_width);
            yscale = (y * 100) / (gint)(self->pp_height);
        }

        if (xscale >= 50 && yscale <= 800 && yscale >= 50 && yscale <= 800)
        {

            g_object_set (G_OBJECT (core_data->imagesink),
                "rotation", 0,
                "x-pos",0,
                "y-pos",0,
                "x-scale", xscale,
                "y-scale", yscale,
                NULL);
        }
        else
        {
            g_object_set (G_OBJECT (core_data->imagesink),
                "rotation", 0,
                "x-pos",0,
                "y-pos",0,
                NULL);
        }
    }
#endif

}

int
core_data_set_lcdout (CoreData *core_data)
{
    system ("echo lcd > /sys/class/display_control/omap_disp_control/video1");
}

void core_data_enable_lcdfullscreen (AppData *app_data)
{
    CoreData *core_data = app_data->core_data;

    if (core_data->imagesink)
    {
#ifdef USE_GSTOMX
    /* Set the PP display to the width of the GtkWindow */

        OmxArea *self;

        gint x;
        gint y;
        gint xscale;
        gint yscale;

        self = app_data->ui_data->omx_area;

        x = 320;
        y = 240;

        if (self->pp_width > 0 && self->pp_height > 0)
        {
            xscale = (x * 100) / (gint)(self->pp_width);
            yscale = (y * 100) / (gint)(self->pp_height);
        }

        if (xscale >= 50 && yscale <= 800 && yscale >= 50 && yscale <= 800)
        {

            g_object_set (G_OBJECT (core_data->imagesink),
                "rotation", 90,
                "x-pos",0,
                "y-pos",0,
                "x-scale", xscale,
                "y-scale", yscale,
                NULL);
        }
        else
        {
            g_object_set (G_OBJECT (core_data->imagesink),
                "rotation", 90,
                "x-pos",0,
                   "y-pos",0,
                NULL);
        }
    }
#endif
    }
}

void core_data_disable_lcdfullscreen (AppData *app_data)
{

    CoreData *core_data = app_data->core_data;

    if (core_data->imagesink)
    {
            g_object_set (G_OBJECT (core_data->imagesink),
                "rotation", 0, NULL);
    }
}
