/**
 * Copyright (C) 2010 hejian <hejian.he@gmail.com>
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

#define PLUG_IN_PROC   "plug-in-rip-border"
#define PLUG_IN_BINARY "rip-border"
#define PLUG_IN_ROLE   "gimp-rip-border"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

typedef struct
{
  gchar   *texture;
  GimpRGB  color;
  gdouble  opacity;
} RipBorderValues;

static void     query    (void);
static void     run      (const gchar      *name,
                          gint              nparams,
                          const GimpParam  *param,
                          gint             *nreturn_vals,
                          GimpParam       **return_vals);

static void     rip_border (gint32     image_ID);

static gboolean rip_border_dialog ();

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

static RipBorderValues rbvals =
{
  NULL,                 /* texture file */
  {1.0, 1.0, 1.0, 1.0}, /* color */
  100,                  /* opacity */
};

static gint32     image_ID         = 0;
static gint       width;
static gint       height;

static GtkWidget *preview          = NULL;
static gint32     preview_image    = 0;
static gint32     color_layer      = 0;
static gint32     texture_mask     = 0;

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
                          "Create an rip border effect",
                          "Beautify, most easiest way for beautify photo."
                          "Rip border is one of the beautify serial plugin.",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Rip Border...",
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
      if (! rip_border_dialog ())
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
      rip_border (image_ID);
      gimp_image_undo_group_end (image_ID);

      /* If run mode is interactive, flush displays */
      if (run_mode != GIMP_RUN_NONINTERACTIVE)
        gimp_displays_flush ();

      /* Store data */
      if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data (PLUG_IN_PROC, &rbvals, sizeof (RipBorderValues));

    }

  gimp_drawable_detach (drawable);
}

