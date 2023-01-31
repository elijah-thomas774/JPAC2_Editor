# JPAC2_Editor
Tool for editing JPAC2_10 and JPAC2_11 files used in Nintendo Games such as Twilight Princess and Skyward Sword

The [Json Library (github)](https://github.com/nlohmann/json) used is by Niels Lohmann.

Original JPA reverse engineering done by Jasper at [noclip.website](https://github.com/magcius/noclip.website).

The original file used for refernce is [JPA.ts](https://github.com/magcius/noclip.website/blob/master/src/Common/JSYSTEM/JPA.ts). I added in support for JPAC2-11, which is the version Skyward Sword uses. (As of right now, its not fully supported on the main website. If you want to see JPAC2-11 particles, use my [fork](https://github.com/elijah-thomas774/noclip.website) and run it locally. Directions are in the noclip.website discord to do so.)

## USE
The exe is meant to be used via command line.
a few options are represented below:
- `-h` shows the help menu
- `-d` signifies that you want to dump the jpc file
- `-i [input file]` is the source particle file
- `-o [output file]` is for the output file
- `-c [configuration json]` is the file to pull edits from

example:
- `.\JPAC2_Editor.exe -i Common.jpc -o newCommon.jpc -c edits.json` will take `Common.jpc` (original file), use replacements defined in `edits.json` and output to `newCommon.jpc`

Note about dumping:
- `.\JPAC2_Editor.exe -i Common.jpc -t TextureDump\ -d` will dump the contents of the particle file - extracting all resources into a master json and export textures.
- These textures are in the form of `.bti`. You can edit them and apply changes if you know how to. If not, you can ignore them.
- If you make texture changes and want to apply them, pass in the `-t [Folder with Texture Changes]` to apply edited textures.
- Example: `.\JPAC2_Editor.exe -i Common.jpc -o newCommon.jpc -c edits.json -t NewTextures\` will do as the first example above, but also apply texture changes from the `NewTextures` if you created it and added texture changes.
