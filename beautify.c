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

#define TEXTURE_PATH   "textures/effects"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

typedef struct
{
  gint brightness;
  gint contrast;
  gdouble saturation;
  gdouble sharpeness;
  gdouble hue;
  gdouble cyan_red;
  gdouble magenta_green;
  gdouble yellow_blue;
} BeautifyValues;

typedef enum
{
  BEAUTIFY_EFFECT_NONE,
  BEAUTIFY_EFFECT_SOFT_LIGHT,
  BEAUTIFY_EFFECT_WARM,
  BEAUTIFY_EFFECT_SHARPEN,
  BEAUTIFY_EFFECT_SMART_COLOR,
  BEAUTIFY_EFFECT_INVERT,
  BEAUTIFY_EFFECT_GOTHIC_STYLE,
  BEAUTIFY_EFFECT_LITTLE_FRESH,
  BEAUTIFY_EFFECT_ABAO,
  BEAUTIFY_EFFECT_ICE_SPIRIT,
  BEAUTIFY_EFFECT_JAPANESE,
  BEAUTIFY_EFFECT_NEW_JAPANESE,
  BEAUTIFY_EFFECT_ASTRAL,
} BeautifyEffectType;

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
static void     create_color_page (GtkNotebook *notebook);

static void     brightness_update    (GtkRange *range, gpointer data);
static void     contrast_update      (GtkRange *range, gpointer data);
static void     saturation_update    (GtkRange *range, gpointer data);
static void     hue_update           (GtkRange *range, gpointer data);
static void     cyan_red_update      (GtkRange *range, gpointer data);
static void     magenta_green_update (GtkRange *range, gpointer data);
static void     yellow_blue_update   (GtkRange *range, gpointer data);

static void     adjustment();

static void create_effect_pages (GtkNotebook *notebook);
static GtkWidget* effect_icon_new (BeautifyEffectType effect);

static gboolean select_effect (GtkWidget *widget, GdkEvent *event, gpointer user_data);

static void do_effect (gint32 image, BeautifyEffectType effect);
static void reset_adjustment ();
static void apply_effect ();

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
  0,  /* hue */
  0,  /* cyan_red */
  0,  /* magenta_green */
  0,  /* yellow_blue */
};

static gint32     image_ID         = 0;
static gint       width;
static gint       height;

static GtkWidget *brightness = NULL;
static GtkWidget *contrast = NULL;
static GtkWidget *saturation = NULL;
static GtkWidget *sharpeness = NULL;
static GtkWidget *hue = NULL;
static GtkWidget *cyan_red = NULL;
static GtkWidget *magenta_green = NULL;
static GtkWidget *yellow_blue = NULL;

static GtkWidget *preview          = NULL;
static gint32     preview_image    = 0;
static gint32     saved_image      = 0;

