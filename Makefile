export GBDK_HOME="../../gbdk2020/gbdk-2020-git/build/gbdk/"
export BGB="../../tools/bgbw64-1.5.8/bgb64.exe"

LCC = $(GBDK_HOME)bin/lcc -Wa-l -Wl-m -Wl-j -Wl-y -v -Wl-b_CODE=0x0300

# CFLAGS	= -DGBDK_2_COMPAT

OBJDIR      = obj
SRCDIR      = src
BINS	    = torch.gb
CSOURCES   := $(wildcard $(SRCDIR)/*.c) $(wildcard res/*.c)
ASMSOURCES := $(wildcard $(SRCDIR)/*.s)
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

usage:
	 ../../tools/romusage/romusage torch.noi -a -g
