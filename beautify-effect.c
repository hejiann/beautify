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

#if GTK_MAJOR_VERSION < 3
#include <gdk-pixbuf/gdk-pixbuf.h>
#endif

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
    case BEAUTIFY_EFFECT_ASTRAL:
    {
      GdkPixbuf *pixbuf = gdk_pixbuf_new_from_inline (-1, texture_astral, FALSE, NULL);
      gint32 texture_layer = gimp_layer_new_from_pixbuf (image_ID, "texture", pixbuf, 100, GIMP_SOFTLIGHT_MODE, 0, 0);
      gimp_image_add_layer (image_ID, texture_layer, -1);
      gimp_layer_scale (texture_layer, width, height, FALSE);
      gimp_image_merge_down (image_ID, texture_layer, GIMP_CLIP_TO_BOTTOM_LAYER);
    }
      break;
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