static void
rip_border (gint32 image_ID)
{
  if (rbvals.texture != NULL)
  {
    gint32 color_layer = gimp_layer_new (image_ID, "color",
                                         width, height,
                                         GIMP_RGBA_IMAGE,
                                         rbvals.opacity,
                                         GIMP_NORMAL_MODE);
    gimp_image_insert_layer (image_ID, color_layer, -1, 0);

    gimp_context_set_foreground (&rbvals.color);
    gimp_edit_fill (color_layer, GIMP_FOREGROUND_FILL);

    gint32 texture_mask = gimp_layer_create_mask (color_layer,
                                                  GIMP_ADD_WHITE_MASK);
    gimp_layer_add_mask (color_layer, texture_mask);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (rbvals.texture, NULL);
    gint32 texture = gimp_layer_new_from_pixbuf(image_ID, "texture", pixbuf, rbvals.opacity, GIMP_NORMAL_MODE, 0, 0);
    gimp_image_insert_layer (image_ID, texture, -1, 0);
    gimp_layer_scale (texture, width, height, FALSE);
    gimp_invert (texture);
    gimp_edit_copy (texture);
    gint32 floating_sel_ID = gimp_edit_paste (texture_mask, TRUE);
    gimp_image_remove_layer (image_ID, texture);

    gimp_floating_sel_anchor (floating_sel_ID);

    gimp_image_merge_down(image_ID, color_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
  }
}

static inline gboolean
is_hidden (const gchar *filename)
{
  /*  skip files starting with '.' so we don't try to parse
   *  stuff like .DS_Store or other metadata storage files
   */
  return (filename[0] == '.');
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
select_texture (GtkWidget *event_box, GdkEventButton *event, gchar* texture)
{
  rbvals.texture = texture;

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (rbvals.texture, NULL);
  gint32 texture_layer = gimp_layer_new_from_pixbuf (preview_image,
                                              "texture",
                                              pixbuf,
                                              rbvals.opacity,
                                              GIMP_NORMAL_MODE, 0, 0);
  gimp_image_insert_layer (preview_image, texture_layer, -1, 0);
  gimp_layer_scale (texture_layer, width, height, FALSE);
  gimp_invert (texture_layer);
  gimp_edit_copy (texture_layer);
  gimp_edit_paste (texture_mask, TRUE);
  gimp_image_remove_layer (preview_image, texture_layer);

  preview_update (preview);
}

static void
color_update (GtkWidget *preview)
{
  gimp_context_set_foreground (&rbvals.color);
  gimp_edit_fill (color_layer, GIMP_FOREGROUND_FILL);
  
  preview_update (preview);
}

static void
opacity_update (GtkRange *range, gpointer data) {
  rbvals.opacity = gtk_range_get_value (range);
  gimp_layer_set_opacity (color_layer, rbvals.opacity);
  preview_update (preview);
}

static gboolean
rip_border_dialog ()
{
  GtkWidget *dialog;
  GtkWidget *main_hbox;
  GtkWidget *left_vbox;
  GtkWidget *middle_vbox;
  GtkWidget *right_vbox;
  GtkWidget *label;

  GtkWidget *frame;
  gboolean   run;

  gimp_ui_init (PLUG_IN_BINARY, FALSE);

  dialog = gimp_dialog_new ("Rip border", PLUG_IN_ROLE,
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

  /* color select */
  label = gtk_label_new ("Border color");
  gtk_box_pack_start (GTK_BOX (left_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *button = gimp_color_button_new ("Border color", 40, 20, &rbvals.color, GIMP_COLOR_AREA_FLAT);
  gimp_color_button_set_update (GIMP_COLOR_BUTTON (button), TRUE);
  gtk_box_pack_start (GTK_BOX (left_vbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_signal_connect (button, "color-changed",
                   G_CALLBACK (gimp_color_button_get_color),
                   &rbvals.color);

  /* opacity */
  label = gtk_label_new ("Border opacity");
  gtk_box_pack_start (GTK_BOX (left_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *hscale = gtk_hscale_new_with_range (0, 100, 1);
  gtk_range_set_value (GTK_RANGE (hscale), rbvals.opacity);
  gtk_scale_set_value_pos (GTK_SCALE (hscale), GTK_POS_BOTTOM);
  gtk_box_pack_start (GTK_BOX (left_vbox), hscale, FALSE, FALSE, 0);
  gtk_widget_show (hscale);

  g_signal_connect (hscale, "value-changed",
                   G_CALLBACK (opacity_update),
                   NULL);

  /* preview */
  label = gtk_label_new ("Preview");
  gtk_box_pack_start (GTK_BOX (middle_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  preview_image = gimp_image_duplicate(image_ID);
  color_layer = gimp_layer_new (preview_image, "color",
                                         width, height,
                                         GIMP_RGBA_IMAGE,
                                         rbvals.opacity,
                                         GIMP_NORMAL_MODE);
  gimp_image_insert_layer (preview_image, color_layer, -1, 0);

  gimp_context_set_foreground (&rbvals.color);
  gimp_edit_fill (color_layer, GIMP_FOREGROUND_FILL);

  texture_mask = gimp_layer_create_mask (color_layer,
                                         GIMP_ADD_BLACK_MASK);
  gimp_layer_add_mask (color_layer, texture_mask);

  preview = gtk_image_new();
  preview_update (preview);

  gtk_box_pack_start (GTK_BOX (middle_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  g_signal_connect_swapped (button, "color-changed",
                           G_CALLBACK (color_update),
                           preview);

  /* textures */
  label = gtk_label_new ("Textures");
  gtk_box_pack_start (GTK_BOX (right_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  gint rows = 5;
  gint cols = 3;
  GtkWidget *table = gtk_table_new (rows, cols, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_box_pack_start (GTK_BOX (right_vbox), table, FALSE, FALSE, 0);
  gtk_widget_show (table);

  gint row = 1;
  gint col = 1;
  const gchar *home = g_get_home_dir();
  gchar *dirname = g_strconcat(home, "/textures", NULL);
  GDir *dir = g_dir_open(dirname, 0, NULL);
  if (dir)
  {
    const gchar *dir_ent;
    while (dir_ent = g_dir_read_name(dir))
    {
      gchar *filename;

      if (is_hidden (dir_ent))
        continue;

      filename = g_build_filename (dirname, dir_ent, NULL);

      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (filename, NULL);
      pixbuf = gdk_pixbuf_scale_simple (pixbuf, THUMBNAIL_SIZE, THUMBNAIL_SIZE, GDK_INTERP_BILINEAR);
      GtkWidget *image = gtk_image_new_from_pixbuf (pixbuf);
      GtkWidget *event_box = gtk_event_box_new ();
      gtk_container_add (GTK_CONTAINER (event_box), image);
      gtk_widget_show (image);
      gtk_table_attach_defaults (GTK_TABLE (table), event_box, col - 1, col, row - 1, row);
      gtk_widget_show (event_box);

      col++;
      if (col > cols)
      {
        row++;
        col = 1;
      }

      g_signal_connect (event_box, "button_press_event", G_CALLBACK (select_texture), filename);

      //g_free(filename);
    }
    g_dir_close (dir);
  }

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}

