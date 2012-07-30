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

// fix gimp-2.6 issue: Procedure 'gimp-layer-new' has been called with value '100' for argument 'mode'
#include <libgimp/gimpui.h>

#include "beautify-effect.h"
#include "beautify-textures.h"

void
run_effect (gint32 image_ID, BeautifyEffectType effect)
{
  gimp_context_push ();

  gint32 layer = gimp_image_get_active_layer (image_ID);
  gint32 effect_layer = gimp_layer_copy (layer);
  gimp_image_add_layer (image_ID, effect_layer, -1);

  gint width = gimp_image_width (image_ID);
  gint height = gimp_image_height (image_ID);

  switch (effect)
  {
    case BEAUTIFY_EFFECT_SOFT_LIGHT:
      //gimp_layer_set_mode (effect_layer, GIMP_SOFTLIGHT_MODE);
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.018301 * 255,
        0.121569 * 255, 0.140340 * 255,
        0.247059 * 255, 0.293839 * 255,
        0.372549 * 255, 0.436997 * 255,
        0.498039 * 255, 0.572834 * 255,
        0.623529 * 255, 0.704905 * 255,
        0.749020 * 255, 0.822627 * 255,
        0.874510 * 255, 0.918599 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.039216 * 255,
        0.121569 * 255, 0.137287 * 255,
        0.247059 * 255, 0.287181 * 255,
        0.372549 * 255, 0.432968 * 255,
        0.498039 * 255, 0.572484 * 255,
        0.623529 * 255, 0.698476 * 255,
        0.749020 * 255, 0.825490 * 255,
        0.874510 * 255, 0.920394 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.005744 * 255,
        0.121569 * 255, 0.135120 * 255,
        0.247059 * 255, 0.287459 * 255,
        0.372549 * 255, 0.429606 * 255,
        0.498039 * 255, 0.575840 * 255,
        0.623529 * 255, 0.697813 * 255,
        0.749020 * 255, 0.823317 * 255,
        0.874510 * 255, 0.918301 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
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
                                                   GIMP_PDB_IMAGE, image_ID,
                                                   GIMP_PDB_DRAWABLE, effect_layer,
                                                   GIMP_PDB_INT32, 50,
                                                   GIMP_PDB_END);
      gimp_destroy_params (return_vals, nreturn_vals);
    }
      break;
    case BEAUTIFY_EFFECT_STRONG_CONTRAST:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.039216 * 255,
        0.247059 * 255, 0.105882 * 255,
        0.372549 * 255, 0.235294 * 255,
        0.498039 * 255, 0.537255 * 255,
        0.623529 * 255, 0.768627 * 255,
        0.749020 * 255, 0.858824 * 255,
        0.874510 * 255, 0.929412 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.027451 * 255,
        0.247059 * 255, 0.117647 * 255,
        0.372549 * 255, 0.286275 * 255,
        0.498039 * 255, 0.576471 * 255,
        0.623529 * 255, 0.780392 * 255,
        0.749020 * 255, 0.890196 * 255,
        0.874510 * 255, 0.952941 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.050980 * 255,
        0.247059 * 255, 0.133333 * 255,
        0.372549 * 255, 0.294118 * 255,
        0.498039 * 255, 0.568627 * 255,
        0.623529 * 255, 0.772549 * 255,
        0.749020 * 255, 0.874510 * 255,
        0.874510 * 255, 0.941176 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
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
    case BEAUTIFY_EFFECT_BLACK_AND_WHITE:
      gimp_desaturate_full (effect_layer, GIMP_DESATURATE_LUMINOSITY);
      break;
    case BEAUTIFY_EFFECT_INVERT:
      gimp_invert (effect_layer);
      break;
    case BEAUTIFY_EFFECT_RETRO_LOMO:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.009477 * 255,
        0.121569 * 255, 0.066342 * 255,
        0.247059 * 255, 0.211570 * 255,
        0.372549 * 255, 0.391796 * 255,
        0.498039 * 255, 0.576389 * 255,
        0.623529 * 255, 0.745091 * 255,
        0.749020 * 255, 0.875015 * 255,
        0.874510 * 255, 0.959604 * 255,
        1.000000 * 255, 0.989234 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.075980 * 255,
        0.121569 * 255, 0.176692 * 255,
        0.247059 * 255, 0.294329 * 255,
        0.372549 * 255, 0.415297 * 255,
        0.498039 * 255, 0.536491 * 255,
        0.623529 * 255, 0.651230 * 255,
        0.749020 * 255, 0.751355 * 255,
        0.874510 * 255, 0.843675 * 255,
        1.000000 * 255, 0.921772 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.246068 * 255,
        0.121569 * 255, 0.310134 * 255,
        0.247059 * 255, 0.373558 * 255,
        0.372549 * 255, 0.435251 * 255,
        0.498039 * 255, 0.503361 * 255,
        0.623529 * 255, 0.565592 * 255,
        0.749020 * 255, 0.629995 * 255,
        0.874510 * 255, 0.690267 * 255,
        1.000000 * 255, 0.751997 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
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
    case BEAUTIFY_EFFECT_FILM:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.101961 * 255,
        0.121569 * 255, 0.101961 * 255,
        0.247059 * 255, 0.164706 * 255,
        0.372549 * 255, 0.333333 * 255,
        0.498039 * 255, 0.521569 * 255,
        0.623529 * 255, 0.701961 * 255,
        0.749020 * 255, 0.850980 * 255,
        0.874510 * 255, 0.956863 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.192157 * 255,
        0.121569 * 255, 0.192157 * 255,
        0.247059 * 255, 0.192157 * 255,
        0.372549 * 255, 0.329412 * 255,
        0.498039 * 255, 0.517647 * 255,
        0.623529 * 255, 0.698039 * 255,
        0.749020 * 255, 0.850980 * 255,
        0.874510 * 255, 0.956863 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.266667 * 255,
        0.121569 * 255, 0.266667 * 255,
        0.247059 * 255, 0.266667 * 255,
        0.372549 * 255, 0.290196 * 255,
        0.498039 * 255, 0.470588 * 255,
        0.623529 * 255, 0.658824 * 255,
        0.749020 * 255, 0.827451 * 255,
        0.874510 * 255, 0.949020 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_HDR:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.015686 * 255,
        0.247059 * 255, 0.207843 * 255,
        0.372549 * 255, 0.396078 * 255,
        0.498039 * 255, 0.556863 * 255,
        0.623529 * 255, 0.698039 * 255,
        0.749020 * 255, 0.811765 * 255,
        0.874510 * 255, 0.909804 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.015686 * 255,
        0.247059 * 255, 0.200000 * 255,
        0.372549 * 255, 0.384314 * 255,
        0.498039 * 255, 0.541176 * 255,
        0.623529 * 255, 0.682353 * 255,
        0.749020 * 255, 0.800000 * 255,
        0.874510 * 255, 0.901961 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.054902 * 255,
        0.121569 * 255, 0.121569 * 255,
        0.247059 * 255, 0.262745 * 255,
        0.372549 * 255, 0.407843 * 255,
        0.498039 * 255, 0.537255 * 255,
        0.623529 * 255, 0.662745 * 255,
        0.749020 * 255, 0.780392 * 255,
        0.874510 * 255, 0.890196 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_CLASSIC_HDR:
    {
      guint8 red_pts[] = {
        0.0, 0.054902 * 255, 0.121569 * 255, 0.070588 * 255,
        0.247059 * 255, 0.243137 * 255, 0.372549 * 255, 0.407843 * 255,
        0.498039 * 255, 0.552941 * 255, 0.623529 * 255, 0.678431 * 255,
        0.749020 * 255, 0.780392 * 255, 0.874510 * 255, 0.866667 * 255,
        1.000000 * 255, 0.949020 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.007843 * 255, 0.121569 * 255, 0.023529 * 255,
        0.247059 * 255, 0.207843 * 255, 0.372549 * 255, 0.388235 * 255,
        0.498039 * 255, 0.541176 * 255, 0.623529 * 255, 0.682353 * 255,
        0.749020 * 255, 0.796078 * 255, 0.874510 * 255, 0.898039 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.258824 * 255, 0.121569 * 255, 0.294118 * 255,
        0.247059 * 255, 0.372549 * 255, 0.372549 * 255, 0.450980 * 255,
        0.498039 * 255, 0.521569 * 255, 0.623529 * 255, 0.592157 * 255,
        0.749020 * 255, 0.654902 * 255, 0.874510 * 255, 0.717647 * 255,
        1.000000 * 255, 0.776471 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_YELLOWING_DARK_CORNERS:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.093137 * 255,
        0.121569 * 255, 0.125134 * 255,
        0.247059 * 255, 0.227000 * 255,
        0.372549 * 255, 0.372794 * 255,
        0.498039 * 255, 0.537491 * 255,
        0.623529 * 255, 0.706434 * 255,
        0.749020 * 255, 0.852155 * 255,
        0.874510 * 255, 0.953969 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.092647 * 255,
        0.121569 * 255, 0.125205 * 255,
        0.247059 * 255, 0.227129 * 255,
        0.372549 * 255, 0.372871 * 255,
        0.498039 * 255, 0.537711 * 255,
        0.623529 * 255, 0.706357 * 255,
        0.749020 * 255, 0.851153 * 255,
        0.874510 * 255, 0.953240 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.029810 * 255,
        0.247059 * 255, 0.143778 * 255,
        0.372549 * 255, 0.305764 * 255,
        0.498039 * 255, 0.488796 * 255,
        0.623529 * 255, 0.672134 * 255,
        0.749020 * 255, 0.833704 * 255,
        0.874510 * 255, 0.948010 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_yellowing_dark_corners, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_MULTIPLY_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_IMPRESSION:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.113725 * 255,
        0.121569 * 255, 0.213975 * 255,
        0.247059 * 255, 0.323494 * 255,
        0.372549 * 255, 0.460137 * 255,
        0.498039 * 255, 0.621504 * 255,
        0.623529 * 255, 0.716847 * 255,
        0.749020 * 255, 0.766909 * 255,
        0.874510 * 255, 0.817666 * 255,
        1.000000 * 255, 0.862745 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.200000 * 255,
        0.121569 * 255, 0.317329 * 255,
        0.247059 * 255, 0.407881 * 255,
        0.372549 * 255, 0.535429 * 255,
        0.498039 * 255, 0.682828 * 255,
        0.623529 * 255, 0.770688 * 255,
        0.749020 * 255, 0.813005 * 255,
        0.874510 * 255, 0.852891 * 255,
        1.000000 * 255, 0.902716 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.317714 * 255,
        0.121569 * 255, 0.364205 * 255,
        0.247059 * 255, 0.417294 * 255,
        0.372549 * 255, 0.495841 * 255,
        0.498039 * 255, 0.612710 * 255,
        0.623529 * 255, 0.719834 * 255,
        0.749020 * 255, 0.795937 * 255,
        0.874510 * 255, 0.845977 * 255,
        1.000000 * 255, 0.883024 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_DEEP_BLUE_TEAR_RAIN:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.094118 * 255,
        0.247059 * 255, 0.254902 * 255,
        0.372549 * 255, 0.439216 * 255,
        0.498039 * 255, 0.603922 * 255,
        0.623529 * 255, 0.745098 * 255,
        0.749020 * 255, 0.850980 * 255,
        0.874510 * 255, 0.941176 * 255,
        1.000000 * 255, 0.992157 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.019608 * 255,
        0.121569 * 255, 0.164706 * 255,
        0.247059 * 255, 0.337255 * 255,
        0.372549 * 255, 0.509804 * 255,
        0.498039 * 255, 0.666667 * 255,
        0.623529 * 255, 0.784314 * 255,
        0.749020 * 255, 0.878431 * 255,
        0.874510 * 255, 0.941176 * 255,
        1.000000 * 255, 0.992157 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.133333 * 255,
        0.121569 * 255, 0.333333 * 255,
        0.247059 * 255, 0.494118 * 255,
        0.372549 * 255, 0.643137 * 255,
        0.498039 * 255, 0.760784 * 255,
        0.623529 * 255, 0.850980 * 255,
        0.749020 * 255, 0.913725 * 255,
        0.874510 * 255, 0.964706 * 255,
        1.000000 * 255, 0.988235 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_PURPLE_SENSATION:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.003922 * 255,
        0.247059 * 255, 0.149020 * 255,
        0.372549 * 255, 0.345098 * 255,
        0.498039 * 255, 0.541176 * 255,
        0.623529 * 255, 0.725490 * 255,
        0.749020 * 255, 0.870588 * 255,
        0.874510 * 255, 0.960784 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.003922 * 255,
        0.247059 * 255, 0.003922 * 255,
        0.372549 * 255, 0.274510 * 255,
        0.498039 * 255, 0.521569 * 255,
        0.623529 * 255, 0.713725 * 255,
        0.749020 * 255, 0.862745 * 255,
        0.874510 * 255, 0.952941 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.070588 * 255,
        0.247059 * 255, 0.313725 * 255,
        0.372549 * 255, 0.513725 * 255,
        0.498039 * 255, 0.682353 * 255,
        0.623529 * 255, 0.811765 * 255,
        0.749020 * 255, 0.905882 * 255,
        0.874510 * 255, 0.968627 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_BRONZE:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.078431 * 255,
        0.247059 * 255, 0.196078 * 255,
        0.372549 * 255, 0.341176 * 255,
        0.498039 * 255, 0.494118 * 255,
        0.623529 * 255, 0.650980 * 255,
        0.749020 * 255, 0.729412 * 255,
        0.874510 * 255, 0.729412 * 255,
        1.000000 * 255, 0.729412 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.078431 * 255,
        0.247059 * 255, 0.196078 * 255,
        0.372549 * 255, 0.341176 * 255,
        0.498039 * 255, 0.494118 * 255,
        0.623529 * 255, 0.650980 * 255,
        0.749020 * 255, 0.792157 * 255,
        0.874510 * 255, 0.913725 * 255,
        1.000000 * 255, 0.925490 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.450980 * 255,
        0.121569 * 255, 0.450980 * 255,
        0.247059 * 255, 0.450980 * 255,
        0.372549 * 255, 0.450980 * 255,
        0.498039 * 255, 0.494118 * 255,
        0.623529 * 255, 0.650980 * 255,
        0.749020 * 255, 0.792157 * 255,
        0.874510 * 255, 0.913725 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_RECALL:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_recall, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_MULTIPLY_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);
      break;
    }
    case BEAUTIFY_EFFECT_ELEGANT:
    {
      gimp_hue_saturation (effect_layer, GIMP_ALL_HUES, 0, 0, -40);

      guint8 red_pts[] = {
        0.000000 * 255, 0.000171 * 255,
        0.121569 * 255, 0.047560 * 255,
        0.247059 * 255, 0.162212 * 255,
        0.372549 * 255, 0.329729 * 255,
        0.498039 * 255, 0.520206 * 255,
        0.623529 * 255, 0.704597 * 255,
        0.749020 * 255, 0.849098 * 255,
        0.874510 * 255, 0.971076 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.063296 * 255,
        0.247059 * 255, 0.177207 * 255,
        0.372549 * 255, 0.326921 * 255,
        0.498039 * 255, 0.494137 * 255,
        0.623529 * 255, 0.653138 * 255,
        0.749020 * 255, 0.799763 * 255,
        0.874510 * 255, 0.903914 * 255,
        1.000000 * 255, 0.949020 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.031373 * 255,
        0.121569 * 255, 0.103265 * 255,
        0.247059 * 255, 0.214688 * 255,
        0.372549 * 255, 0.347533 * 255,
        0.498039 * 255, 0.485165 * 255,
        0.623529 * 255, 0.619805 * 255,
        0.749020 * 255, 0.751388 * 255,
        0.874510 * 255, 0.868222 * 255,
        1.000000 * 255, 0.949020 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      break;
    }
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
    case BEAUTIFY_EFFECT_CLASSIC_STUDIO:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.002941 * 255,
        0.121569 * 255, 0.105177 * 255,
        0.247059 * 255, 0.276869 * 255,
        0.372549 * 255, 0.449951 * 255,
        0.498039 * 255, 0.615011 * 255,
        0.623529 * 255, 0.765528 * 255,
        0.749020 * 255, 0.884498 * 255,
        0.874510 * 255, 0.964439 * 255,
        1.000000 * 255, 0.996641 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.000980 * 255,
        0.121569 * 255, 0.023976 * 255,
        0.247059 * 255, 0.117564 * 255,
        0.372549 * 255, 0.268570 * 255,
        0.498039 * 255, 0.450785 * 255,
        0.623529 * 255, 0.640827 * 255,
        0.749020 * 255, 0.821280 * 255,
        0.874510 * 255, 0.944143 * 255,
        1.000000 * 255, 0.994046 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.001705 * 255,
        0.121569 * 255, 0.091176 * 255,
        0.247059 * 255, 0.255272 * 255,
        0.372549 * 255, 0.426934 * 255,
        0.498039 * 255, 0.599930 * 255,
        0.623529 * 255, 0.749604 * 255,
        0.749020 * 255, 0.879809 * 255,
        0.874510 * 255, 0.963030 * 255,
        1.000000 * 255, 0.994565 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_RETRO:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.011765 * 255,
        0.121569 * 255, 0.050275 * 255,
        0.247059 * 255, 0.163976 * 255,
        0.372549 * 255, 0.316983 * 255,
        0.498039 * 255, 0.493141 * 255,
        0.623529 * 255, 0.671170 * 255,
        0.749020 * 255, 0.829955 * 255,
        0.874510 * 255, 0.941938 * 255,
        1.000000 * 255, 0.988797 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.044118 * 255,
        0.121569 * 255, 0.081048 * 255,
        0.247059 * 255, 0.181188 * 255,
        0.372549 * 255, 0.327417 * 255,
        0.498039 * 255, 0.493717 * 255,
        0.623529 * 255, 0.658936 * 255,
        0.749020 * 255, 0.811563 * 255,
        0.874510 * 255, 0.915557 * 255,
        1.000000 * 255, 0.956299 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.247630 * 255,
        0.121569 * 255, 0.268491 * 255,
        0.247059 * 255, 0.325230 * 255,
        0.372549 * 255, 0.405204 * 255,
        0.498039 * 255, 0.497829 * 255,
        0.623529 * 255, 0.588839 * 255,
        0.749020 * 255, 0.675181 * 255,
        0.874510 * 255, 0.731610 * 255,
        1.000000 * 255, 0.752075 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_PINK_LADY:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.196078 * 255,
        0.247059 * 255, 0.356863 * 255,
        0.372549 * 255, 0.509804 * 255,
        0.498039 * 255, 0.647059 * 255,
        0.623529 * 255, 0.760784 * 255,
        0.749020 * 255, 0.858824 * 255,
        0.874510 * 255, 0.937255 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.180392 * 255,
        0.247059 * 255, 0.329412 * 255,
        0.372549 * 255, 0.478431 * 255,
        0.498039 * 255, 0.611765 * 255,
        0.623529 * 255, 0.729412 * 255,
        0.749020 * 255, 0.831373 * 255,
        0.874510 * 255, 0.921569 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.168627 * 255,
        0.247059 * 255, 0.317647 * 255,
        0.372549 * 255, 0.458824 * 255,
        0.498039 * 255, 0.592157 * 255,
        0.623529 * 255, 0.709804 * 255,
        0.749020 * 255, 0.819608 * 255,
        0.874510 * 255, 0.913725 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_ABAO_COLOR:
    {
      /*GimpPixelRgn  src_rgn, dest_rgn;
      gint          x1, y1;
      gint          width, height;
      gpointer      pr;

      GimpDrawable *drawable = gimp_drawable_get (effect_layer);
      if (!gimp_drawable_mask_intersect (drawable->drawable_id,
                                         &x1, &y1, &width, &height))
        return;

      gimp_pixel_rgn_init (&src_rgn, drawable, x1, y1, width, height, FALSE, FALSE);
      gimp_pixel_rgn_init (&dest_rgn, drawable, x1, y1, width, height, TRUE, TRUE);
      for (pr = gimp_pixel_rgns_register (2, &src_rgn, &dest_rgn);
           pr != NULL;
           pr = gimp_pixel_rgns_process (pr))
      {
        const guchar *src = src_rgn.data;
        guchar       *dest = dest_rgn.data;
        gint          x, y;

        for (y = 0; y < src_rgn.h; y++)
        {
          const guchar *s = src;
          guchar *d = dest;

          for (x = 0; x < src_rgn.w; x++)
          {
            GimpRGB rgb = {s[0], s[1], s[2], 1};
            GimpHSV hsv;
            gimp_rgb_to_hsv (&rgb, &hsv);
            if (hsv.h <= 0.1) {
              hsv.s = (0.1 - hsv.h) / 0.1;
            }
            gimp_hsv_to_rgb (&hsv, &rgb);
            d[0] = rgb.r;
            d[1] = rgb.g;
            d[2] = rgb.b;
            
            s += src_rgn.bpp;
            d += dest_rgn.bpp;
          }

          src += src_rgn.rowstride;
          dest += dest_rgn.rowstride;
        }
      }

      gimp_drawable_flush (drawable);
      gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
      gimp_drawable_update (drawable->drawable_id, x1, y1, width, height);*/

      break;
    }
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
    case BEAUTIFY_EFFECT_JAPANESE_STYLE:
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
    case BEAUTIFY_EFFECT_NEW_JAPANESE_STYLE:
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
    case BEAUTIFY_EFFECT_MILK:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_milk, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 20, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      guint8 red_pts[] = {
        0.000000 * 255, 0.309804 * 255,
        0.121569 * 255, 0.392252 * 255,
        0.247059 * 255, 0.496494 * 255,
        0.372549 * 255, 0.597570 * 255,
        0.498039 * 255, 0.684511 * 255,
        0.623529 * 255, 0.768060 * 255,
        0.749020 * 255, 0.843330 * 255,
        0.874510 * 255, 0.917231 * 255,
        1.000000 * 255, 0.992213 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.299346 * 255,
        0.121569 * 255, 0.395641 * 255,
        0.247059 * 255, 0.498204 * 255,
        0.372549 * 255, 0.597187 * 255,
        0.498039 * 255, 0.680991 * 255,
        0.623529 * 255, 0.762936 * 255,
        0.749020 * 255, 0.841731 * 255,
        0.874510 * 255, 0.910444 * 255,
        1.000000 * 255, 0.980896 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.296899 * 255,
        0.121569 * 255, 0.400465 * 255,
        0.247059 * 255, 0.501991 * 255,
        0.372549 * 255, 0.598690 * 255,
        0.498039 * 255, 0.685437 * 255,
        0.623529 * 255, 0.766052 * 255,
        0.749020 * 255, 0.842140 * 255,
        0.874510 * 255, 0.913442 * 255,
        1.000000 * 255, 0.981385 * 255,
      };
      layer = gimp_image_get_active_layer (image_ID);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_OLD_PHOTOS:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.030719 * 255,
        0.121569 * 255, 0.094286 * 255,
        0.247059 * 255, 0.215434 * 255,
        0.372549 * 255, 0.446688 * 255,
        0.498039 * 255, 0.744585 * 255,
        0.623529 * 255, 0.854446 * 255,
        0.749020 * 255, 0.909348 * 255,
        0.874510 * 255, 0.948601 * 255,
        1.000000 * 255, 0.975656 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.111275 * 255,
        0.121569 * 255, 0.172338 * 255,
        0.247059 * 255, 0.301636 * 255,
        0.372549 * 255, 0.462942 * 255,
        0.498039 * 255, 0.615155 * 255,
        0.623529 * 255, 0.736568 * 255,
        0.749020 * 255, 0.807584 * 255,
        0.874510 * 255, 0.874171 * 255,
        1.000000 * 255, 0.892938 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.333451 * 255,
        0.121569 * 255, 0.423250 * 255,
        0.247059 * 255, 0.517089 * 255,
        0.372549 * 255, 0.591683 * 255,
        0.498039 * 255, 0.631057 * 255,
        0.623529 * 255, 0.658333 * 255,
        0.749020 * 255, 0.678149 * 255,
        0.874510 * 255, 0.678550 * 255,
        1.000000 * 255, 0.677872 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_old_photos, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_WARM_YELLOW:
    {
      guint8 red_pts[] = {
        0.0, 0.000980 * 255, 0.121569 * 255, 0.065574 * 255,
        0.247059 * 255, 0.213677 * 255, 0.372549 * 255, 0.383298 * 255,
        0.498039 * 255, 0.556855 * 255, 0.623529 * 255, 0.726149 * 255,
        0.749020 * 255, 0.864046 * 255, 0.874510 * 255, 0.958157 * 255,
        1.000000 * 255, 0.996641 * 255,
      };
      guint8 green_pts[] = {
        0.0, 0.005882 * 255, 0.121569 * 255, 0.107837 * 255,
        0.247059 * 255, 0.276792 * 255, 0.372549 * 255, 0.452811 * 255,
        0.498039 * 255, 0.617782 * 255, 0.623529 * 255, 0.763782 * 255,
        0.749020 * 255, 0.886822 * 255, 0.874510 * 255, 0.965223 * 255,
        1.000000 * 255, 0.996993 * 255,
      };
      guint8 blue_pts[] = {
        0.0, 0.000495 * 255, 0.121569 * 255, 0.035825 * 255,
        0.247059 * 255, 0.149480 * 255, 0.372549 * 255, 0.305398 * 255,
        0.498039 * 255, 0.491352 * 255, 0.623529 * 255, 0.670305 * 255,
        0.749020 * 255, 0.838898 * 255, 0.874510 * 255, 0.951301 * 255,
        1.000000 * 255, 0.994118 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
    }
      break;
    case BEAUTIFY_EFFECT_BLUES:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.321569 * 255,
        0.247059 * 255, 0.541176 * 255,
        0.372549 * 255, 0.713725 * 255,
        0.498039 * 255, 0.831373 * 255,
        0.623529 * 255, 0.905882 * 255,
        0.749020 * 255, 0.952941 * 255,
        0.874510 * 255, 0.980392 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.266667 * 255,
        0.247059 * 255, 0.466667 * 255,
        0.372549 * 255, 0.627451 * 255,
        0.498039 * 255, 0.756863 * 255,
        0.623529 * 255, 0.847059 * 255,
        0.749020 * 255, 0.917647 * 255,
        0.874510 * 255, 0.964706 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.286275 * 255,
        0.247059 * 255, 0.505882 * 255,
        0.372549 * 255, 0.682353 * 255,
        0.498039 * 255, 0.811765 * 255,
        0.623529 * 255, 0.901961 * 255,
        0.749020 * 255, 0.960784 * 255,
        0.874510 * 255, 0.988235 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_COLD_BLUE:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.000040 * 255,
        0.121569 * 255, 0.008043 * 255,
        0.247059 * 255, 0.066237 * 255,
        0.372549 * 255, 0.184771 * 255,
        0.498039 * 255, 0.359165 * 255,
        0.623529 * 255, 0.568527 * 255,
        0.749020 * 255, 0.772928 * 255,
        0.874510 * 255, 0.927927 * 255,
        1.000000 * 255, 0.993301 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.001961 * 255,
        0.121569 * 255, 0.069848 * 255,
        0.247059 * 255, 0.214185 * 255,
        0.372549 * 255, 0.386385 * 255,
        0.498039 * 255, 0.560533 * 255,
        0.623529 * 255, 0.722231 * 255,
        0.749020 * 255, 0.865600 * 255,
        0.874510 * 255, 0.958973 * 255,
        1.000000 * 255, 0.996979 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.006332 * 255,
        0.121569 * 255, 0.225447 * 255,
        0.247059 * 255, 0.425338 * 255,
        0.372549 * 255, 0.585919 * 255,
        0.498039 * 255, 0.724790 * 255,
        0.623529 * 255, 0.833296 * 255,
        0.749020 * 255, 0.922240 * 255,
        0.874510 * 255, 0.975900 * 255,
        1.000000 * 255, 0.995237 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_COLD_GREEN:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.000058 * 255,
        0.121569 * 255, 0.013332 * 255,
        0.247059 * 255, 0.092201 * 255,
        0.372549 * 255, 0.228389 * 255,
        0.498039 * 255, 0.407339 * 255,
        0.623529 * 255, 0.610095 * 255,
        0.749020 * 255, 0.797573 * 255,
        0.874510 * 255, 0.937331 * 255,
        1.000000 * 255, 0.993303 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.008824 * 255,
        0.121569 * 255, 0.140109 * 255,
        0.247059 * 255, 0.324052 * 255,
        0.372549 * 255, 0.497982 * 255,
        0.498039 * 255, 0.655209 * 255,
        0.623529 * 255, 0.789734 * 255,
        0.749020 * 255, 0.900202 * 255,
        0.874510 * 255, 0.969304 * 255,
        1.000000 * 255, 0.997007 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.000495 * 255,
        0.121569 * 255, 0.035825 * 255,
        0.247059 * 255, 0.149480 * 255,
        0.372549 * 255, 0.305398 * 255,
        0.498039 * 255, 0.491352 * 255,
        0.623529 * 255, 0.670305 * 255,
        0.749020 * 255, 0.838898 * 255,
        0.874510 * 255, 0.951301 * 255,
        1.000000 * 255, 0.994118 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_PURPLE_FANTASY:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.184314 * 255,
        0.247059 * 255, 0.376471 * 255,
        0.372549 * 255, 0.533333 * 255,
        0.498039 * 255, 0.662745 * 255,
        0.623529 * 255, 0.788235 * 255,
        0.749020 * 255, 0.878431 * 255,
        0.874510 * 255, 0.941176 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.113725 * 255,
        0.247059 * 255, 0.243137 * 255,
        0.372549 * 255, 0.407843 * 255,
        0.498039 * 255, 0.623529 * 255,
        0.623529 * 255, 0.760784 * 255,
        0.749020 * 255, 0.847059 * 255,
        0.874510 * 255, 0.925490 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.309804 * 255,
        0.247059 * 255, 0.505882 * 255,
        0.372549 * 255, 0.603922 * 255,
        0.498039 * 255, 0.709804 * 255,
        0.623529 * 255, 0.784314 * 255,
        0.749020 * 255, 0.854902 * 255,
        0.874510 * 255, 0.929412 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_COLD_PURPLE:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.004412 * 255,
        0.121569 * 255, 0.137411 * 255,
        0.247059 * 255, 0.308078 * 255,
        0.372549 * 255, 0.470911 * 255,
        0.498039 * 255, 0.617224 * 255,
        0.623529 * 255, 0.745202 * 255,
        0.749020 * 255, 0.851077 * 255,
        0.874510 * 255, 0.936608 * 255,
        1.000000 * 255, 0.991056 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.033824 * 255,
        0.121569 * 255, 0.044738 * 255,
        0.247059 * 255, 0.161690 * 255,
        0.372549 * 255, 0.319742 * 255,
        0.498039 * 255, 0.492767 * 255,
        0.623529 * 255, 0.662258 * 255,
        0.749020 * 255, 0.830133 * 255,
        0.874510 * 255, 0.940380 * 255,
        1.000000 * 255, 0.996802 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.140723 * 255,
        0.121569 * 255, 0.295178 * 255,
        0.247059 * 255, 0.504249 * 255,
        0.372549 * 255, 0.648502 * 255,
        0.498039 * 255, 0.757808 * 255,
        0.623529 * 255, 0.853601 * 255,
        0.749020 * 255, 0.928529 * 255,
        0.874510 * 255, 0.975401 * 255,
        1.000000 * 255, 0.992089 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_BRIGHT_RED:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_bright_red, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      guint8 red_pts[] = {
        0.000000 * 255, 0.001183 * 255,
        0.121569 * 255, 0.131140 * 255,
        0.247059 * 255, 0.353431 * 255,
        0.372549 * 255, 0.538498 * 255,
        0.498039 * 255, 0.690185 * 255,
        0.623529 * 255, 0.804008 * 255,
        0.749020 * 255, 0.900806 * 255,
        0.874510 * 255, 0.988271 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.000098 * 255,
        0.121569 * 255, 0.097098 * 255,
        0.247059 * 255, 0.286323 * 255,
        0.372549 * 255, 0.458654 * 255,
        0.498039 * 255, 0.611045 * 255,
        0.623529 * 255, 0.744221 * 255,
        0.749020 * 255, 0.858522 * 255,
        0.874510 * 255, 0.968540 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.000369 * 255,
        0.121569 * 255, 0.100320 * 255,
        0.247059 * 255, 0.285806 * 255,
        0.372549 * 255, 0.459693 * 255,
        0.498039 * 255, 0.612676 * 255,
        0.623529 * 255, 0.745027 * 255,
        0.749020 * 255, 0.860280 * 255,
        0.874510 * 255, 0.967918 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      layer = gimp_image_get_active_layer (image_ID);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
    case BEAUTIFY_EFFECT_CHRISTMAS_EVE:
    {
      gint32 layer = gimp_layer_new (image_ID, "color", width, height, GIMP_RGB_IMAGE, 100, GIMP_OVERLAY_MODE);
      gimp_image_add_layer (image_ID, layer, -1);
      GimpRGB color =
      {
        (gdouble) 156.0 / 255.0,
        (gdouble) 208.0 / 255.0,
        (gdouble) 240.0 / 255.0,
        1.0,
      };
      gimp_context_set_foreground (&color);
      gimp_edit_fill (layer, GIMP_FOREGROUND_FILL);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline (-1, texture_christmas_eve, FALSE, NULL);
      gint32 texture_layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, texture_layer, -1);
      gimp_layer_scale (texture_layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, texture_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
      break;
    }
    case BEAUTIFY_EFFECT_NIGHT_VIEW:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.181978 * 255,
        0.247059 * 255, 0.348183 * 255,
        0.372549 * 255, 0.498116 * 255,
        0.498039 * 255, 0.630567 * 255,
        0.623529 * 255, 0.746150 * 255,
        0.749020 * 255, 0.848447 * 255,
        0.874510 * 255, 0.939235 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.002451 * 255,
        0.121569 * 255, 0.182281 * 255,
        0.247059 * 255, 0.347678 * 255,
        0.372549 * 255, 0.495968 * 255,
        0.498039 * 255, 0.629733 * 255,
        0.623529 * 255, 0.747508 * 255,
        0.749020 * 255, 0.852278 * 255,
        0.874510 * 255, 0.937573 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.005240 * 255,
        0.121569 * 255, 0.182529 * 255,
        0.247059 * 255, 0.347919 * 255,
        0.372549 * 255, 0.496757 * 255,
        0.498039 * 255, 0.638620 * 255,
        0.623529 * 255, 0.753394 * 255,
        0.749020 * 255, 0.852676 * 255,
        0.874510 * 255, 0.948770 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_night_view, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_ASTRAL:
    {
      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline (-1, texture_astral, FALSE, NULL);
      gint32 texture_layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SOFTLIGHT_MODE, 0, 0);
      gimp_image_add_layer (image_ID, texture_layer, -1);
      gimp_layer_scale (texture_layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, texture_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
    }
      break;
    case BEAUTIFY_EFFECT_COLORFUL_GLOW:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.151307 * 255,
        0.121569 * 255, 0.322524 * 255,
        0.247059 * 255, 0.448847 * 255,
        0.372549 * 255, 0.575643 * 255,
        0.498039 * 255, 0.686567 * 255,
        0.623529 * 255, 0.782251 * 255,
        0.749020 * 255, 0.867647 * 255,
        0.874510 * 255, 0.945974 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.141176 * 255,
        0.121569 * 255, 0.293870 * 255,
        0.247059 * 255, 0.430463 * 255,
        0.372549 * 255, 0.557343 * 255,
        0.498039 * 255, 0.673448 * 255,
        0.623529 * 255, 0.772324 * 255,
        0.749020 * 255, 0.849510 * 255,
        0.874510 * 255, 0.933274 * 255,
        1.000000 * 255, 0.991523 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.141529 * 255,
        0.121569 * 255, 0.292286 * 255,
        0.247059 * 255, 0.434369 * 255,
        0.372549 * 255, 0.557046 * 255,
        0.498039 * 255, 0.669468 * 255,
        0.623529 * 255, 0.772360 * 255,
        0.749020 * 255, 0.858682 * 255,
        0.874510 * 255, 0.932288 * 255,
        1.000000 * 255, 0.987544 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_colorful_glow, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_PICK_LIGHT:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      layer = gimp_layer_new (image_ID, "color", width, height, GIMP_RGB_IMAGE, 100, GIMP_SCREEN_MODE);
      gimp_image_add_layer (image_ID, layer, -1);
      GimpRGB color =
      {
        (gdouble) 62.0 / 255.0,
        (gdouble) 62.0 / 255.0,
        (gdouble) 62.0 / 255.0,
        1.0,
      };
      gimp_context_set_foreground (&color);
      gimp_edit_fill (layer, GIMP_FOREGROUND_FILL);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_pick_light_1, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture 1", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_pick_light_2, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture 2", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);
      break;
    }
    case BEAUTIFY_EFFECT_GLASS_DROPS:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_glass_drops, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_SKETCH:
    {
      gint32     layer;

      gimp_desaturate_full (effect_layer, GIMP_DESATURATE_LUMINOSITY);
      layer = gimp_layer_copy (effect_layer);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_set_mode  (layer, GIMP_DODGE_MODE);
      gimp_invert (layer);

      GimpParam *return_vals;
      gint nreturn_vals;
      return_vals = gimp_run_procedure ("plug-in-gauss",
                                        &nreturn_vals,
                                        GIMP_PDB_INT32, GIMP_RUN_NONINTERACTIVE,
                                        GIMP_PDB_IMAGE, image_ID,
                                        GIMP_PDB_DRAWABLE, layer,
                                        GIMP_PDB_FLOAT, 20.0,
                                        GIMP_PDB_FLOAT, 20.0,
                                        GIMP_PDB_INT32, 1,
                                        GIMP_PDB_END);
      gimp_destroy_params (return_vals, nreturn_vals);

      gimp_levels (layer, GIMP_HISTOGRAM_VALUE, 0, 255, 1, 0, 240);

      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);
      
      break;
    }
    case BEAUTIFY_EFFECT_LIFE_SKETCH:
    {
      gimp_desaturate_full (effect_layer, GIMP_DESATURATE_LUMINOSITY);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_life_sketch_1, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 60, GIMP_OVERLAY_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_life_sketch_2, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      guint8 red_pts[] = {
        0.000000 * 255, 0.074510 * 255,
        0.121569 * 255, 0.105882 * 255,
        0.247059 * 255, 0.203391 * 255,
        0.372549 * 255, 0.325314 * 255,
        0.498039 * 255, 0.451172 * 255,
        0.623529 * 255, 0.582024 * 255,
        0.749020 * 255, 0.717003 * 255,
        0.874510 * 255, 0.855324 * 255,
        1.000000 * 255, 0.995987 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.070588 * 255,
        0.121569 * 255, 0.102941 * 255,
        0.247059 * 255, 0.205857 * 255,
        0.372549 * 255, 0.330846 * 255,
        0.498039 * 255, 0.457491 * 255,
        0.623529 * 255, 0.591683 * 255,
        0.749020 * 255, 0.724243 * 255,
        0.874510 * 255, 0.859604 * 255,
        1.000000 * 255, 0.996069 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.062745 * 255,
        0.121569 * 255, 0.110294 * 255,
        0.247059 * 255, 0.223629 * 255,
        0.372549 * 255, 0.355159 * 255,
        0.498039 * 255, 0.490793 * 255,
        0.623529 * 255, 0.626121 * 255,
        0.749020 * 255, 0.753347 * 255,
        0.874510 * 255, 0.877884 * 255,
        1.000000 * 255, 0.996034 * 255,
      };
      layer = gimp_image_get_active_layer (image_ID);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      break;
    }
    case BEAUTIFY_EFFECT_COLOR_PENCIL:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_color_pencil, FALSE, NULL);

      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_OVERLAY_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      break;
    }
    case BEAUTIFY_EFFECT_BEAM_GRADIENT:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.003922 * 255,
        0.121569 * 255, 0.041976 * 255,
        0.247059 * 255, 0.156522 * 255,
        0.372549 * 255, 0.317488 * 255,
        0.498039 * 255, 0.499707 * 255,
        0.623529 * 255, 0.684590 * 255,
        0.749020 * 255, 0.845234 * 255,
        0.874510 * 255, 0.957891 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.005882 * 255,
        0.121569 * 255, 0.041838 * 255,
        0.247059 * 255, 0.156351 * 255,
        0.372549 * 255, 0.317066 * 255,
        0.498039 * 255, 0.499800 * 255,
        0.623529 * 255, 0.684389 * 255,
        0.749020 * 255, 0.843945 * 255,
        0.874510 * 255, 0.958236 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.005064 * 255,
        0.121569 * 255, 0.043674 * 255,
        0.247059 * 255, 0.157355 * 255,
        0.372549 * 255, 0.318250 * 255,
        0.498039 * 255, 0.499405 * 255,
        0.623529 * 255, 0.683918 * 255,
        0.749020 * 255, 0.844833 * 255,
        0.874510 * 255, 0.957540 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (effect_layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);

      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_beam_gradient, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);
      break;
    }
    case BEAUTIFY_EFFECT_PINK_BLUE_GRADIENT:
    {
      gint32     layer;
      GdkPixbuf *pixbuf;

      pixbuf = gdk_pixbuf_new_from_inline (-1, texture_pink_blue_gradient, FALSE, NULL);
      layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SCREEN_MODE, 0, 0);
      gimp_image_add_layer (image_ID, layer, -1);
      gimp_layer_scale (layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, layer, GIMP_CLIP_TO_BOTTOM_LAYER);

      guint8 red_pts[] = {
        0.000000 * 255, 0.000392 * 255,
        0.121569 * 255, 0.038765 * 255,
        0.247059 * 255, 0.163053 * 255,
        0.372549 * 255, 0.298924 * 255,
        0.498039 * 255, 0.443050 * 255,
        0.623529 * 255, 0.612325 * 255,
        0.749020 * 255, 0.790453 * 255,
        0.874510 * 255, 0.939548 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 green_pts[] = {
        0.000000 * 255, 0.001569 * 255,
        0.121569 * 255, 0.044421 * 255,
        0.247059 * 255, 0.161120 * 255,
        0.372549 * 255, 0.300417 * 255,
        0.498039 * 255, 0.478406 * 255,
        0.623529 * 255, 0.664685 * 255,
        0.749020 * 255, 0.828593 * 255,
        0.874510 * 255, 0.954235 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      guint8 blue_pts[] = {
        0.000000 * 255, 0.260784 * 255,
        0.121569 * 255, 0.260784 * 255,
        0.247059 * 255, 0.260784 * 255,
        0.372549 * 255, 0.327074 * 255,
        0.498039 * 255, 0.447592 * 255,
        0.623529 * 255, 0.589356 * 255,
        0.749020 * 255, 0.776297 * 255,
        0.874510 * 255, 0.939428 * 255,
        1.000000 * 255, 1.000000 * 255,
      };
      layer = gimp_image_get_active_layer (image_ID);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, green_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, blue_pts);
      break;
    }
  }

  gimp_context_pop ();
}

