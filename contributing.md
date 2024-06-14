# Contributing to `mm5-patches`
Want to add your own custom ASM and/or C code to MysteryMail 5? You've come to the right place!
To contribute to this repository, please open up a pull request with any edits you'd like to make. If you haven't done so before, you may view [this tutorial](https://github.com/firstcontributions/first-contributions/blob/main/README.md) for further instructions.

It's also encouraged to familiarize yourself with this repo's [`.gitignore`](.gitignore), as we will obviously not accept someone attempting to upload an xdelta patch here.

## Standalone ASM Patches
The dedicated ASM patch section will begin at `0x023DA1B0` in Overlay36, consisting of 0x1000 bytes. It is strongly encouraged that patches are written in C, but ASM is also acceptable. Please contribute any standalone ASM patches in the [`asm/`](asm/) folder of this repository. We won't be too strict on how this is formatted, but if it can be applied as a skypatch or ARMIPS, then it's fine.

## C Patches
The dedicated C patch section will begin at `0x023DB1B0` in Overlay36, consisting of 0x2E50 bytes. Feel free to edit any of the files as you see fit in [`src/`](src/), as well as [`patch.asm`](patches/patch.asm)

## Prohibited Contributions
There are a handful of house rules in place to maintain the cutscene-centric (well, for the most part) nature of MysteryMail. Not every possible contribution will be accepted. For the most part, you're restricted to writing code that only occurs within ground mode. To be more specific, we will *not* accept:

- Patches that cannot run naturally within ground mode. This includes, but is not limited to:
 - The Top Menu overlays (0-9)
 - Dungeon-related overlays (29-31)
- Edits to the `main` section in [`linker.ld`](linker.ld)
 - You may, however, add new sections that are not within Overlay36 or an aforementioned banned overlay
- Overlay36 patches that occupy space outside of the aforementioned listed regions
- Code that causes an intentional crash or softlock
- Custom script opcodes/instructions
- Edits to `CART_REMOVED_IMG_DATA` or any functions related to game cart ejection
- Edits that change how the game saves data
- Edits to mission generation
- Edits to a pre-existing SkyTemple ASM patch

This list is non-exhaustive; contributions may still be rejected for a reason not currently listed.

## What If My Code Isn't Compatible With a Certain System?
It's *heavily* preferred that all code does not cause (unintentionally) a crash on any piece of hardware or emulator, but things like hardware compatibility won't be a strict requirement. If any problematic code does slip through, this repo's maintainer will ~~cry~~ try to clean up stuff.

## When Can I Contribute?
Whenever you'd like! It doesn't have to be your designated turn to contribute, nor do you even have to participate in the event to contribute. Just be aware that this repository will remain public throughout the event, so any piece of code you write here won't ever be hidden. Spoilers beware, so please try not to write the entirety of a cutscene in C (no matter how impressed we would be).