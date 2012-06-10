/**
 * Copyright (C) 2012 hejian <hejian.he@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#define PLUG_IN_PROC   "plug-in-beautify"
#define PLUG_IN_BINARY "beautify"
#define PLUG_IN_ROLE   "gimp-beautify"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

typedef struct
{
  gint brightness;
  gint contrast;
  gdouble saturation;
  gdouble sharpeness;
} BeautifyValues;

static void     query    (void);
static void     run      (const gchar      *name,
                          gint              nparams,
                          const GimpParam  *param,
                          gint             *nreturn_vals,
                          GimpParam       **return_vals);

static void     beautify (GimpDrawable *drawable);

static gboolean beautify_dialog (gint32        image_ID,
                                 GimpDrawable *drawable);

static void     create_base_page (GtkNotebook *notebook);

static void     brightness_update (GtkRange *range, gpointer data);
static void     contrast_update   (GtkRange *range, gpointer data);

static void     adjustment();

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

static BeautifyValues bvals =
{
  0,  /* brightness */
  0,  /* contrast   */
  0,  /* saturation */
  0,  /* sharpeness */
};

static gint32     image_ID         = 0;
static gint       width;
static gint       height;

static GtkWidget *preview          = NULL;
static gint32     preview_image    = 0;

MAIN ()

static void
query (void)
{
  static const GimpParamDef args[] =
  {
    { GIMP_PDB_INT32,    "run-mode",   "The run mode { RUN-INTERACTIVE (0), RUN-NONINTERACTIVE (1) }" },
    { GIMP_PDB_IMAGE,    "image",      "Input image" },
    { GIMP_PDB_DRAWABLE, "drawable",   "Input drawable" },
  };

  gimp_install_procedure (PLUG_IN_PROC,
                          "Beautify, most easiest way for beautify photo.",
                          "Beautify, most easiest way for beautify photo.",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Beautify...",
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args), 0,
                          args, NULL);

  gimp_plugin_menu_register (PLUG_IN_PROC, "<Image>/Tools/Beautify");
}

static void
run (const gchar      *name,
     gint              nparams,
     const GimpParam  *param,
     gint             *nreturn_vals,
     GimpParam       **return_vals)
{
  static GimpParam   values[2];
  GimpDrawable      *drawable;
  GimpRunMode        run_mode;
  GimpPDBStatusType  status = GIMP_PDB_SUCCESS;

  run_mode = param[0].data.d_int32;

  *nreturn_vals = 1;
  *return_vals  = values;

  values[0].type          = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  image_ID = param[1].data.d_image;
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  width = gimp_image_width (image_ID);
  height = gimp_image_height (image_ID);

  switch (run_mode)
  {
    case GIMP_RUN_INTERACTIVE:
      if (! beautify_dialog (image_ID, drawable))
        return;
      break;

    case GIMP_RUN_NONINTERACTIVE:
      break;

    case GIMP_RUN_WITH_LAST_VALS:
      break;

    default:
      break;
  }

  if ((status == GIMP_PDB_SUCCESS) &&
      (gimp_drawable_is_rgb(drawable->drawable_id) ||
       gimp_drawable_is_gray(drawable->drawable_id)))
    {
      /* Run! */
      gimp_image_undo_group_start (image_ID);
      beautify (drawable);
      gimp_image_undo_group_end (image_ID);

      /* If run mode is interactive, flush displays */
      if (run_mode != GIMP_RUN_NONINTERACTIVE)
        gimp_displays_flush ();

      /* Store data */
      if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data (PLUG_IN_PROC, &bvals, sizeof (BeautifyValues));

    }

  gimp_drawable_detach (drawable);
}

static void
beautify (GimpDrawable *drawable)
{
  if (bvals.brightness != 0 || bvals.contrast != 0) {
    gimp_brightness_contrast (drawable->drawable_id, bvals.brightness, bvals.contrast);
  }
}

static void
preview_update (GtkWidget *preview)
{
  gint preview_size = PREVIEW_SIZE;
  gint max_size = height;
  if (height < width)
    max_size = width;
  if (preview_size > max_size)
    preview_size = max_size;
  GdkPixbuf *pixbuf = gimp_image_get_thumbnail (preview_image, preview_size, preview_size, GIMP_PIXBUF_SMALL_CHECKS);
  gtk_image_set_from_pixbuf (GTK_IMAGE(preview), pixbuf);
}