static BeautifyEffectType current_effect = BEAUTIFY_EFFECT_NONE;
gint32 preview_effect_layer = 0;

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
                          "Beautify, the easiest way to beautify photo.",
                          "Beautify, the easiest way to beautify photo.",
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
  apply_effect ();
  gint32 source = gimp_image_get_active_layer (preview_image);
  gimp_edit_copy (source);
  gint32 floating_sel = gimp_edit_paste (drawable->drawable_id, FALSE);
  gimp_floating_sel_anchor (floating_sel);
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
  create_color_page (GTK_NOTEBOOK (notebook));

  /* preview */
  /*label = gtk_label_new ("Preview");
  gtk_box_pack_start (GTK_BOX (middle_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);*/

  preview_image = gimp_image_duplicate(image_ID);

  preview = gtk_image_new();
  preview_update (preview);

  gtk_box_pack_start (GTK_BOX (middle_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  /* effects */
  notebook = gtk_notebook_new ();
  gtk_box_pack_start (GTK_BOX (right_vbox), notebook, FALSE, FALSE, 0);
  gtk_widget_show (notebook);

  create_effect_pages (GTK_NOTEBOOK (notebook));

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

  brightness = gtk_hscale_new_with_range (-127, 127, 1);
  gtk_range_set_value (GTK_RANGE (brightness), bvals.brightness);
  gtk_scale_set_value_pos (GTK_SCALE (brightness), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), brightness, FALSE, FALSE, 0);
  gtk_widget_show (brightness);

  g_signal_connect (brightness, "value-changed",
                   G_CALLBACK (brightness_update),
                   NULL);

  /* contrast */
  label = gtk_label_new ("Contrast");
  gtk_box_pack_start (GTK_BOX (thispage), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  contrast = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (contrast), bvals.contrast);
  gtk_scale_set_value_pos (GTK_SCALE (contrast), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), contrast, FALSE, FALSE, 0);
  gtk_widget_show (contrast);

  g_signal_connect (contrast, "value-changed",
                   G_CALLBACK (contrast_update),
                   NULL);

  /* saturation */
  label = gtk_label_new ("Saturation");
  gtk_box_pack_start (GTK_BOX (thispage), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  saturation = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (saturation), bvals.saturation);
  gtk_scale_set_value_pos (GTK_SCALE (saturation), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), saturation, FALSE, FALSE, 0);
  gtk_widget_show (saturation);

  g_signal_connect (saturation, "value-changed",
                   G_CALLBACK (saturation_update),
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
saturation_update (GtkRange *range, gpointer data) {
  bvals.saturation = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
create_color_page (GtkNotebook *notebook) {
  GtkWidget *label;
  GtkWidget *hscale;

  GtkWidget *pagelabel = gtk_label_new ("Color");

  GtkWidget *thispage = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (thispage), 12);
  gtk_widget_show (thispage);

  /* hue */
  label = gtk_label_new ("Hue");
  gtk_box_pack_start (GTK_BOX (thispage), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  hue = gtk_hscale_new_with_range (-180, 180, 1);
  gtk_range_set_value (GTK_RANGE (hue), bvals.hue);
  gtk_scale_set_value_pos (GTK_SCALE (hue), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (thispage), hue, FALSE, FALSE, 0);
  gtk_widget_show (hue);

  g_signal_connect (hue, "value-changed",
                   G_CALLBACK (hue_update),
                   NULL);

  GtkWidget *table = gtk_table_new (3, 3, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_box_pack_start (GTK_BOX (thispage), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  /* cyan_red */
  GtkWidget *event_box;
  GdkColor color;

  event_box = gtk_event_box_new ();
  color.red = 0x0; color.green = 0xFFFF; color.blue = 0xFFFF;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("C");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 0, 1, 0, 1);
  gtk_widget_show (event_box);

  cyan_red = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (cyan_red), bvals.cyan_red);
  gtk_scale_set_value_pos (GTK_SCALE (cyan_red), GTK_POS_BOTTOM);
  gtk_table_attach_defaults (GTK_TABLE (table), cyan_red, 1, 2, 0, 1);
  gtk_widget_show (cyan_red);

  gtk_widget_set_size_request (cyan_red, 100, -1);

  g_signal_connect (cyan_red, "value-changed",
                   G_CALLBACK (cyan_red_update),
                   NULL);

  event_box = gtk_event_box_new ();
  color.red = 0xFFFF; color.green = 0x0; color.blue = 0x0;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("R");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 2, 3, 0, 1);
  gtk_widget_show (event_box);

  /* magenta_green */
  event_box = gtk_event_box_new ();
  color.red = 0xFFFF; color.green = 0x0; color.blue = 0xFFFF;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("M");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 0, 1, 1, 2);
  gtk_widget_show (event_box);

  magenta_green = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (magenta_green), bvals.magenta_green);
  gtk_scale_set_value_pos (GTK_SCALE (magenta_green), GTK_POS_BOTTOM);
  gtk_table_attach_defaults (GTK_TABLE (table), magenta_green, 1, 2, 1, 2);
  gtk_widget_show (magenta_green);

  g_signal_connect (magenta_green, "value-changed",
                   G_CALLBACK (magenta_green_update),
                   NULL);

  event_box = gtk_event_box_new ();
  color.red = 0x0; color.green = 0xFFFF; color.blue = 0x0;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("G");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 2, 3, 1, 2);
  gtk_widget_show (event_box);

  /* yellow_blue */
  event_box = gtk_event_box_new ();
  color.red = 0xFFFF; color.green = 0xFFFF; color.blue = 0x0;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("Y");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 0, 1, 2, 3);
  gtk_widget_show (event_box);

  yellow_blue = gtk_hscale_new_with_range (-50, 50, 1);
  gtk_range_set_value (GTK_RANGE (yellow_blue), bvals.yellow_blue);
  gtk_scale_set_value_pos (GTK_SCALE (yellow_blue), GTK_POS_BOTTOM);
  gtk_table_attach_defaults (GTK_TABLE (table), yellow_blue, 1, 2, 2, 3);
  gtk_widget_show (yellow_blue);

  g_signal_connect (yellow_blue, "value-changed",
                   G_CALLBACK (yellow_blue_update),
                   NULL);

  event_box = gtk_event_box_new ();
  color.red = 0x0; color.green = 0x0; color.blue = 0xFFFF;
  gtk_widget_modify_bg (event_box, 0, &color);

  label = gtk_label_new ("B");
  gtk_container_add (GTK_CONTAINER (event_box), label);
  gtk_widget_show (label);
  gtk_table_attach_defaults (GTK_TABLE (table), event_box, 2, 3, 2, 3);
  gtk_widget_show (event_box);

  gtk_notebook_append_page_menu (notebook, thispage, pagelabel, NULL);
}

