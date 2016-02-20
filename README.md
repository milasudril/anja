Anja med J -- A sample player for Jack
======================================

<img src="anjalogo.png" alt="Anja logo">

This is a live performance sample player/recorder for Jack. In contrast to
Hydrogen, this will feature a virtual keyboard display as its main UI.
Pre-recorded wave files can be associated to different keyboard keys, and the
virtual keyboard display will show labels for mapped keys, making it easier to
find the right sample during a live session. There is also a recording mode, so
it is possible to record sound clips during a live session.

Dependencies
------------
* libgtk-3-dev
* g++
* libjack-jackd2-dev
* libsndfile-dev

Build instructions (ubuntu)
---------------------------
* $ sudo apt-get install libgtk-3-dev g++ libjack-jackd2-dev libsndfile-dev
* $ make

