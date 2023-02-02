GBDK_HOME="../../../gbdk2020/gbdk-2020-git/build/gbdk/"

PNG2ASSET = $(GBDK_HOME)bin/png2asset
LCC = $(GBDK_HOME)bin/lcc -Wa-l -Wl-m -Wl-j -Wl-y -v 


OBJDIR      = obj
SRCDIR      = src
BINS	    = torch.gb
CSOURCES   := $(wildcard $(SRCDIR)/*.c) $(wildcard res/*.c)
ASMSOURCES := $(wildcard $(SRCDIR)/*.s)
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

all:	$(BINS)

make.bat: Makefile
	@echo "REM Automatically generated from Makefile" > make.bat
	@make -sn | sed y/\\//\\\\/ | grep -v make >> make.bat

$(OBJDIR)/%.o:	%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/res/%.o:	%.c
	$(LCC) $(CFLAGS) -c -o $@ $<


$(OBJDIR)/%.o:	%.s
	$(LCC) $(CFLAGS) -c -o $@ $<

%.s:	%.c
	$(LCC) $(CFLAGS) -S -o $@ $<

# Link the compiled object files into a autobanks.gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(CFLAGS) -o $(BINS) $(OBJS)

clean:
	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.* $(OBJDIR)/res/*.*

assets:
# Set Priority flag so sprite is under BG & Window except for BG Color 0
# -sp flag is temproarily broken in GBDK-2020 4.1.1-post-release png2asset
# so need to manually edit sprite.c to OR in the S_PRIORITY flag
	$(PNG2ASSET) res/sprite.png -spr8x16 -sw 48 -sh 48 -sp 80 -keep_palette_order
	$(PNG2ASSET) res/dungeon.png -map -noflip -keep_palette_order