static void
hue_update (GtkRange *range, gpointer data) {
  bvals.hue = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
cyan_red_update (GtkRange *range, gpointer data) {
  bvals.cyan_red = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
magenta_green_update (GtkRange *range, gpointer data) {
  bvals.magenta_green = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}

static void
yellow_blue_update (GtkRange *range, gpointer data) {
  bvals.yellow_blue = gtk_range_get_value (range);
  adjustment ();
  preview_update (preview);
}
static void
adjustment () {
  if (bvals.brightness == 0 && bvals.contrast == 0 && bvals.saturation == 0 && bvals.hue == 0 && bvals.cyan_red == 0 && bvals.magenta_green == 0 && bvals.yellow_blue == 0) {
    return;
  }

  apply_effect ();

  if (!saved_image) {
    saved_image = gimp_image_duplicate (preview_image);
  }

  preview_image = gimp_image_duplicate (saved_image);
  gint32 layer = gimp_image_get_active_layer (preview_image);

  if (bvals.brightness != 0 || bvals.contrast != 0) {
    gint low_input = 0;
    gint high_input = 255;
    gint low_output = 0;
    gint high_output = 255;
    if (bvals.brightness > 0) {
      high_input -= bvals.brightness;
    }
    if (bvals.brightness < 0) {
      high_output += bvals.brightness;
    }
    if (bvals.contrast > 0) {
      low_input += bvals.contrast;
      high_input -= bvals.contrast;
    }
    if (bvals.contrast < 0) {
      low_output -= bvals.contrast;
      high_output += bvals.contrast;
    }
    gimp_levels (layer, GIMP_HISTOGRAM_VALUE,
                 low_input, high_input,
                 1,
                 low_output, high_output);
  }

  if (bvals.saturation != 0 || bvals.hue) {
    gimp_hue_saturation (layer, GIMP_ALL_HUES, bvals.hue, 0, bvals.saturation);
  }

  if (bvals.cyan_red != 0 || bvals.magenta_green != 0 || bvals.yellow_blue != 0) {
    gimp_color_balance (layer, GIMP_SHADOWS, TRUE,
                        bvals.cyan_red, bvals.magenta_green, bvals.yellow_blue);
    gimp_color_balance (layer, GIMP_MIDTONES, TRUE,
                        bvals.cyan_red, bvals.magenta_green, bvals.yellow_blue);
    gimp_color_balance (layer, GIMP_HIGHLIGHTS, TRUE,
                        bvals.cyan_red, bvals.magenta_green, bvals.yellow_blue);
  }
}

static void
create_effect_pages (GtkNotebook *notebook) {
  GtkWidget *pagelabel = gtk_label_new ("Basic");

  GtkWidget *thispage = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (thispage), 12);
  gtk_widget_show (thispage);

  BeautifyEffectType effects[] =
  {
    BEAUTIFY_EFFECT_SOFT_LIGHT,
    BEAUTIFY_EFFECT_WARM,
    BEAUTIFY_EFFECT_SHARPEN,
    BEAUTIFY_EFFECT_SMART_COLOR,
    BEAUTIFY_EFFECT_INVERT,
    BEAUTIFY_EFFECT_GOTHIC_STYLE,
    BEAUTIFY_EFFECT_LITTLE_FRESH,
    BEAUTIFY_EFFECT_ABAO,
    BEAUTIFY_EFFECT_ICE_SPIRIT,
    BEAUTIFY_EFFECT_JAPANESE,
    BEAUTIFY_EFFECT_NEW_JAPANESE,
    BEAUTIFY_EFFECT_ASTRAL,
  };

  /* table */
  gint rows = 5;
  gint cols = 3;
  GtkWidget *table = gtk_table_new (rows, cols, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_box_pack_start (GTK_BOX (thispage), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  gint row = 1;
  gint col = 1;

  gint i;
  for (i = 0; i < G_N_ELEMENTS (effects); i++) {
    GtkWidget *icon = effect_icon_new (effects[i]);
    //gtk_box_pack_start (GTK_BOX (thispage), icon, FALSE, FALSE, 0);
    gtk_table_attach_defaults (GTK_TABLE (table), icon, col - 1, col, row - 1, row);
    gtk_widget_show (icon);

    col++;
    if (col > cols)
    {
      row++;
      col = 1;
    }
  }

  gtk_notebook_append_page_menu (notebook, thispage, pagelabel, NULL);
}

static GtkWidget *
effect_icon_new (BeautifyEffectType effect)
{
  gchar *title = 0;
  switch (effect) {
    case BEAUTIFY_EFFECT_SOFT_LIGHT:
      title = "Soft Light";
      break;
    case BEAUTIFY_EFFECT_WARM:
      title = "Warm";
      break;
    case BEAUTIFY_EFFECT_SHARPEN:
      title = "Sharpen";
      break;
    case BEAUTIFY_EFFECT_SMART_COLOR:
      title = "Smart Color";
      break;
    case BEAUTIFY_EFFECT_INVERT:
      title = "Invert";
      break;
    case BEAUTIFY_EFFECT_GOTHIC_STYLE:
      title = "Gothic Style";
      break;
    case BEAUTIFY_EFFECT_LITTLE_FRESH:
      title = "Little Fresh";
      break;
    case BEAUTIFY_EFFECT_ABAO:
      title = "A Bao";
      break;
    case BEAUTIFY_EFFECT_ICE_SPIRIT:
      title = "Ice Spirit";
      break;
    case BEAUTIFY_EFFECT_JAPANESE:
      title = "Japanese";
      break;
    case BEAUTIFY_EFFECT_NEW_JAPANESE:
      title = "New Japanese";
      break;
    case BEAUTIFY_EFFECT_ASTRAL:
      title = "Astral";
      break;
  }

  gint32 image = gimp_image_duplicate (preview_image);
  do_effect (image, effect);

  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

  GdkPixbuf *pixbuf = gimp_image_get_thumbnail (image, THUMBNAIL_SIZE, THUMBNAIL_SIZE, GIMP_PIXBUF_SMALL_CHECKS);
  GtkWidget *icon = gtk_image_new_from_pixbuf (pixbuf);
  GtkWidget *event_box = gtk_event_box_new ();
  gtk_container_add (GTK_CONTAINER (event_box), icon);
  gtk_widget_show (icon);
  gtk_box_pack_start (GTK_BOX (box), event_box, FALSE, FALSE, 0);
  gtk_widget_show (event_box);

  GtkWidget *label = gtk_label_new (title);
  gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  g_signal_connect (event_box, "button_press_event", G_CALLBACK (select_effect), (gpointer) effect);

  return box;
}

static gboolean
select_effect (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  reset_adjustment ();
  BeautifyEffectType effect = (BeautifyEffectType) user_data;
  do_effect (preview_image, effect);
  current_effect = effect;
  preview_update (preview);

  return TRUE;
}

static void
do_effect (gint32 image, BeautifyEffectType effect)
{
  gint32 layer = gimp_image_get_active_layer (image);
  gint32 effect_layer = gimp_layer_copy (layer);
  gimp_image_add_layer (image, effect_layer, -1);

  switch (effect)
  {
    case BEAUTIFY_EFFECT_SOFT_LIGHT:
      gimp_layer_set_mode (effect_layer, GIMP_SOFTLIGHT_MODE);
      break;

    case BEAUTIFY_EFFECT_WARM:
    {
      guint8 red_pts[] = {
        0.0, 0.082031 * 255,
        0.405488 * 255, 0.621094 * 255,
        0.954268 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.0,
        0.503049 * 255, 0.636719 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 6, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 6, green_pts);
    }
      break;

    case BEAUTIFY_EFFECT_SHARPEN:
    {
      gint nreturn_vals;
      GimpParam *return_vals = gimp_run_procedure ("plug-in-sharpen",
                                                   &nreturn_vals,
                                                   GIMP_PDB_INT32, 1,
                                                   GIMP_PDB_IMAGE, image,
                                                   GIMP_PDB_DRAWABLE, effect_layer,
                                                   GIMP_PDB_INT32, 50,
                                                   GIMP_PDB_END);
      gimp_destroy_params (return_vals, nreturn_vals);
    }
      break;
    case BEAUTIFY_EFFECT_SMART_COLOR:
    {
      guint8 red_pts[] = {
        0.0, 0.001012 * 255, 0.121569 * 255, 0.126695 * 255,
        0.247059 * 255, 0.279821 * 255, 0.372549 * 255, 0.428038 * 255,
        0.498039 * 255, 0.567700 * 255, 0.623529 * 255, 0.699439 * 255,
        0.749020 * 255, 0.821423 * 255, 0.874510 * 255, 0.953474 * 255,
        1.000000 * 255, 0.997988 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.004278 * 255, 0.121569 * 255, 0.107139 * 255,
        0.247059 * 255, 0.225961 * 255, 0.372549 * 255, 0.346578 * 255,
        0.498039 * 255, 0.472647 * 255, 0.623529 * 255, 0.602136 * 255,
        0.749020 * 255, 0.730046 * 255, 0.874510 * 255, 0.873495 * 255,
        1.000000 * 255, 0.996787 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.000105 * 255, 0.121569 * 255, 0.060601 * 255,
        0.247059 * 255, 0.146772 * 255, 0.372549 * 255, 0.262680 * 255,
        0.498039 * 255, 0.408053 * 255, 0.623529 * 255, 0.566459 * 255,
        0.749020 * 255, 0.691468 * 255, 0.874510 * 255, 0.847356 * 255,
        1.000000 * 255, 0.999226 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_INVERT:
      gimp_invert (effect_layer);
      break;
    case BEAUTIFY_EFFECT_GOTHIC_STYLE:
    {
      guint8 red_pts[] = {
        0.0, 0.003922 * 255, 0.121569 * 255, 0.011765 * 255,
        0.247059 * 255, 0.074510 * 255, 0.372549 * 255, 0.200000 * 255,
        0.498039 * 255, 0.380392 * 255, 0.623529 * 255, 0.584314 * 255,
        0.749020 * 255, 0.784314 * 255, 0.874510 * 255, 0.933333 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.003922 * 255, 0.121569 * 255, 0.039216 * 255,
        0.247059 * 255, 0.160784 * 255, 0.372549 * 255, 0.317647 * 255,
        0.498039 * 255, 0.501961 * 255, 0.623529 * 255, 0.682353 * 255,
        0.749020 * 255, 0.843137 * 255, 0.874510 * 255, 0.952941 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.003922 * 255, 0.121569 * 255, 0.007843 * 255,
        0.247059 * 255, 0.058824 * 255, 0.372549 * 255, 0.172549 * 255,
        0.498039 * 255, 0.349020 * 255, 0.623529 * 255, 0.556863 * 255,
        0.749020 * 255, 0.768627 * 255, 0.874510 * 255, 0.929412 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_LITTLE_FRESH:
    {
      guint8 red_pts[] = {
        0.0, 0.002975 * 255, 0.121569 * 255, 0.135413 * 255,
        0.247059 * 255, 0.271797 * 255, 0.372549 * 255, 0.420642 * 255,
        0.498039 * 255, 0.587088 * 255, 0.623529 * 255, 0.672206 * 255,
        0.749020 * 255, 0.781208 * 255, 0.874510 * 255, 0.881668 * 255,
        1.000000 * 255, 0.993149 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.001070 * 255, 0.121569 * 255, 0.123393 * 255,
        0.247059 * 255, 0.254300 * 255, 0.372549 * 255, 0.377336 * 255,
        0.498039 * 255, 0.486582 * 255, 0.623529 * 255, 0.607331 * 255,
        0.749020 * 255, 0.722174 * 255, 0.874510 * 255, 0.858206 * 255,
        1.000000 * 255, 0.992154 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.003917 * 255, 0.121569 * 255, 0.098807 * 255,
        0.247059 * 255, 0.234746 * 255, 0.372549 * 255, 0.378388 * 255,
        0.498039 * 255, 0.520273 * 255, 0.623529 * 255, 0.633239 * 255,
        0.749020 * 255, 0.748242 * 255, 0.874510 * 255, 0.862234 * 255,
        1.000000 * 255, 0.964176 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_ABAO:
      /* TODO */
      break;
    case BEAUTIFY_EFFECT_ICE_SPIRIT:
    {
      guint8 red_pts[] = {
        0.0, 0.007843 * 255, 0.121569 * 255, 0.141176 * 255,
        0.247059 * 255, 0.286275 * 255, 0.372549 * 255, 0.423529 * 255,
        0.498039 * 255, 0.552941 * 255, 0.623529 * 255, 0.674510 * 255,
        0.749020 * 255, 0.792157 * 255, 0.874510 * 255, 0.898039 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.007843 * 255, 0.121569 * 255, 0.184314 * 255,
        0.247059 * 255, 0.360784 * 255, 0.372549 * 255, 0.517647 * 255,
        0.498039 * 255, 0.654902 * 255, 0.623529 * 255, 0.768627 * 255,
        0.749020 * 255, 0.866667 * 255, 0.874510 * 255, 0.945098 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.007843 * 255, 0.121569 * 255, 0.211765 * 255,
        0.247059 * 255, 0.407843 * 255, 0.372549 * 255, 0.576471 * 255,
        0.498039 * 255, 0.717647 * 255, 0.623529 * 255, 0.827451 * 255,
        0.749020 * 255, 0.913725 * 255, 0.874510 * 255, 0.972549 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_JAPANESE:
    {
      guint8 red_pts[] = {
        0.0, 0.098039 * 255, 0.121569 * 255, 0.188479 * 255,
        0.247059 * 255, 0.329761 * 255, 0.372549 * 255, 0.496682 * 255,
        0.498039 * 255, 0.657383 * 255, 0.623529 * 255, 0.787002 * 255,
        0.749020 * 255, 0.864444 * 255, 0.874510 * 255, 0.900704 * 255,
        1.000000 * 255, 0.917552 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.103431 * 255, 0.121569 * 255, 0.224676 * 255,
        0.247059 * 255, 0.394142 * 255, 0.372549 * 255, 0.541888 * 255,
        0.498039 * 255, 0.675963 * 255, 0.623529 * 255, 0.785613 * 255,
        0.749020 * 255, 0.893224 * 255, 0.874510 * 255, 0.943625 * 255,
        1.000000 * 255, 0.972720 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.412025 * 255, 0.121569 * 255, 0.469119 * 255,
        0.247059 * 255, 0.615777 * 255, 0.372549 * 255, 0.751174 * 255,
        0.498039 * 255, 0.862955 * 255, 0.623529 * 255, 0.954468 * 255,
        0.749020 * 255, 0.995760 * 255, 0.874510 * 255, 1.000000 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_NEW_JAPANESE:
    {
      guint8 red_pts[] = {
        0.0, 0.042969 * 255,
        0.350610 * 255, 0.320312 * 255,
        0.621951 * 255, 0.566406 * 255,
        0.847561 * 255, 0.632812 * 255,
        1.000000 * 255, 0.769531 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.031250 * 255,
        0.125000 * 255, 0.144531 * 255,
        0.500000 * 255, 0.523438 * 255,
        0.881098 * 255, 0.738281 * 255,
        1.000000 * 255, 0.882812 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.0,
        0.121951 * 255, 0.039062 * 255,
        1.000000 * 255, 0.972656 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 10, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 10, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 6, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_ASTRAL:
    {
      const gchar *home = g_get_home_dir();
      gchar *dirname = g_build_filename(home, TEXTURE_PATH, NULL);
      gchar *texture = g_build_filename(dirname, "astral.jpg", NULL);
      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (texture, NULL);
      gint32 texture_layer = gimp_layer_new_from_pixbuf (image, "texture", pixbuf, 100, GIMP_SOFTLIGHT_MODE, 0, 0);
      gimp_image_insert_layer (image, texture_layer, -1, 0);
      gimp_layer_scale (texture_layer, width, height, FALSE);
      gimp_image_merge_down (image, texture_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
    }
      break;
  }
}

static void
reset_adjustment ()
{
  if (bvals.brightness != 0) {
    bvals.brightness = 0;
    gtk_range_set_value (GTK_RANGE (brightness), 0);
  }
  if (bvals.contrast != 0) {
    bvals.contrast = 0;
    gtk_range_set_value (GTK_RANGE (contrast), 0);
  }
  if (bvals.saturation != 0) {
    bvals.saturation = 0;
    gtk_range_set_value (GTK_RANGE (saturation), 0);
  }
  if (bvals.sharpeness != 0) {
    bvals.sharpeness = 0;
    gtk_range_set_value (GTK_RANGE (sharpeness), 0);
  }
  if (bvals.hue != 0) {
    bvals.hue = 0;
    gtk_range_set_value (GTK_RANGE (hue), 0);
  }
  if (bvals.cyan_red != 0) {
    bvals.cyan_red = 0;
    gtk_range_set_value (GTK_RANGE (cyan_red), 0);
  }
  if (bvals.magenta_green != 0) {
    bvals.magenta_green = 0;
    gtk_range_set_value (GTK_RANGE (magenta_green), 0);
  }
  if (bvals.yellow_blue != 0) {
    bvals.yellow_blue = 0;
    gtk_range_set_value (GTK_RANGE (yellow_blue), 0);
  }
}

static void
apply_effect ()
{
  if (current_effect == BEAUTIFY_EFFECT_NONE) {
    return;
  }

  gint32 current_layer = gimp_image_get_active_layer (preview_image);
  gimp_image_merge_down (preview_image, current_layer, GIMP_EXPAND_AS_NECESSARY);

  current_effect = BEAUTIFY_EFFECT_NONE;

  if (saved_image) {
    gimp_image_delete (saved_image);
    saved_image = 0;
  }
}

