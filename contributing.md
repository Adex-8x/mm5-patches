# Contributing to `mm5-patches`
Want to add your own custom ASM and/or C code to MysteryMail 5? You've come to the right place!
To contribute to this repository, please open up a pull request with any edits you'd like to make. If you haven't done so before, you may view [this tutorial](https://github.com/firstcontributions/first-contributions/blob/main/README.md) for further instructions.

## Standalone ASM Patches
The dedicated ASM patch section will begin at `0x023DA1B0` in Overlay36, consisting of 0x1000 bytes. It is strongly encouraged that patches are written in C, but ASM is also acceptable. Please contribute any standalone ASM patches in the [`asm/`](asm/) folder of this repository. We won't be too strict on how this is formatted, but if it can be applied as a skypatch or ARMIPS, then it's fine.

## C Patches
The dedicated C patch section will begin at `0x023DB1B0` in Overlay36, consisting of 0x2E50 bytes. Feel free to edit any of the files as you see fit in [`src/`](src/), as well as [`patch.asm`](patches/patch.asm)

## Prohibited Contributions
There are a handful of ground rules, given the type of community event MysteryMail is. Not every possible commit will be accepted. For the most part, you're restricted to writing code that only occurs within ground mode. To be more specific, we will *not* accept:

- Patches that cannot run within ground mode. This includes, but is not limited to:
 - The Top Menu overlays (0-9)
 - Dungeon-related overlays (29-31)
- Patches involving the credits overlay (28)
- Edits that change how the game saves data
- Edits to mission generation
- Edits to a pre-existing SkyTemple ASM patch
- Edits to the `main` section in [linker.ld](linker.ld)
- Overlay36 patches that occupy space outside of the common area
