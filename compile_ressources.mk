# run with make -f compile_ressources.mk (while in the project root’s folder)
# must have a "rom_true_base.nds" present in the root folder, which will generate "rom.nds"

# compile some ressources into the rom (separated from main Makefile as it need some python dependancies and allow to manage some files without having to spoil them on this repo)

# the magic nix command (for those who use it) to get the dep is nix-shell -p python3Packages.skytemple-files -p python3Packages.pillow -p python3Packages.svgelements -p python3Packages.scipy
ROM_IN := rom_true_base.nds
ROM_DEST := rom.nds

all: $(ROM_DEST)

PRP_SRC = $(wildcard fs_patch_source/drawing/*.svg)
PRP_DEST = $(patsubst fs_patch_source/drawing/%.svg,fs_patch_temp/CUSTOM/DRAWING/%.prp, $(PRP_SRC))

fs_patch_temp/CUSTOM/DRAWING/%.prp: fs_patch_source/drawing/%.svg tool/drawing/convert2.py
	mkdir -p fs_patch_temp/CUSTOM/DRAWING
	python3 tool/drawing/convert2.py $< $@


SCREEN_SRC = $(wildcard fs_patch_source/screen/*.png)
SCREEN_DEST = $(patsubst fs_patch_source/screen/%.png,fs_patch_temp/CUSTOM/SCREEN/%.raw,$(SCREEN_SRC))

fs_patch_temp/CUSTOM/SCREEN/%.raw: fs_patch_source/screen/%.png tool/display_image/convert.py
	mkdir -p fs_patch_temp/CUSTOM/SCREEN
	python3 tool/display_image/convert.py $< $@

VRAM_SRC = $(wildcard fs_patch_source/vram/*.png)
VRAM_DEST = $(patsubst fs_patch_source/vram/%.png,fs_patch_temp/CUSTOM/VRAM/%.wte,$(VRAM_SRC))

fs_patch_temp/CUSTOM/VRAM/%.wte: fs_patch_source/vram/%.png tool/wte_convert/convert.py
	mkdir -p fs_patch_temp/CUSTOM/VRAM
	python3 tool/wte_convert/convert.py $< $@

FS_PATCH_TEMP_INPUT = $(SCREEN_DEST) $(PRP_DEST) $(VRAM_DEST) $(ROM_IN)

$(ROM_DEST): $(FS_PATCH_TEMP_INPUT)
	@rm -rf temp_rom_folder
	mkdir -p temp_rom_folder
	ndstool -x $(ROM_IN) -9 temp_rom_folder/arm9.bin -7 temp_rom_folder/arm7.bin -y9 temp_rom_folder/y9.bin -y7 temp_rom_folder/y7.bin -d temp_rom_folder/data -y temp_rom_folder/overlay -t temp_rom_folder/banner.bin -h temp_rom_folder/header.bin
	cp -r fs_patch_temp/* temp_rom_folder/data/
	ndstool -c $(ROM_DEST) -9 temp_rom_folder/arm9.bin -7 temp_rom_folder/arm7.bin -y9 temp_rom_folder/y9.bin -y7 temp_rom_folder/y7.bin -d temp_rom_folder/data -y temp_rom_folder/overlay -t temp_rom_folder/banner.bin -h $(ROM_IN) -r9 0x2000000 -e9 0x2000800 -r7 0x2380000 -e7 0x2380000

clean:
	@rm -rf fs_patch_temp temp_rom_folder
	rm -f $(ROM_DEST)