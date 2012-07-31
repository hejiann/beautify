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
  WHITENING_EFFECT_NONE,
  WHITENING_EFFECT_LITTLE_WHITENING,
  WHITENING_EFFECT_MODERATE_WHITENING,
  WHITENING_EFFECT_HIGH_WHITENING,
  WHITENING_EFFECT_LITTLE_PINK,
  WHITENING_EFFECT_MODERATE_PINK,
  WHITENING_EFFECT_HIGH_PINK,
  WHITENING_EFFECT_LITTLE_FLESH,
  WHITENING_EFFECT_MODERATE_FLESH,
  WHITENING_EFFECT_HIGH_FLESH, // TODO
} WhiteningEffectType;

void run_effect (gint32 image_ID, WhiteningEffectType effect);

