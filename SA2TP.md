# Speed Highway as a 2P stage (SA2_1P_in_2P)

Speed Highway replaces Radical Highway (level 14). With the SA2 2P mod
([SA2_1P_in_2P](https://github.com/TurboTGS/SA2_1P_in_2P)) installed it is its
own stage in the 2P Race stage select, and Radical Highway stays Radical
Highway: the level header swap, the SADX fixes and the replaced files only
apply while a Speed Highway match is played, then everything is put back.

It registers through the 2P stage API (`SA2SpeedHighway/sa2_2p_api.h`, version 2):

- `SA2TP_Init` (in `mod.cpp`) hands over the name, host level 14, the
  start/end/2P-intro positions and the `stage14` folder.
- The 2P mod swaps the files in `stage14\` (they mirror `gd_PC`; they replace
  the stock Radical Highway mission text and sound bank) in and out, and
  registers/restores the positions.
- `SpeedHighway_Apply` / `SpeedHighway_Revert` (in `level.cpp`) swap
  `RadicalHighwayHeader`. All permanent hooks are gated by
  `g_speedHighwayActive` (`isSADXLevel()`), so stock Radical Highway never
  sees them.
- Without the 2P mod, the first `OnFrame` applies everything for good, as
  before.

Everything else (`gd_PC\Models`, `speed-highway*.sa2lvl`, music, ...) only has
new file names and stays in `gd_PC`.

## Building
The project is written against the mod loader pinned in `.gitmodules`
(June 2021, `13ff6a9`), not current headers. Initialise the submodule and
build `SA2SpeedHighway.sln` (Release, Win32; v143 works with
`/p:PlatformToolset=v143 /p:WindowsTargetPlatformVersion=10.0`). The output
needs `libmodutils.lib` and `ModLoaderCommon.lib` in `bin\`.

To install, copy `bin\SA2SpeedHighway.dll` next to the contents of `files\`.
