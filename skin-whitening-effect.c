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

#include "skin-whitening-effect.h"

void
run_effect (gint32 image_ID, WhiteningEffectType effect)
{
  gint32 layer = gimp_image_get_active_layer (image_ID);

  switch (effect)
  {
    case WHITENING_EFFECT_LITTLE_WHITENING:
    {
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.160784 * 255,
        0.247059 * 255, 0.317647 * 255,
        0.372549 * 255, 0.462745 * 255,
        0.498039 * 255, 0.592157 * 255,
        0.623529 * 255, 0.713725 * 255,
        0.749020 * 255, 0.819608 * 255,
        0.874510 * 255, 0.913725 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
    case WHITENING_EFFECT_MODERATE_WHITENING:
    {
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.192157 * 255,
        0.247059 * 255, 0.372549 * 255,
        0.372549 * 255, 0.529412 * 255,
        0.498039 * 255, 0.666667 * 255,
        0.623529 * 255, 0.784314 * 255,
        0.749020 * 255, 0.874510 * 255,
        0.874510 * 255, 0.945098 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
    case WHITENING_EFFECT_HIGH_WHITENING:
    {
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.223529 * 255,
        0.247059 * 255, 0.427451 * 255,
        0.372549 * 255, 0.600000 * 255,
        0.498039 * 255, 0.741176 * 255,
        0.623529 * 255, 0.854902 * 255,
        0.749020 * 255, 0.933333 * 255,
        0.874510 * 255, 0.980392 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
    case WHITENING_EFFECT_LITTLE_PINK:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.160784 * 255,
        0.247059 * 255, 0.317647 * 255,
        0.372549 * 255, 0.462745 * 255,
        0.498039 * 255, 0.592157 * 255,
        0.623529 * 255, 0.713725 * 255,
        0.749020 * 255, 0.819608 * 255,
        0.874510 * 255, 0.913725 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.145098 * 255,
        0.247059 * 255, 0.290196 * 255,
        0.372549 * 255, 0.427451 * 255,
        0.498039 * 255, 0.556863 * 255,
        0.623529 * 255, 0.678431 * 255,
        0.749020 * 255, 0.792157 * 255,
        0.874510 * 255, 0.898039 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
    case WHITENING_EFFECT_MODERATE_PINK:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.192157 * 255,
        0.247059 * 255, 0.372549 * 255,
        0.372549 * 255, 0.529412 * 255,
        0.498039 * 255, 0.666667 * 255,
        0.623529 * 255, 0.784314 * 255,
        0.749020 * 255, 0.874510 * 255,
        0.874510 * 255, 0.945098 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.160784 * 255,
        0.247059 * 255, 0.321569 * 255,
        0.372549 * 255, 0.470588 * 255,
        0.498039 * 255, 0.600000 * 255,
        0.623529 * 255, 0.721569 * 255,
        0.749020 * 255, 0.827451 * 255,
        0.874510 * 255, 0.917647 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
    case WHITENING_EFFECT_HIGH_PINK:
    {
      guint8 red_pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.223529 * 255,
        0.247059 * 255, 0.427451 * 255,
        0.372549 * 255, 0.600000 * 255,
        0.498039 * 255, 0.741176 * 255,
        0.623529 * 255, 0.854902 * 255,
        0.749020 * 255, 0.933333 * 255,
        0.874510 * 255, 0.980392 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      guint8 pts[] = {
        0.000000 * 255, 0.007843 * 255,
        0.121569 * 255, 0.180392 * 255,
        0.247059 * 255, 0.356863 * 255,
        0.372549 * 255, 0.513725 * 255,
        0.498039 * 255, 0.647059 * 255,
        0.623529 * 255, 0.764706 * 255,
        0.749020 * 255, 0.862745 * 255,
        0.874510 * 255, 0.937255 * 255,
        1.000000 * 255, 0.996078 * 255,
      };
      gimp_curves_spline (layer, GIMP_HISTOGRAM_RED, 18, red_pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_GREEN, 18, pts);
      gimp_curves_spline (layer, GIMP_HISTOGRAM_BLUE, 18, pts);
      break;
    }
  }
}
