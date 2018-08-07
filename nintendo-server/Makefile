#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/ds_rules

export TARGET		:=	$(shell basename $(CURDIR))
export TOPDIR		:=	$(CURDIR)


.PHONY: checkarm7 checkarm9

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: checkarm7 checkarm9 $(TARGET).nds

#---------------------------------------------------------------------------------
$(TARGET).nds	:	source/arm7/$(TARGET).elf source/arm9/$(TARGET).elf
	ndstool	-c $(TARGET).nds -7 source/arm7/$(TARGET).elf -9 source/arm9/$(TARGET).elf

#---------------------------------------------------------------------------------
checkarm7 source/arm7/$(TARGET).elf:
	$(MAKE) -C source/arm7
	
#---------------------------------------------------------------------------------
checkarm9 source/arm9/$(TARGET).elf:
	$(MAKE) -C source/arm9

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C source/arm9 clean
	$(MAKE) -C source/arm7 clean
	rm -f $(TARGET).nds $(TARGET).arm7 $(TARGET).arm9
