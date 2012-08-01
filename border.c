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

#include "border-textures.h"

#define RIP_BORDER_PROC   "plug-in-rip-border"
#define TEXTURE_BORDER_PROC   "plug-in-texture-border"
#define PLUG_IN_BINARY "border"
#define PLUG_IN_ROLE   "gimp-border"

#define RIP_BORDER_TEXTURE_PATH   "rip-border"
#define TEXTURE_BORDER_TEXTURE_PATH   "texture-border"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

typedef struct
{
  const guint8* texture;
  GimpRGB  color;
  gdouble  opacity;
  const gchar *custom_texture;
} BorderValues;

static void     query    (void);
static void     run      (const gchar      *name,
                          gint              nparams,
                          const GimpParam  *param,
                          gint             *nreturn_vals,
                          GimpParam       **return_vals);

static void     border (gint32     image_ID);

static gboolean border_dialog (const gchar *title, const gchar *help_id, const guint8** textures, guint n_textures, const gchar *texture_path);

static void     create_texture_page (GtkNotebook *notebook, const gchar *category, const guint8** textures, guint n_textures);
static gboolean create_custom_texture_pages (GtkNotebook *notebook, const gchar *texture_path);
static void     create_custom_texture_page (GtkNotebook *notebook, const gchar *category, const gchar *path);
static void     textures_switch_page (GtkNotebook *notebook, GtkWidget *page, guint page_num, const gchar *texture_path);

static gboolean texture_press (GtkWidget *event_box, GdkEventButton *event, const guint8 *texture);
static gboolean custom_texture_press (GtkWidget *event_box, GdkEventButton *event, const gchar *texture);
static void     do_texture_press ();

static inline gboolean
is_hidden (const gchar *filename)
{
  /* skip files starting with '.' so we don't try to parse
* stuff like .DS_Store or other metadata storage files
*/
  return (filename[0] == '.');
}

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

static BorderValues bvals =
{
  NULL,                 /* texture */
  {1.0, 1.0, 1.0, 1.0}, /* color */
  100,                  /* opacity */
  NULL,                 /* custom_texture */
};

static gint32     image_ID         = 0;
static gint       width;
static gint       height;

static GtkWidget *preview          = NULL;
static gint32     preview_image    = 0;
static gint32     color_layer      = 0;
static gint32     texture_mask     = 0;

static const guint8* rip_border_textures[] =
{
  texture_14911,
  texture_12847,
  texture_201301,
  texture_201185,
  texture_111601,
  texture_201072,
  texture_15614,
  texture_201106,
  texture_12858,
  texture_200836,
  texture_15618,
  texture_200773,
  texture_200623,
  texture_200549,
  texture_200442,
};

static const guint8* texture_border_textures[] =
{
  texture_12854,
  texture_200542,
  texture_15315,
  texture_13039,
  texture_201367,
  texture_15311,
  texture_200835,
  texture_201122,
  texture_114549,
  texture_200650,
  texture_15319,
  texture_200622,
  texture_200763,
  texture_200285,
  texture_200543,
};

static GArray *textures_timestamps = NULL;

/* compatable with gtk2 */
#if GTK_MAJOR_VERSION < 3
GtkWidget *
gtk_box_new (GtkOrientation  orientation,
             gint            spacing)
{
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    return gtk_hbox_new (FALSE, spacing);
  else
    return gtk_vbox_new (FALSE, spacing);
}
#endif

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

  gimp_install_procedure (RIP_BORDER_PROC,
                          "Create an rip border effect",
                          "Rip border is one of the beautify serial plugin.",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Rip Border...",
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args), 0,
                          args, NULL);

  gimp_install_procedure (TEXTURE_BORDER_PROC,
                          "Create an texture border effect",
                          "Texture border is one of the beautify serial plugin.",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Texture Border...",
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args), 0,
                          args, NULL);

  gimp_plugin_menu_register (RIP_BORDER_PROC,     "<Image>/Filters/Beautify");
  gimp_plugin_menu_register (TEXTURE_BORDER_PROC, "<Image>/Filters/Beautify");
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

  if (strcmp (name, RIP_BORDER_PROC) == 0)
  {
    switch (run_mode)
    {
      case GIMP_RUN_INTERACTIVE:
        if (! border_dialog ("Rip Border", RIP_BORDER_PROC, rip_border_textures, G_N_ELEMENTS (rip_border_textures), RIP_BORDER_TEXTURE_PATH))
          return;
        break;

      case GIMP_RUN_NONINTERACTIVE:
        break;

      case GIMP_RUN_WITH_LAST_VALS:
        break;

      default:
        break;
    }
  }
  else if (strcmp (name, TEXTURE_BORDER_PROC) == 0)
  {
    switch (run_mode)
    {
      case GIMP_RUN_INTERACTIVE:
        if (! border_dialog ("Texture Border", TEXTURE_BORDER_PROC, texture_border_textures, G_N_ELEMENTS (texture_border_textures), TEXTURE_BORDER_TEXTURE_PATH))
          return;
        break;

      case GIMP_RUN_NONINTERACTIVE:
        break;

      case GIMP_RUN_WITH_LAST_VALS:
        break;

      default:
        break;
    }
  }

  if ((status == GIMP_PDB_SUCCESS) &&
      (gimp_drawable_is_rgb(drawable->drawable_id) ||
       gimp_drawable_is_gray(drawable->drawable_id)))
    {
      /* Run! */
      gimp_image_undo_group_start (image_ID);
      border (image_ID);
      gimp_image_undo_group_end (image_ID);

      /* If run mode is interactive, flush displays */
      if (run_mode != GIMP_RUN_NONINTERACTIVE)
        gimp_displays_flush ();

      /* Store data */
      /*if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data (PLUG_IN_PROC, &rbvals, sizeof (BorderValues));*/

    }

  gimp_drawable_detach (drawable);
}

