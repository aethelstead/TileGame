# TileGame

## To Do
Support Aseprite animation formats for entity animations??? (In addition to Tiled animations)

Potential Supported Resolutions:

Normal
---------
640x480
800x600
1024x768
1152x864
1280x960
1600x1200
1920x1440

Widescreen
----------
720x480
720x576
1176x664
1280x720
1280x768
1280x800
1280x1024
1360x768
1366x768
1440x900
1600x900
1600x1024
1680x1050
1920x1200
2048x1280
2560x1440

Current Supported Resolutions:

1280x720
1920x1080
2560x1440

## VGui

Menu
------
TextBox: { x, y, width, height, centerX, centerY, colour, border, texture }
Button: { x, y, width, height, centerX, centerY, colour, border, texture }
    TextBox

Slider
Check
Radio
List

# Game Loop
Game supports optional vsync. If vsync is enabled (as specified in config.ini) then the target FPS (also specified in config.ini)
will be ignored, and the game rendering will be synchonised with the monitor's refresh rate. If vsync is not enabled then the
game rendering will aspire to run at the specified target FPS, or a default value of 60 if not specified in config.

@TODO: Use something more accurate than SDL_GetTicks
