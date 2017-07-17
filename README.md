Anja med J -- A sample player for Jack
======================================

This is a live performance sample player/recorder for Jack. In contrast to
Hydrogen, this features a virtual keyboard display as its main UI.
Pre-recorded wave files can be associated to different keyboard keys, and the
virtual keyboard display will show labels for mapped keys, making it easier to
find the right sample during a live session. There is also a recording mode, so
it is possible to record sound clips during a live session.

Dependencies
------------
This is a list of all dependencies

### Libraries
These libraries are required in order to run Anja

 * gtk+3
 * gtksourceview-3.0
 * jack
 * jemalloc
 * sndfile

In addition to these libraries, Anja uses the PCG32 generator from Melissa O'Neill.
Currently, her repository is kept as a git submodule. To process the command line, the ALICE library is used. This is also refered to by a submodule.

### Tools
These tools are required in order to compile Anja

 * blender -- 3D modelling and rendering package
 * g++ -- The GNU project C++ compiler
 * inkscape -- SVG (Scalable Vector Graphics) editing program
 * import -- Command-line utility for creating screenshots
 * octave -- Package for numerical analysis. MATLAB should work, but requires
	changes to `maikeconfig` files.
 * maike -- Automated build tool. You must use version 0.15.9 or later.
	In case this tool is missing, the build script will download the latest release
	from https://github.com/milasudril/maike/. This step requires `wget` and `jq` Command-line JSON processor.
 * pkg-config -- Return metainformation about installed libraries
 * python3 -- An interpreted, interactive, object-oriented programming language
 * xdpyinfo -- Display information utility for X
 * xdotool -- Tool for manipulating X clients
 * xsltproc -- Command-line XSLT processor
 * Xvfb -- Virtual X server

 ### Other dependencies

 Anja uses the font *Linux Libertine* in some of the SVG files. If this font
 is not installed, a serif substitude will be used instead.

Build instructions (ubuntu or debian-like systems)
--------------------------------------------------
These steps should be sufficient to compile Anja on an *Ubuntu desktop:

	sudo apt-get install jackd2 pkg-config libgtk-3-dev g++ blender inkscape \
		libjack-jackd2-dev libsndfile-dev libgtksourceview-3.0-dev \
		libjemalloc-dev xvfb octave xdotool xsltproc
	make

In order to compile `maike`, you will also need `libjansson-dev`. **You may need
to add a more up-to-date version of `blender`**. Ubuntu users can use this PPA:

 * https://launchpad.net/~thomas-schiex/+archive/ubuntu/blender

The KxStudio repositories may contain a more up-to-date version of JACK. More information can be found here: http://kxstudio.linuxaudio.org/Repositories.

After compling Anja, the binary will be located in `__targets_rel` or `__targets_dbg`.
