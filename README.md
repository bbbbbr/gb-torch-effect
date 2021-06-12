
Spotlight / Torch effect Demo for the Game Boy
==============================================


This demo creates an effect of darkened concentric circles around a centered sprite, while still allowing the background map to scroll freely. 

It gives the appearance of a foreground layer with a transparent hole in addition to the background map. Alternating frame images are used to further create the impression of additional color shades (~7) above the usual 4 colors on a Game Boy.

![Game Boy video of torch effect demo](https://raw.githubusercontent.com/bbbbbr/gb-torch-effect/release/info/torch_effect_anim.gif)

The demo works in emulators and on actual hardware.
See releases for a ROM file.


# Implementation

There are two background maps:
  * One using a dungeon tileset and map
  * Another with a map filled with all-black tiles

Above and Below the torch area: The background map is set to use the all-black one

Left edge curve mask: 
  * The background map swaps (~ 1/4 through the scanline) from all-black to the dungeon map
  * Some sprites are used to create the arc shape and cover some scrollx % 8 behavior (see below)

Right edge curve mask:
  * The X start location of the Window layer is moved based on the current scanline using a Look-up-Table (LUT)

A scanline ISR is set to fire when it reaches the top of the torch area
  * It waits for the end of H-Blank and then starts a loop that's timed to exactly match the duration of each scanline. The loop exits when the bottom of the torch area is reached
  * In each iteration of the loop
    * H-Blank: Load the new Window layer X start location
    * Switch the background map to all-black
    * Wait a while then switch from the background map to the dungeon map

Every other frame a larger or smaller circle is displayed
  * A couple of the Left masking sprites are swapped
  * A different Window X start Look-up-Table is selected


# Additional Details

The left edge sprite layout:
![Masking Sprite layout](https://raw.githubusercontent.com/bbbbbr/gb-torch-effect/release/info/torch_effect_sprites_anim.gif)


And with sprites turned off:
![Sprites Off](https://raw.githubusercontent.com/bbbbbr/gb-torch-effect/release/info/torch_effect_sprites_off.png)


Here the sprites are off *AND* the background set to a solid color instead of the dungeon map.

The dark part is the Window layer masking. You see two different arc shapes since there is a different one for each alternating frame. The horizontal line at the top of the arcs is ~when the ISR starts.
![Window Masking](https://raw.githubusercontent.com/bbbbbr/gb-torch-effect/release/info/torch_effect_window_only.png)


Why are there extra sprites as a column mask on the Left, in addition to the ones used for the arc shape?

When the background map is swapped mid-scanline, the pixel it takes effect on will vary based on whether the background map X scroll register is an even multiple of 8. This is due to how the Game Boy LCD hardware implements smooth scrolling.

With the sprite masking turned off the offset is visible. The extra column of sprites are used to hide this variability.
![Window Masking](https://raw.githubusercontent.com/bbbbbr/gb-torch-effect/release/info/torch_effect_scx_mod_8.gif)


# Limitations
* Right now the demo only works when the sprite is centered in the screen. Adding support for moving the torch area vertically would probably not be too hard. Horizontal movement of the torch area may be possible but would require careful timing adjustments for the left edge.


# Credits:
* The dungeon tileset and sprite are from: https://sondanielson.itch.io/gameboy-simple-rpg-tileset
* Very readable description of Game Boy render and LCD timing behavior: http://blog.kevtris.org/blogfiles/Nitty%20Gritty%20Gameboy%20VRAM%20Timing.txt
* Pandocs info on rendering: https://gbdev.io/pandocs/pixel_fifo.html


# Tools
* Built with GBDK-2020 - https://github.com/gbdk-2020/gbdk-2020
* Emulicious emulator - https://emulicious.net/
* BGB emularor - http://bgb.bircd.org/
* GIMP TileMap GB - https://github.com/bbbbbr/gimp-tilemap
* Game Boy Tile Designer / Map Builder - https://github.com/gbdk-2020/GBTD_GBMB