static void
border (gint32 image_ID)
{
  GdkPixbuf *pixbuf = NULL;

  if (bvals.texture)
    pixbuf = gdk_pixbuf_new_from_inline (-1, bvals.texture, FALSE, NULL);
  else if (bvals.custom_texture)
    pixbuf = gdk_pixbuf_new_from_file (bvals.custom_texture, NULL);

  if (pixbuf)
  {
    gint32 color_layer = gimp_layer_new (image_ID, "color",
                                         width, height,
                                         GIMP_RGBA_IMAGE,
                                         bvals.opacity,
                                         GIMP_NORMAL_MODE);
    gimp_image_add_layer (image_ID, color_layer, -1);

    gimp_context_set_foreground (&bvals.color);
    gimp_edit_fill (color_layer, GIMP_FOREGROUND_FILL);

    gint32 texture_mask = gimp_layer_create_mask (color_layer,
                                                  GIMP_ADD_WHITE_MASK);
    gimp_layer_add_mask (color_layer, texture_mask);

    gint32 texture = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, bvals.opacity, GIMP_NORMAL_MODE, 0, 0);
    gimp_image_add_layer (image_ID, texture, -1);
    gimp_layer_scale (texture, width, height, FALSE);
    gimp_invert (texture);
    gimp_edit_copy (texture);
    gint32 floating_sel_ID = gimp_edit_paste (texture_mask, TRUE);
    gimp_image_remove_layer (image_ID, texture);

    gimp_floating_sel_anchor (floating_sel_ID);

    gimp_image_merge_down(image_ID, color_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
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

static void
color_update (GtkWidget *preview)
{
  gimp_context_set_foreground (&bvals.color);
  gimp_edit_fill (color_layer, GIMP_FOREGROUND_FILL);
  
  preview_update (preview);
}

static void
opacity_update (GtkRange *range, gpointer data) {
  bvals.opacity = gtk_range_get_value (range);
  gimp_layer_set_opacity (color_layer, bvals.opacity);
  preview_update (preview);
}

static gboolean
border_dialog (const gchar *title, const gchar *help_id, const guint8** textures, guint n_textures, const gchar *texture_path)
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

  dialog = gimp_dialog_new (title, PLUG_IN_ROLE,
                            NULL, 0,
                            gimp_standard_help_func, help_id,

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
  gtk_widget_set_size_request (right_vbox, 320, -1);
  gtk_box_pack_start (GTK_BOX (main_hbox), right_vbox, TRUE, TRUE, 0);
  gtk_widget_show (right_vbox);

  /* color select */
  label = gtk_label_new ("Border color");
  gtk_box_pack_start (GTK_BOX (left_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *button = gimp_color_button_new ("Border color", 40, 20, &bvals.color, GIMP_COLOR_AREA_FLAT);
  gimp_color_button_set_update (GIMP_COLOR_BUTTON (button), TRUE);
  gtk_box_pack_start (GTK_BOX (left_vbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_signal_connect (button, "color-changed",
                   G_CALLBACK (gimp_color_button_get_color),
                   &bvals.color);

  /* opacity */
  label = gtk_label_new ("Border opacity");
  gtk_box_pack_start (GTK_BOX (left_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *hscale = gtk_hscale_new_with_range (0, 100, 1);
  gtk_range_set_value (GTK_RANGE (hscale), bvals.opacity);
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
                                         bvals.opacity,
                                         GIMP_NORMAL_MODE);
  gimp_image_add_layer (preview_image, color_layer, -1);

  gimp_context_set_foreground (&bvals.color);
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

  GtkWidget *notebook = gtk_notebook_new ();
  gtk_box_pack_start (GTK_BOX (right_vbox), notebook, FALSE, FALSE, 0);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), TRUE);
  gtk_widget_show (notebook);

  create_texture_page (GTK_NOTEBOOK (notebook), "Top", textures, n_textures);

  if (create_custom_texture_pages (GTK_NOTEBOOK (notebook), texture_path))
  {
    textures_timestamps = g_array_new (FALSE, TRUE, sizeof (time_t));
    g_array_set_size (textures_timestamps, gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook)));
    g_signal_connect (notebook, "switch-page", G_CALLBACK (textures_switch_page), (gpointer) texture_path);
  }
  else
  {
    label = gtk_label_new ("You can download more textures at https://github.com/hejiann/beautify/wiki/Textures-Download");
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_box_pack_start (GTK_BOX (right_vbox), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
  }

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}

static void
create_texture_page (GtkNotebook *notebook, const gchar* category, const guint8** textures, guint n_textures) {
  GtkWidget *label = gtk_label_new (category);

  GtkWidget *thispage = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (thispage), 12);
  gtk_widget_show (thispage);

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
  for (i = 0; i < n_textures; i++)
  {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline (-1, textures[i], FALSE, NULL);
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

    g_signal_connect (event_box, "button_press_event", G_CALLBACK (texture_press), (gpointer)textures[i]);
  }

  gtk_notebook_append_page_menu (notebook, thispage, label, NULL);
}

static gboolean
create_custom_texture_pages (GtkNotebook *notebook, const gchar *texture_path)
{
  gboolean has_custom_texture = FALSE;

  const gchar *gimp_dir = gimp_directory ();
  const gchar *texture_dir = g_build_filename (gimp_dir, texture_path, NULL);
  GDir *dir = g_dir_open (texture_dir, 0, NULL);
  if (dir)
  {
    const gchar *dir_ent;
    while (dir_ent = g_dir_read_name (dir))
    {
      if (is_hidden (dir_ent))
        continue;

      gchar *filename = g_build_filename (texture_dir, dir_ent, NULL);
      if (g_file_test (filename, G_FILE_TEST_IS_DIR)) {
        create_custom_texture_page (GTK_NOTEBOOK (notebook), dir_ent, filename);
        has_custom_texture = TRUE;
      }
    }
  }

  return has_custom_texture;
}

static void
create_custom_texture_page (GtkNotebook *notebook, const gchar* category, const gchar* path) {
  GtkWidget *label = gtk_label_new (category);

  GtkWidget *thispage = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (thispage), 12);
  gtk_widget_show (thispage);

  gtk_notebook_append_page_menu (notebook, thispage, label, NULL);
}

static void
textures_switch_page (GtkNotebook *notebook, GtkWidget *page, guint page_num, const gchar *texture_path)
{
  if (page_num == 0 || g_array_index (textures_timestamps, time_t, page_num) > 0)
    return;

  // fix gtk2
  page = gtk_notebook_get_nth_page (notebook, page_num);

  gtk_container_set_border_width (GTK_CONTAINER (page), 0);
  gtk_widget_set_size_request (page, -1, 480);

  const gchar *category = gtk_notebook_get_tab_label_text(notebook, page);

  /* scrolled window */
  GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  gtk_box_pack_start (GTK_BOX (page), scrolled_window, TRUE, TRUE, 0);
  gtk_widget_show (scrolled_window);

  /* table */
  gint rows = 5;
  gint cols = 3;
  GtkWidget *table = gtk_table_new (rows, cols, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  gtk_container_set_border_width (GTK_CONTAINER (table), 10);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), table);
  gtk_widget_show (table);

  gint row = 1;
  gint col = 1;

  const gchar *gimp_dir = gimp_directory ();
  const gchar *texture_dir = g_build_filename (gimp_dir, texture_path, NULL);
  const gchar *path = g_build_filename (texture_dir, category, NULL);

  GDir *dir = g_dir_open (path, 0, NULL);
  if (dir)
  {
    const gchar *dir_ent;
    while (dir_ent = g_dir_read_name (dir))
    {
      if (is_hidden (dir_ent))
        continue;

      gchar *filename = g_build_filename (path, dir_ent, NULL);
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

      g_signal_connect (event_box, "button_press_event", G_CALLBACK (custom_texture_press), filename);
    }
  }

  g_array_index (textures_timestamps, time_t, page_num) = time (NULL);
}

static gboolean
texture_press (GtkWidget *event_box, GdkEventButton *event, const guint8* texture)
{
  bvals.texture = texture;
  bvals.custom_texture = NULL;

  do_texture_press ();
}

static gboolean
custom_texture_press (GtkWidget *event_box, GdkEventButton *event, const gchar *texture)
{
  bvals.texture = NULL;
  bvals.custom_texture = texture;

  do_texture_press ();
}

static void
do_texture_press ()
{
  GdkPixbuf *pixbuf;
  if (bvals.texture)
    pixbuf = gdk_pixbuf_new_from_inline (-1, bvals.texture, FALSE, NULL);
  else
    pixbuf = gdk_pixbuf_new_from_file (bvals.custom_texture, NULL);

  gint32 texture_layer = gimp_layer_new_from_pixbuf (preview_image,
                                              "texture",
                                              pixbuf,
                                              bvals.opacity,
                                              GIMP_NORMAL_MODE, 0, 0);
  gimp_image_add_layer (preview_image, texture_layer, -1);
  gimp_layer_scale (texture_layer, width, height, FALSE);
  gimp_invert (texture_layer);
  gimp_edit_copy (texture_layer);
  gimp_edit_paste (texture_mask, TRUE);
  gimp_image_remove_layer (preview_image, texture_layer);

  preview_update (preview);
}

