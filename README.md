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
 * samplerate
 * sndfile


In addition to these libraries, Anja uses the PCG32 generator from Melissa
O'Neill. Currently, her repository is kept as a git submodule. To process the
command line, the ALICE library is used. This is also refered to by a submodule.

### Tools
These tools are required in order to compile Anja. The list only includes tools
that are not POSIX standard command line tools. For a more complete list, see
https://milasudril.github.io/anja#deps.

 * Xvfb -- virtual X server
 * bash -- the Bourne-Again SHell
 * blender -- 3D modelling and rendering package
 * dot -- tool from the Graphviz package that draws directed graphs
 * g++ -- the GNU project C++ compiler
 * import -- command-line utility from ImageMagick that is for creating screenshots
 * inkscape -- SVG (Scalable Vector Graphics) editing program
 * maike -- Automated build tool. You must use version 0.15.9 or later.
	In case this tool is missing, the build script will download the latest
    release	from https://github.com/milasudril/maike/. This step requires `wget`
    and the `jq` command-line JSON processor. maike itself requires libjansson
    to be installed
 * markdown -- converts markdown to (x)html
 * octave -- Package for numerical analysis. MATLAB should work, but it requires
	some changes to `maikeconfig` files.
 * pkg-config -- return metainformation about installed libraries
 * python3 -- an interpreted, interactive, object-oriented programming language
 * xdotool -- tool for manipulating X clients
 * xdpyinfo -- display information utility for X
 * xsltproc -- command-line XSLT processor


### Other dependencies

Anja uses the font *Linux Libertine* in some of the SVG files. If this font
is not installed, a serif substitude will be used instead.

Build instructions (Ubuntu or debian-like systems)
--------------------------------------------------

If `maike` is not installed, prepare for bootstrapping it

    sudo apt-get install libjansson-dev jq wget

In any case, **make sure you have access to the latest `blender`**. Ubuntu users can
use the following PPA:

 * https://launchpad.net/~thomas-schiex/+archive/ubuntu/blender

It can be added through the commands

    sudo add-apt-repository ppa:thomas-schiex/blender
    sudo apt-get update

In addition to that, the KxStudio repositories may contain a more up-to-date
version of JACK, as well as some other useful utilities. More information can be
found here: http://kxstudio.linuxaudio.org/Repositories.

After adding this PPA, install all tools and libraries

    sudo apt-get install jackd2 pkg-config libgtk-3-dev g++ blender inkscape \
        libjack-jackd2-dev libsndfile-dev libgtksourceview-3.0-dev python3 \
        libjemalloc-dev xvfb octave xdotool xsltproc libsamplerate0-dev \
        graphviz imagemagick
    make

After compling Anja, the binary will be located in `__targets_rel` or `__targets_dbg`.
