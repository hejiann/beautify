#!/bin/bash

TEXTURES="$HOME/textures"
[ -d "$TEXTURES" ] || mkdir -p "$TEXTURES"

cp textures/* $TEXTURES/ -rf

gimptool --install rip-border.c

