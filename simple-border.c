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

#include "simple-border-textures.h"

#define PLUG_IN_PROC   "plug-in-simple-border"
#define PLUG_IN_BINARY "border"
#define PLUG_IN_ROLE   "gimp-border"

#define TEXTURE_PATH     "simple-border"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

typedef struct
{
  gint32  top;
  gint32  bottom;
  gint32  left;
  gint32  right;
  gint32  length;
  const guint8* texture;
} Border;

typedef struct
{
  const Border* border;
  const gchar *custom_texture;
} BorderValues;

static void     query    (void);
static void     run      (const gchar      *name,
                          gint              nparams,
                          const GimpParam  *param,
                          gint             *nreturn_vals,
                          GimpParam       **return_vals);

static void     border (gint32     image_ID);

static gboolean border_dialog (gint32 image_ID,
                               GimpDrawable *drawable);

static void     create_texture_page (GtkNotebook *notebook, const gchar *category, const Border* textures, guint n_textures);
static gboolean create_custom_texture_pages (GtkNotebook *notebook, const gchar *texture_path);
static void     create_custom_texture_page (GtkNotebook *notebook, const gchar *category, const gchar *path);
static void     textures_switch_page (GtkNotebook *notebook, GtkWidget *page, guint page_num, const gchar *texture_path);

static gboolean texture_press (GtkWidget *event_box, GdkEventButton *event, const Border *border);
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
  NULL,                 /* border */
};

static gint32     image_ID         = 0;
static gint       width;
static gint       height;

static GtkWidget *preview          = NULL;
static gint32     preview_image    = 0;

