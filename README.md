
Spotlight / Torch effect Demo for the Game Boy
==============================================

This demo shows small circles of a background map visible among a larger dark area, while still allowing the background map to scroll freely. Temporal blending of alternating frames creates the semi-transparent effect for the medium sized circle.

It looks like a simple, yet it's not a straightforward effect given the limited graphics abilities of the Game Boy. I learned a lot in the process of making this.

![Game Boy video of torch effect demo](/info/torch_effect_anim.gif)

The demo works in emulators and on actual hardware.
See releases for a ROM file.

# Alternate Method

[Shintendo](https://twitter.com/shintendoYT) made a demo using a different method using Background / Object priority and Palettes.

Related tweet: https://twitter.com/shintendoYT/status/1620576705139314695
 
 * Entire background palette is black (or a dimmed version of the palette)
 * The torch circle and player are a large metasprite (composed of multiple sprites)
 * Priority is set so the sprite is behind the background (transparent parts show as background black)
 
The player and torch circle will show through wherever color 0 is used in the background. This approach has the benefit of being much easier to actually use though the background is effectively 2 colors only. There is a GBDK version of this in the `shintendo-method` folder.
 
Example using GBDK and similar assets to above:
![Shintendo's method of a torch effect](/shintendo-method/torch_gbdk_shintendo_method.gif)


# My Implementation

There are two background maps:
  * One is a dungeon tileset and map
  * Another is a map filled with all-black tiles

Above and Below the torch area:
  * The background map is set to all-black one

Left edge curve mask: 
  * The background map swaps (~ 1/4 through the scanline) from all-black to the dungeon map
  * Some sprites are used to create the arc shape and cover some background (X scroll % 8) behavior (see below)

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
(The left most column of masking sprites has since been removed)

![Masking Sprite layout](/info/torch_effect_sprites_anim.gif)


And with sprites turned off:

![Sprites Off](/info/torch_effect_sprites_off.png)


Why are there extra sprites as a column mask on the Left in addition to the ones used for the arc shape?

When the background map is swapped mid-scanline, the pixel that change takes effect on will vary based on the background map X scroll register modulo 8. This is due to how the Game Boy implements X scrolling with the LCD.

With the sprite masking turned off the offset is visible. The extra column of sprites are used to hide this variability.

![Window Masking](/info/torch_effect_scx_mod_8.gif)


Here, to show the Window layer masking, the sprites are off *AND* the background set to a solid color instead of the dungeon map.

The dark part is the Window layer. Two different arc shapes are visible since there is a different one for each alternating frame. The horizontal line at the top of the arcs is ~when the ISR starts for a given arc size.

![Window Masking](/info/torch_effect_window_only.png)



# Limitations
* Right now the demo only works when the sprite is centered in the screen. Adding support for moving the torch area vertically would probably not be too hard. Horizontal movement of the torch area may be possible but would require careful timing adjustments of the left edge as it moved back and forth.


# Credits:
* The dungeon tileset and sprite are from - https://sondanielson.itch.io/gameboy-simple-rpg-tileset
* A very readable description of Game Boy render and LCD timing behavior - http://blog.kevtris.org/blogfiles/Nitty%20Gritty%20Gameboy%20VRAM%20Timing.txt
* Pandocs info on rendering - https://gbdev.io/pandocs/pixel_fifo.html


# Tools
* Built with GBDK-2020 - https://github.com/gbdk-2020/gbdk-2020
* Emulicious emulator - https://emulicious.net/
* BGB emulator - https://bgb.bircd.org/
* GIMP TileMap GB - https://github.com/bbbbbr/gimp-tilemap
* Game Boy Tile Designer / Map Builder - https://github.com/gbdk-2020/GBTD_GBMB

