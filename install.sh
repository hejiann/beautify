#!/bin/bash

#gimptool --install beautify.c

#TEXTURES="$HOME/textures"
#[ -d "$TEXTURES" ] && rm -rf $TEXTURES/*
#[ -d "$TEXTURES" ] || mkdir -p "$TEXTURES"

#cp -rf textures/* $TEXTURES/

#gimptool --install rip-border.c

make && make install

