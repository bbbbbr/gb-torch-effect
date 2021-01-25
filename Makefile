export GBDK_HOME="/home/birch/git/gbdev/gbdk2020/gbdk-2020-git/build/gbdk/"
export BGB="../../tools/bgbw64-1.5.8/bgb64.exe"

LCC = $(GBDK_HOME)bin/lcc -Wa-l -Wl-m -Wl-j

# CFLAGS	= -DGBDK_2_COMPAT

OBJDIR      = obj
BINS	    = torch.gb
CSOURCES   := $(wildcard *.c) $(wildcard res/*.c)
ASMSOURCES := $(wildcard *.s)
# OBJS       = $(CSOURCES:%.c=%.o) $(ASMSOURCES:%.s=%.o)
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

