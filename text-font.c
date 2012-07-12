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

#define PLUG_IN_PROC   "plug-in-text-font"
#define PLUG_IN_BINARY "text-font"
#define PLUG_IN_ROLE   "gimp-text-font"

#define PREVIEW_SIZE  480
#define THUMBNAIL_SIZE  80

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct
{
  gchar* font;
} TextFontValues;

static void     query    (void);
static void     run      (const gchar      *name,
                          gint              nparams,
                          const GimpParam  *param,
                          gint             *nreturn_vals,
                          GimpParam       **return_vals);

static void     text_font (GimpDrawable *drawable);

static gboolean text_font_dialog (gint32        image_ID,
                                  GimpDrawable *drawable);

static gboolean select_font (GtkWidget      *event_box,
                             GdkEventButton *event,
                             gchar          *font);

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  /* init_proc  */
  NULL,  /* quit_proc  */
  query, /* query_proc */
  run,   /* run_proc   */
};

static TextFontValues tfvals =
{
  0,  /* font */
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
                          "Easy text font select.",
                          "Easy text font select.",
                          "Hejian <hejian.he@gmail.com>",
                          "Hejian <hejian.he@gmail.com>",
                          "2012",
                          "_Text font select...",
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

  if (!gimp_item_is_text_layer (drawable->drawable_id)) {
    status = GIMP_PDB_EXECUTION_ERROR;
    *nreturn_vals = 2;
    values[0].data.d_status = status;
    values[1].type = GIMP_PDB_STRING;
    values[1].data.d_string = ("This plugin only can process text layer.");
    return;
  }

  width = gimp_image_width (image_ID);
  height = gimp_image_height (image_ID);

  switch (run_mode)
  {
    case GIMP_RUN_INTERACTIVE:
      if (! text_font_dialog (image_ID, drawable))
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
      gimp_item_is_text_layer(drawable->drawable_id) &&
      (gimp_drawable_is_rgb(drawable->drawable_id) ||
       gimp_drawable_is_gray(drawable->drawable_id)))
    {
      /* Run! */
      gimp_image_undo_group_start (image_ID);
      text_font (drawable);
      gimp_image_undo_group_end (image_ID);

      /* If run mode is interactive, flush displays */
      if (run_mode != GIMP_RUN_NONINTERACTIVE)
        gimp_displays_flush ();

      /* Store data */
      if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_set_data (PLUG_IN_PROC, &tfvals, sizeof (TextFontValues));

    }

  gimp_drawable_detach (drawable);
}

static void
text_font (GimpDrawable *drawable)
{
  if (tfvals.font != 0) {
    gimp_text_layer_set_font (drawable->drawable_id, tfvals.font);
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
text_font_dialog (gint32        image_ID,
                  GimpDrawable *drawable)
{
  GtkWidget *dialog;
  GtkWidget *main_hbox;
  GtkWidget *left_vbox;
  GtkWidget *middle_vbox;
  GtkWidget *right_vbox;
  GtkWidget *label;

  gimp_ui_init (PLUG_IN_BINARY, FALSE);

  dialog = gimp_dialog_new ("Text font select", PLUG_IN_ROLE,
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

  /* preview */
  label = gtk_label_new ("Preview");
  gtk_box_pack_start (GTK_BOX (middle_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  preview_image = gimp_image_duplicate (image_ID);

  preview = gtk_image_new();
  preview_update (preview);

  gtk_box_pack_start (GTK_BOX (middle_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  /* fonts */
  label = gtk_label_new ("Select font");
  gtk_box_pack_start (GTK_BOX (right_vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  GtkWidget *scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_box_pack_start (GTK_BOX (right_vbox), scrolled_win, TRUE, TRUE, 0);
  gtk_widget_show (scrolled_win);

  gint rows = 5;
  gint cols = 3;
  GtkWidget *table = gtk_table_new (rows, cols, FALSE);
  gtk_table_set_col_spacings (GTK_TABLE (table), 6);
  gtk_table_set_row_spacings (GTK_TABLE (table), 6);
  //gtk_container_add (GTK_CONTAINER (scrolled_win), table);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_win), table);
  gtk_widget_show (table);

  gint num_fonts;
  gchar **fonts = gimp_fonts_get_list (NULL, &num_fonts);

  gint32 text_image = gimp_image_duplicate (image_ID);

  gint offset_x, offset_y;
  gimp_drawable_offsets (drawable->drawable_id, &offset_x, &offset_y);
  gint text_width = gimp_drawable_width(drawable->drawable_id);
  gint text_height = gimp_drawable_height(drawable->drawable_id);
  text_width += 20;
  text_height += 20;
  offset_x -= 10;
  offset_y -= 10;
  /*if (offset_x < 0)
    offset_x = 0;
  if (offset_y < 0)
    offset_y = 0;
  if (offset_x + text_width > width)
    text_width = width - offset_x;
  if (offset_y + text_height > height)
    text_height = height - offset_y;*/
  gimp_image_crop (text_image, text_width, text_height, offset_x, offset_y);
  gint thumb_width = MIN (text_width, 192);
  gint thumb_height = MIN (text_height, 48);
  gint32 text_layer = gimp_image_get_active_layer (text_image);
 
  gint row = 1;
  gint col = 1;

  int i;
  for (i = 0; i < num_fonts; i++) {
    gimp_text_layer_set_font (text_layer, fonts[i]);
    GdkPixbuf *pixbuf = gimp_image_get_thumbnail (text_image, thumb_width, thumb_height, GIMP_PIXBUF_SMALL_CHECKS);

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

    g_signal_connect (event_box, "button_press_event", G_CALLBACK (select_font), fonts[i]);
  }

  gboolean run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (dialog);

  return run;
}

static gboolean
select_font (GtkWidget *event_box, GdkEventButton *event, gchar* font)
{
  tfvals.font = font;
  gint32 text_layer = gimp_image_get_active_layer (preview_image);
  gimp_text_layer_set_font (text_layer, tfvals.font);
  preview_update (preview);
}
