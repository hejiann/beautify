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

typedef enum
{
  BEAUTIFY_EFFECT_NONE,
  BEAUTIFY_EFFECT_SOFT_LIGHT,
  BEAUTIFY_EFFECT_WARM,
  BEAUTIFY_EFFECT_SHARPEN,
  BEAUTIFY_EFFECT_STRONG_CONTRAST,
  BEAUTIFY_EFFECT_SMART_COLOR,
  BEAUTIFY_EFFECT_BLACK_AND_WHITE,
  BEAUTIFY_EFFECT_INVERT,

  BEAUTIFY_EFFECT_RETRO_LOMO,
  BEAUTIFY_EFFECT_GOTHIC_STYLE,
  BEAUTIFY_EFFECT_FILM,
  BEAUTIFY_EFFECT_HDR,
  BEAUTIFY_EFFECT_CLASSIC_HDR,
  BEAUTIFY_EFFECT_IMPRESSION,
  BEAUTIFY_EFFECT_DEEP_BLUE,
  BEAUTIFY_EFFECT_BRONZE,

  BEAUTIFY_EFFECT_LITTLE_FRESH,
  BEAUTIFY_EFFECT_RETRO,
  BEAUTIFY_EFFECT_PINK_LADY,
  BEAUTIFY_EFFECT_ABAO,
  BEAUTIFY_EFFECT_ICE_SPIRIT,
  BEAUTIFY_EFFECT_JAPANESE,
  BEAUTIFY_EFFECT_NEW_JAPANESE,
  BEAUTIFY_EFFECT_OLD_PHOTOS,
  BEAUTIFY_EFFECT_WARM_YELLOW,
  BEAUTIFY_EFFECT_BLUES,
  BEAUTIFY_EFFECT_COLD_GREEN,
  BEAUTIFY_EFFECT_PURPLE_FANTASY,
  BEAUTIFY_EFFECT_COLD_PURPLE,

  BEAUTIFY_EFFECT_BRIGHT_RED,
  BEAUTIFY_EFFECT_CHRISTMAS_EVE,
  BEAUTIFY_EFFECT_ASTRAL,
  BEAUTIFY_EFFECT_PICK_LIGHT,

  BEAUTIFY_EFFECT_PINK_BLUE_GRADIENT,
} BeautifyEffectType;

void run_effect (gint32 image_ID, BeautifyEffectType effect);
