# mm5-patches
A collection of custom ARM Assembly and C patches specifically written for the fifth (currently untitled) MysteryMail event. The featured code is still a work in progress and may feature some degree of spoilers as the event takes place!

Want to get acquainted with the general features of MM5, including code from this repo? You can check out the [features page](https://github.com/Adex-8x/mm5-patches/wiki/MM5-Features) for info on handy patches participants may use!

If you wish to add your own custom ASM/C code to MM5, please contribute to this repository by first following the installation steps as outlined below, then reading the [contrbution page](https://github.com/Adex-8x/mm5-patches/wiki/Contributing-to-mm5%E2%80%90patches) for more info.

All patches are designed with the NA/US version in mind and likely do/will not have EU or JP support.

# Slightly edited c-of-time README

![c-of-time logo by Irdkwia](./cot-logo.png)
*Logo by [Irdkwia](https://github.com/irdkwia)*

An environment for hooking and linking to Pokémon Mystery Dungeon: Explorers of Sky.

## Credits
This project is loosely based on [EternalCode's template](https://github.com/EternalCode/Empty-Template). The build configuration is based on scripts provided by [devkitPro](https://devkitpro.org). The patch format was inspired by [Starlight](https://github.com/shadowninja108/Starlight).

Special thanks to [UsernameFodder](https://github.com/UsernameFodder) for the [pmdsky-debug](https://github.com/UsernameFodder/pmdsky-debug) project, [End45](https://github.com/End45) for the *ExtraSpace* patch and irdkwia for their research on item, move and special process effects.

## Rust subsystem
**NOTE: The `main` branch does currently not contain the Rust subsystem anymore**, as it's support
for symbols for `pmdsky-debug` is outdated and we eventually want to split the Rust subsytem
off so we can keep `c-of-time` up-to-date with `pmdsky-debug` more easily. Use the `rust` branch
if you want to use the Rust subsystem.

c-of-time can also be used with Rust projects. If you want to use Rust (including mixed Rust + C projects),
continue reading the `README.md` in the `rust` directory.

If you want to build pure C projects, continue below.

## Project setup

### Preparing the ROM

You need a US or EU ROM of Pokémon Mystery Dungeon: Explorers of Sky. The ROM must be patched with the [`ExtraSpace` patch by End45](https://github.com/End45/EoS-asm-hacks/blob/main/src/ExtraSpace.asm). You can apply the patch with [SkyTemple](https://skytemple.org):
  1. Open the ROM in SkyTemple
  2. Click *ASM Patches* (*Patches > ASM* in SkyTemple 1.4+) and switch to the *Utility* tab
  3. Select the *ExtraSpace* patch and click *Apply*

### Installing dependencies

Refer to the setup guide for your platform:
- [Windows](./install_windows.md)
- [Linux](./install_linux.md)
- [macOS](./install_macos.md)

## Building
To build the project, run `make patch`. This command will build your code, inject it into an overlay in the provided ROM and apply the patches in the `patches` directory. The output ROM will be saved as `out.nds` by default.

If you want to check the generated assembly, run `make asmdump`. A file `out.asm` will be generated, which contains an assembly listing annotated with the corresponding source code lines.

## Usage
Patches can be added to `.asm` files inside the `patches` directory. These patch files contain offsets into functions that should be patched and assembly instructions, which allow calling into custom code. See `src/main.c` and `patches/patches.asm` for examples.

### Logging and assertions
You can use the logging macros `COT_LOG`, `COT_WARN` and `COT_ERROR`. To view the logs, open the ROM in the SkyTemple debugger and check "Game Internal" in the log window. A macro for assertions `COT_ASSERT(expr)` is also available.

To disable assertions and logging globally and save some performance, change `RELEASE_CONFIG` in `Makefile`.

### Custom move/item effects and special processes
To create custom special processes, add them into the `switch` statement in `CustomScriptSpecialProcessCall`. This function is only called for special process ID 100 and greater for compatibility with existing patches.

You can add custom item or move effects in `CustomApplyItemEffect` and `CustomApplyMoveEffect`.

#### Compatiblity with existing patches
This project aims to keep compatibility with existing patches for move, item and special process effects to some degree. Special process effects using the `ExtractSpCode` patch can be reused without problems if they were imported with an ID lower than 100. Compatiblity with the `ExtractMoveCode` has not been thoroughly tested yet and might potentially cause issues with the *Metronome* move.

## Updating symbol definitions and headers
To update symbol data from `pmdsky-debug`, run `git submodule foreach git pull origin master`,
then clean the build with `make clean`.

## Adding custom symbols
If you've found symbols that are currently missing, consider contributing them to [pmdsky-debug](https://github.com/UsernameFodder/pmdsky-debug). You can find instructions in the repository's [contribution docs](https://github.com/UsernameFodder/pmdsky-debug/blob/master/docs/contributing.md).

For quick testing, you can also add symbols to `symbols/custom_[region].ld` (`symbols/generated_[region].ld` is auto-generated and should not be modified). You need to specify the file each symbol belongs to in comments:

```
/* !file arm9 */
MyCoolFunction = 0x200DABC;

/* !file overlay29 */
SomeDungeonFunction = 0x22DEABC;
SomeOtherDungeonFunction = 0x22DEEFF;
```

## Code size constraints

The built code gets injected into the custom overlay 36. The entire overlay is 228 KB big, most of which is reserved for common patches provided by SkyTemple. Your code will be placed in the last 32 KB, which are considered the "common area" . If the binary is larger than 32 KB, you will get the following linker error: 
```
error "section '.text' will not fit in region 'out'"
```

### Expanding the available space
To work around this issue, you can extend the space allocated in the overlay. **If you decide to extend the space, you do so at your own risk. Be careful since this space might be used by future patches!** Check the [list of assigned areas](https://docs.google.com/document/d/1Rs4icdYtiM6KYnWxMkdlw7jpWrH7qw5v6LOfDWIiYho) to find out if patches used in your ROM are affected.

The value of `ORIGIN` must a multiple of 16 (end with 0 in hexadecimal). Therefore, the amount of bytes added to `LENGTH` must also be a multiple of 16. However, if you are a participant wishing to contribute to this repository, you don't have to worry and should _not_ edit `ORIGIN` nor `LENGTH`!

### Optimizing for size
You can also change the compiler flags to optimize for size instead of speed. To do so, set `OPT_LEVEL := Os` in `Makefile`. Effectiveness varies per project.

## Licensing
- Build scripts (everything under the `tools`) are licensed under GPLv3. Review the file `LICENSE_GPLv3` for more information.
- The parrot photo at `fs_patch_source/screen/0001.png` is in the public domain (taken by Adrian Pingstone)
- `src/libnds_cache.s`, `src/libnds_dma.h`, `src/libnds_videoGL.h` are under some permissive license from libnds.
- `src/smalllib/fpsqrt.h` and `src/smalllib/fpsqrt.c` are under the MIT licence by Christophe Meessen
- All other code is licensed under MIT. Review the file `LICENSE_MIT` for more information.