static const Border textures[] =
{
  {0, 0, 0, 0, 10, texture_15356},
  {24,37,29,41,10, texture_15327},
  {6, 6, 6, 6, 10, texture_201544},
  {9, 35,7, 6, 10, texture_15353},
  {12,11,12,11,10, texture_201365},
  {56,63,54,56,10, texture_15349},
  {40,43,35,38,90, texture_24252},
  {14,13,14,14,10, texture_114844},
  {7, 7, 7, 7, 10, texture_200832},
  {6, 6, 6, 6, 10, texture_200878},
  {0, 0, 0, 0, 10, texture_201017},
  {0, 0, 0, 0, 10, texture_114568},
  {0, 0, 0, 0, 10, texture_111026},
  {11,88,11,10,10, texture_200377},
  {6, 6, 6, 6, 10, texture_201385},
  {13,28,8, 8, 10, texture_111025},
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

  gimp_install_procedure (PLUG_IN_PROC,
                          "Create an simple border effect",
                          "Create an simple border effect",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Simple Border...",
                          "RGB*, GRAY*",
                          GIMP_PLUGIN,
                          G_N_ELEMENTS (args), 0,
                          args, NULL);

  gimp_plugin_menu_register (PLUG_IN_PROC,     "<Image>/Filters/Beautify/Border");
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
        if (! border_dialog (image_ID, drawable))
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
  int        texture_width;
  int        texture_height;
  gdouble    margin_x;
  gdouble    margin_y;

  pixbuf = gdk_pixbuf_new_from_inline (-1, bvals.border->texture, FALSE, NULL);

  if (pixbuf)
  {
    texture_width = gdk_pixbuf_get_width (pixbuf);
    texture_height = gdk_pixbuf_get_height (pixbuf);

    gint32 texture_image = gimp_image_new (texture_width, texture_height, GIMP_RGB);
    gint32 texture_layer = gimp_layer_new_from_pixbuf (texture_image, "texture", pixbuf, 100, GIMP_NORMAL_MODE, 0, 0);
    gimp_image_add_layer (texture_image, texture_layer, -1);

    gint width = gimp_image_width (image_ID);
    gint height = gimp_image_height (image_ID);

    if (bvals.border->top || bvals.border->bottom || bvals.border->left || bvals.border->right)
    {
      width += bvals.border->left + bvals.border->right;
      height += bvals.border->top + bvals.border->bottom;
      gimp_image_resize (image_ID,
                         width,
                         height,
                         bvals.border->left,
                         bvals.border->top);
    }

    gint32 layer = gimp_layer_new (image_ID, "border",
                                   width, height,
                                   GIMP_RGBA_IMAGE,
                                   100,
                                   GIMP_NORMAL_MODE);
    gimp_image_add_layer (image_ID, layer, -1);

    if (width > texture_width - bvals.border->length)
      margin_x = (texture_width - bvals.border->length) / 2;
    else
      margin_x = (gdouble) width / 2;
    if (height > texture_height - bvals.border->length)
      margin_y = (texture_height - bvals.border->length) / 2;
    else
      margin_y = (gdouble) height / 2;

    gimp_context_set_pattern ("Clipboard");

    if (width > margin_x * 2) {
      /* top */
      gimp_rect_select (texture_image,
                        margin_x,
                        0,
                        texture_width - margin_x * 2,
                        margin_y,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_copy (texture_layer);
      gimp_rect_select (image_ID,
                        margin_x,
                        0,
                        width - margin_x * 2,
                        margin_y,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_fill (layer, GIMP_PATTERN_FILL);

      /* bottom */
      gimp_rect_select (texture_image,
                        margin_x,
                        texture_height - margin_y,
                        texture_width - margin_x * 2,
                        texture_height,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_copy (texture_layer);
      gimp_rect_select (image_ID,
                        margin_x,
                        height - margin_y,
                        width - margin_x * 2,
                        height,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_fill (layer, GIMP_PATTERN_FILL);
    }

    if (height > margin_y * 2) {
      /* left */
      gimp_rect_select (texture_image,
                        0,
                        margin_y,
                        margin_x,
                        texture_height - margin_y * 2,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_copy (texture_layer);
      gimp_rect_select (image_ID,
                        0,
                        margin_y,
                        margin_x,
                        height - margin_y * 2,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_fill (layer, GIMP_PATTERN_FILL);

      /* right */
      gimp_rect_select (texture_image,
                        texture_width - margin_x,
                        margin_y,
                        margin_x,
                        texture_height - margin_y * 2,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_copy (texture_layer);
      gimp_rect_select (image_ID,
                        width - margin_x,
                        margin_y,
                        margin_x,
                        height - margin_y * 2,
                        GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
      gimp_edit_fill (layer, GIMP_PATTERN_FILL);
    }

    /* top left */
    gimp_rect_select (texture_image,
                      0,
                      0,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_copy (texture_layer);
    gimp_rect_select (image_ID,
                      0,
                      0,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_fill (layer, GIMP_PATTERN_FILL);

    /* top right */
    gimp_rect_select (texture_image,
                      texture_width - margin_x,
                      0,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_copy (texture_layer);
    gimp_rect_select (image_ID,
                      width - margin_x,
                      0,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_fill (layer, GIMP_PATTERN_FILL);

    /* bottom left */
    gimp_rect_select (texture_image,
                      0,
                      texture_height - margin_y,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_copy (texture_layer);
    gimp_rect_select (image_ID,
                      0,
                      height - margin_y,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_fill (layer, GIMP_PATTERN_FILL);

    /* bottom right */
    gimp_rect_select (texture_image,
                      texture_width - margin_x,
                      texture_height - margin_y,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_copy (texture_layer);
    gimp_rect_select (image_ID,
                      width - margin_x,
                      height - margin_y,
                      margin_x,
                      margin_y,
                      GIMP_CHANNEL_OP_REPLACE, FALSE, 0);
    gimp_edit_fill (layer, GIMP_PATTERN_FILL);

    gimp_image_merge_down(image_ID, layer, GIMP_CLIP_TO_IMAGE);

    gimp_selection_none (image_ID);
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
border_dialog (gint32        image_ID,
               GimpDrawable *drawable)
{
  GtkWidget *dialog;
  GtkWidget *main_hbox;
  GtkWidget *middle_vbox;
  GtkWidget *right_vbox;
  GtkWidget *label;

  gboolean   run;

  gimp_ui_init (PLUG_IN_BINARY, FALSE);

  dialog = gimp_dialog_new ("Simple Border", PLUG_IN_ROLE,
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

  middle_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (middle_vbox), 12);
  gtk_box_pack_start (GTK_BOX (main_hbox), middle_vbox, TRUE, TRUE, 0);
  gtk_widget_show (middle_vbox);

  right_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  gtk_container_set_border_width (GTK_CONTAINER (right_vbox), 12);
  gtk_widget_set_size_request (right_vbox, 320, -1);
  gtk_box_pack_start (GTK_BOX (main_hbox), right_vbox, TRUE, TRUE, 0);
  gtk_widget_show (right_vbox);

  /* preview */
  label = gtk_label_new ("Preview");
  gtk_box_pack_start (GTK_BOX (middle_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  preview_image = gimp_image_duplicate(image_ID);

  preview = gtk_image_new();
  preview_update (preview);

  gtk_box_pack_start (GTK_BOX (middle_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  /* textures */
  label = gtk_label_new ("Textures");
  gtk_box_pack_start (GTK_BOX (right_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *notebook = gtk_notebook_new ();
  gtk_box_pack_start (GTK_BOX (right_vbox), notebook, FALSE, FALSE, 0);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), TRUE);
  gtk_widget_show (notebook);

  create_texture_page (GTK_NOTEBOOK (notebook), "Top", textures, G_N_ELEMENTS (textures));

  run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}

static void
create_texture_page (GtkNotebook *notebook, const gchar* category, const Border* textures, guint n_textures) {
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
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline (-1, textures[i].texture, FALSE, NULL);
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

    g_signal_connect (event_box, "button_press_event", G_CALLBACK (texture_press), (gpointer)&textures[i]);
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
texture_press (GtkWidget *event_box, GdkEventButton *event, const Border* texture)
{
  bvals.border = texture;
  bvals.custom_texture = NULL;

  do_texture_press ();
}

static gboolean
custom_texture_press (GtkWidget *event_box, GdkEventButton *event, const gchar *texture)
{
  bvals.border = NULL;
  bvals.custom_texture = texture;

  do_texture_press ();
}

static void
do_texture_press ()
{
  gimp_image_delete (preview_image);
  preview_image = gimp_image_duplicate(image_ID);

  border (preview_image);

  preview_update (preview);
}