static gboolean
beautify_dialog (gint32        image_ID,
                 GimpDrawable *drawable)
{
  GtkWidget *dialog;
  GtkWidget *main_hbox;
  GtkWidget *left_vbox;
  GtkWidget *middle_vbox;
  GtkWidget *right_vbox;
  GtkWidget *label;

  gimp_ui_init (PLUG_IN_BINARY, FALSE);

  dialog = gimp_dialog_new ("Beautify", PLUG_IN_ROLE,
                            NULL, 0,
                            gimp_standard_help_func, PLUG_IN_PROC,

                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,

                            NULL);

  gimp_window_set_transient (GTK_WINDOW (dialog));

  gtk_widget_show (dialog);

  main_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (main_hbox), 12);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))),
                      main_hbox, TRUE, TRUE, 0);
  gtk_widget_show (main_hbox);

  left_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (left_vbox), 12);
  gtk_box_pack_start (GTK_BOX (main_hbox), left_vbox, TRUE, TRUE, 0);
  gtk_widget_show (left_vbox);

  middle_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (middle_vbox), 12);
  gtk_box_pack_start (GTK_BOX (main_hbox), middle_vbox, TRUE, TRUE, 0);
  gtk_widget_show (middle_vbox);

  right_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (right_vbox), 12);
  gtk_box_pack_start (GTK_BOX (main_hbox), right_vbox, TRUE, TRUE, 0);
  gtk_widget_show (right_vbox);

  /* adjustment */
  GtkWidget *notebook = gtk_notebook_new ();
  gtk_box_pack_start (GTK_BOX (left_vbox), notebook, FALSE, FALSE, 0);
  gtk_widget_show (notebook);

  create_base_page (GTK_NOTEBOOK (notebook));

  /* preview */
  label = gtk_label_new ("Preview");
  gtk_box_pack_start (GTK_BOX (middle_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  preview_image = gimp_image_duplicate(image_ID);

  preview = gtk_image_new();
  preview_update (preview);

  gtk_box_pack_start (GTK_BOX (middle_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  gboolean run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}

static void
create_base_page (GtkNotebook *notebook) {
  GtkWidget *label;
  GtkWidget *hscale;

  GtkWidget *pagelabel = gtk_label_new ("Basic");

  GtkWidget *thispage = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (thispage), 12);
  gtk_widget_show (thispage);

  /* brightness */
  label = gtk_label_new ("Brightness");
  gtk_box_pack_start (GTK_BOX (thispage), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  hscale = gtk_hscale_new_with_range (-127, 127, 1);
  gtk_range_set_value (GTK_RANGE (hscale), bvals.brightness);
  gtk_scale_set_value_pos (GTK_SCALE (hscale), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), hscale, FALSE, FALSE, 0);
  gtk_widget_show (hscale);

  g_signal_connect (hscale, "value-changed",
                   G_CALLBACK (brightness_update),
                   NULL);

  /* contrast */
  label = gtk_label_new ("Contrast");
  gtk_box_pack_start (GTK_BOX (thispage), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  hscale = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (hscale), bvals.contrast);
  gtk_scale_set_value_pos (GTK_SCALE (hscale), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), hscale, FALSE, FALSE, 0);
  gtk_widget_show (hscale);

  g_signal_connect (hscale, "value-changed",
                   G_CALLBACK (contrast_update),
                   NULL);

  gtk_notebook_append_page_menu (notebook, thispage, pagelabel, NULL);
}

static void
brightness_update (GtkRange *range, gpointer data) {
  bvals.brightness = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
contrast_update (GtkRange *range, gpointer data) {
  bvals.contrast = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
adjustment () {
  if (bvals.brightness != 0 || bvals.contrast != 0) {
    preview_image = gimp_image_duplicate (image_ID);
    gint32 layer = gimp_image_get_active_layer (preview_image);
    gimp_brightness_contrast (layer, bvals.brightness, bvals.contrast);
  }
}

