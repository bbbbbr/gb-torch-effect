; From SameBoy
; Each line is 456 cycles. Without scrolling, sprites or a window:
; Mode 2 - 80  cycles / OAM Transfer
; Mode 3 - 172 cycles / Rendering
; Mode 0 - 204 cycles / HBlank
; 
; Mode 1 is VBlank

	.area	_HEADER_LCD (ABS)

	.org	0x48		; LCD (Install LCD handler at interrupt address)
.custom_int_LCD_vec:
	JP .custom_int_LCD  ; Call main LCD interrupt handler 

	.area	_BASE



; Must make the update in mode 2?
;  if mode 3 hits before its updated then it doesn't take effect until...
;
;

.custom_int_LCD:
	PUSH	AF  ; 2
	PUSH	BC  ; 2
	PUSH	HL  ; 2

; TODO: just load from _y_line_end once and store for compare each loop iteration

; == SCX offset
; * Even multiples of 8 = no correction
; * Every + 1 after starts -1 more left
;   Up to max of -7 left

;	ldh a, (_SCX_REG+0)
;	and a, #0x07
;	ld  hl, #jump_offset$
;wait_scx_offset$:
;	sub a, (hl)
;	jp 	(hl)

;	nop
;	nop
;	nop
;	nop
;
;	nop
;	nop
;	nop
;	nop
;jump_offset$:


								; == Prep for WX Right-side Rounded Window Update
								; Loads address at *p_x_end in BC
	ld	hl, #_p_x_end+0
	ld	c, (hl)
	inc hl
	ld	b, (hl)
	ld	a, (bc)
								
	ldh	(_WX_REG+0), a			; == Load Window location for first line
	inc	c        				; Move pointer to next LUT entry (LUT is 128 byte aligned, don't need a 16 bit add

								; Then This is performed below for each scanline + inc c
								; 	ld	a, (bc)
								;	ldh	(_WX_REG+0),a   
								; 	inc	c


								; Try to line main loop up with start of Mode 2 (Search Objs)

								; Per-Scanline Modes: 2 (Search Objs) -> 3 (Render) -> 0 (H-Blank)
								;
								; == Since end of Mode 3 -> 0 timing can fluctuate with
								;    extra cycles when sprites are fetched, use the (hoepfully)
								;    more consistant mode 0 -> 2 transition to align instead
	ld h, #0x03 
wait_mode_10$:					; Wait for H-Blank (Mode 0)
    ldh	a, (_STAT_REG+0)  ; 3
;	bit	1, a
;	and a, #0x03
	and a, h
	jr	nz, wait_mode_10$		; Exit loop on Mode 3 (Render) -> 0 (H-Blank) transition

wait_mode_23$:
    ldh	a, (_STAT_REG+0)
;	bit	1, a
	and a, h	
	jr	z, wait_mode_23$		; Exit loop on Mode 0 (H-Blank) -> 2 (Search Objs) transition


								; Should now be ~ Mode 2 (Search Objs)
lcd_loop_start$:

;;	ld		a, #0x00            ; White
;	ld		a, #0xFF            ; Black
;	ldh		(_BGP_REG+0),a		; Set Background Pal to new color at start of row
	LD		A, #0xEF
	LDH		(_LCDC_REG+0),a ; Swap BG Tile Map to Alt

	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop						; ~ Mode 3 (Render) begins

	nop
	nop
	nop
	nop
	nop

	ld		a, #0xE7
	ldh		(_LCDC_REG+0),a ; Swap BG Tile Map to Main
;	; Sort of ok, sort of not
;	ld		a, #0xE7
;	ldh		(_LCDC_REG+0),a ; Swap BG Tile Map to Main
;;	ld		a, #0xE4
;;	ldh		(_BGP_REG+0),a		; Set Background Pal to Normal

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

    nop
    nop
    nop
    nop
    nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop

	nop
	nop


	nop



	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop						; ~ Mode 0 (HBlank) begins
	nop
	nop

							; == WX Right-side Rounded Window Update
							;
							; (After exit test so that WX update should be in hblank)
							; Update WX each scanline from the circle LUT
							; BC is pre-loaded with the first LUT address on entering the ISR
							; 44 cycles total
	ld	a, (bc)  			; Load WX for current scanline from LUT
	ldh	(_WX_REG+0), a
	inc	c        			; Move pointer to next LUT entry (LUT is 128 byte aligned, don't need a 16 bit add

	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop


							; == Test for Exit based on LY
	ldh	a, (_LY_REG+0)
	ld	hl, #_y_line_end
	sub	a, (hl)
	jp 	Z, lcd_isr_exit$	; Exit after ending Y line reached



; -----------------


	jp lcd_loop_start$			; Otherwise, stay in ISR for another line



lcd_isr_exit$:

;	ld		a, #0xFF
;	ldh		(_BGP_REG+0),a		; Set Background Pal to Normal
	LD		A, #0xEF
	LDH		(_LCDC_REG+0),a ; Swap BG Tile Map to Alt

	POP		HL
	POP		BC
	POP		AF
	RETI



