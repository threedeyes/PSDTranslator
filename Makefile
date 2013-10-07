
BINARY := ./build/PSDTranslator

OBJS := libpsd/adjustment.o \
		libpsd/bevel_emboss.o \
		libpsd/bitmap.o \
		libpsd/blend.o \
		libpsd/boundary.o \
		libpsd/brightness_contrast.o \
		libpsd/channel_image.o \
		libpsd/channel_mixer.o \
		libpsd/color_balance.o \
		libpsd/color_mode.o \
		libpsd/color_overlay.o \
		libpsd/color.o \
		libpsd/curves.o \
		libpsd/descriptor.o \
		libpsd/drop_shadow.o \
		libpsd/effects.o \
		libpsd/file_header.o \
		libpsd/fixed.o \
		libpsd/gaussian_blur.o \
		libpsd/gradient_blend.o \
		libpsd/gradient_fill.o \
		libpsd/gradient_map.o \
		libpsd/gradient_overlay.o \
		libpsd/hue_saturation.o \
		libpsd/image_data.o \
		libpsd/image_resource.o \
		libpsd/inner_glow.o \
		libpsd/inner_shadow.o \
		libpsd/invert.o \
		libpsd/layer_mask.o \
		libpsd/levels.o \
		libpsd/outer_glow.o \
		libpsd/path.o \
		libpsd/pattern_fill.o \
		libpsd/pattern_overlay.o \
		libpsd/pattern.o \
		libpsd/photo_filter.o \
		libpsd/posterize.o \
		libpsd/psd_system.o \
		libpsd/psd_zip.o \
		libpsd/psd.o \
		libpsd/rect.o \
		libpsd/satin.o \
		libpsd/selective_color.o \
		libpsd/solid_color.o \
		libpsd/stream.o \
		libpsd/stroke.o \
		libpsd/threshold.o \
		libpsd/thumbnail.o \
		libpsd/type_tool.o \
		shared/TranslatorSettings.o \
		shared/TranslatorWindow.o \
		shared/BaseTranslator.o \
		shared/StreamBuffer.o \
		PSDTranslator.o \
		PSDLoader.o \
		ConfigView.o \
		PSDTranslatorApp.o		

OBJDIR := build

RDEFS := ./res/PSDTranslator.rdef
RSRCS := ./build/PSDTranslator.rsrc

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

CC := g++
LD := $(CC)

LIBS := -lbe -ltranslation -lroot -lz
CFLAGS := -O3 -I./src -I./src/shared -I./src/libpsd
LDFLAGS := 

.PHONY : clean build install

default : build

build : $(BINARY)
	
$(BINARY) : $(OBJDIR) $(OBJS) $(RSRCS)	
	$(LD) $(CFLAGS) $(OBJS) -o $(BINARY) $(LDFLAGS) $(LIBS)
	xres -o $(BINARY) $(RSRCS)
	mimeset -f $(BINARY)

clean:
	rm -rf $(OBJDIR)

build/%.o : src/%.cpp
	@mkdir -p $(OBJDIR)/libpsd
	@mkdir -p $(OBJDIR)/shared
	$(CC) $(CFLAGS) -c $< -o $@

build/%.rsrc : res/%.rdef
	rc -o $@ $<

install:
	mkdir -p /boot/home/config/add-ons/Translators
	cp $(BINARY) /boot/home/config/add-ons/Translators
