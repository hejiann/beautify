# GIMP Beautify

> GIMP Beautify is a set of GIMP plug-ins for quickly and easily beautify photo.

Thanks all GIMP developers, because they give us a great Image Manipulation Program with a strong plug-in architecture, so we can write great plug-ins doing more fantastic things.

***!!! Important: All the beautify plug-ins has been changed to under the "Filters" menu instead of "Tools" menu.***

# INSTALL

!!! Be sure that you have installed the gimp-devel-tools (or libgimp2.0-dev) package before install.

#### Method 1: Install into mathine directory:
```sh
make
sudo make install
```
#### Method 2: Install into user directory:
```sh
make
make userinstall
```

# INSTALL TEXTURES

You can download and install the textures following this document:
https://github.com/hejiann/beautify/wiki/Textures-Download

# USAGE

**GIMP menu:**
Filters -> Beautify -> Beautify
Filters -> Beautify -> Skin whitening
Filters -> Beautify -> Border -> Simple Border
Filters -> Beautify -> Border -> Rip Border
Filters -> Beautify -> Border -> Texture Border

# SUPPORT

* [Document & Tutorials](https://github.com/hejiann/beautify/wiki)
* IRC:                  #beautify on FreeNode
* [GIMP Plugin Registry](http://registry.gimp.org/node/26835)
* [GIMP Chat](http://www.gimpchat.com/viewtopic.php?f=9&t=4811)

# CHANGELOG
|Date|Fixes|
|:------:|:-------|
|2012-08-12|- Make 0.5.3 release.<br>- simple-border: add the effect ID display.<br>- simple-border: add 32 simple border effects.<br>- simple-border: make it works better on small picture.<br>|
|2012-08-11|- add the Simple Border plug-in.<br>- change simple-border, rip-border, texture-border under the "Beautify -> Border" menu.|
|2012-08-06|- beautify: fix contrast adjustment tool.<br>- beautify: add Definition adjustment tool.|
|2012-08-05|- beautify: fix the Black And White effect.|
|2012-08-04|- beautify: fix the Retro LOMO effect.<br>- beautify: fixed GIMP 2.6 compile issue:  undefined reference to `gimp_image_select_rectangle'|
|2012-08-03|- beautify: fix the Soft Light effect.<br>- beautify: fix the Black And White effect.<br>- beautify: fix the Sketch effect.|
|2012-08-02|- beautify: add the Relief effect.<br>- beautify: add the Soft effect.<br>- beautify: add the TV Lines effect.|
|2012-08-01|- Make 0.5.2 release.<br>- Merge rip-border and texture-border into border.<br>- beautify: add the Classic Sketch effect.|
|2012-07-31|- beautify: fix ABao Color effect.<br>- texture-border: slim the plug-in size by only provide top textures and let other textures can be download.<br>- texture-border: add scrollbar to textures and make the dialog display quickly.<br>- rip-border: slim the plug-in size by only provide top textures and let other textures can be download.<br>- rip-border: add scrollbar to textures and make the dialog display quickly.<br>- skin whitening: add the Little Pink effect.<br>- skin whitening: add the Moderate Pink effect.<br>- skin whitening: add the High Pink effect.<br>- skin whitening: add the Little Flesh effect.<br>- skin whitening: add the Moderate Flesh effect.<br>- skin shitening: add the High Flesh effect.<br>- beautify: add the Classic LOMO effect.<br>- beautify: add the Pink Purple Gradient effect.|
|2012-07-30|- beautify: fix missing effect.<br>- beautify: add the glass drops effect.<br>- beautify: add the life sketch effect.<br>- beautify: add the color pencil effect.<br>- rip-border: add the custom texures support.<br>- beautify: add the sunset gradient effect.<br>- beautify: add the rainbow gradient effect.|
|2012-07-29|- texture-border: add custom texures support.<br>- skin-whitening: add the reset feature.<br>- skin-whitening: fix plug-in-skin-whitening procedure to let this filter can be used in scirpts.<br>- Change all the beautify plug-ins under the "Filters" menu instead of "Tools" menu.<br>- beautify: add the classic studio effect.<br>- beautify: add the night view effect.<br>- beautify: add the colorful glow effect.|
|2012-07-27|- beautify: add the sketch effect<br>- beautify: add the recall effect<br>- beautify: fix the plug-in-beautify procedure to let this plug-in can be used in a script<br>- beautify: fix gimp-2.6 issue: Procedure 'gimp-layer-new' has been called with value '100' for argument 'mode'|
|2012-07-26|- beautify: fix gimp-2.6 issue: undefined reference to `gimp_item_delete'<br>- Made 0.5.1 release.|
|2012-07-25|- beautify: add beam gradient effect<br>- beautify: add reset feature|
|2012-07-24|- beautify: add deep blue effect <br>- beautify: add bronze effect <br>- beautify: add milk effect <br>- beautify: add elegant effect <br>- beautify: add yellowing dark corners effect <br>- beautify: add purple sensation effect <br>- beautify: add cold blue effect|
|2012-07-23|- beautify: fix soft light effect<br>- beautify: add pink blue gradient effect<br>- fix compatable with gtk2|
|2012-07-22|- beautify: add film effect<br>- beautify: add cold green effect|
|2012-07-21|- beautify: add retro effect|
|2012-07-20|- fix make install in x86_64<br>- beautify refactor: split the effect code into beautify-feature.c<br>- beautify: add HDR effect<br>- provide make userinstall to install the plugin into user directory<br>- add make uninstall and make useruninstall<br>- beautify: add black and white effect<br>- beautify: add retro LOMO effect<br>- beautify: add cold purple effect<br>- beautify: add old photos effect<br>- add texture border plugin|
|2012-07-19|- beautify: fix memory leak<br>- beautify: speed up dialog display<br>- fix Makefile: install plugin into mathine directory instead of user directory|
|2012-07-18|- beautify: add blues effect<br>- beautify: add pink lady effect<br>- beautify: add pick light effect<br>- beautify: add purple fantasy effect<br>- beautify: add bright red effect<br>- beautify: add strong contrast effect|
|2012-07-17|- beautify: add impression effect<br>- fix some small bugs<br>- beautify: add christmas eve effect|
|2012-07-16|- add Makefile<br>- add skin whitening plugin|
|2012-07-15|- beautify: add warm yellow effect<br>- beautify: add classic HDR effect<br>- beautify: add effect opacity|
|2012-07-14|- beautify: add smart color effect<br>- beautify: add gothic style effect|
|2012-07-13|- beautify: add little fresh effect|
|2012-07-12|- beautify: add ice spirit effect<br>- beautify: add japanese effect|
|2012-07-11|- beautify: Add invert effect<br>- beautify: Add astral effect|
|2012-07-10|- beautify: Fix warm effect|
|2012-07-09|- beautify: Fix New Japan effect|
|2012-07-06|- beautify: Add warm effect<br>- beautify: Add Japanese effect|
|2012-07-05|- beautify: Add effects: soft light, sharpen|
|2012-07-04|- beautify: Fix brightness and contrast.<br>- beautify: Add saturation.<br>- beautify: Add hue, cyan_red, magenta_green, yellow_blue adjustment.|
|2012-06-08|- Add more rip-border textures, and display the textures in groups.|
|2012-06-07|- Speed up the preview display.<br>- Add the border opacity adjustment for rip-border plugin.|
|2012-06-06|- Provided the base version of rip-border GIMP plugin.|

<!-- If you find very difficult to make the tables for the changelog, I highly recommend to use this page: https://www.tablesgenerator.com/markdown_tables and only copy the row that you want to add -->
