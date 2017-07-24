# midi-constants -- An unofficial MIDI SDK
This repository contains all (General)MIDI constants in semicolon delimited text
files.

## Contnets
 * `status_codes.txt` contains all MIDI status codes
 * `control_codes.txt` contains all control codes for MIDI status 0xB0
 * `gm_programs.txt` contains names of all General MIDI programs
 * `gm_drumkit.txt` contains names of all General MIDI percussion instruments

## Usage
You may compile any include file by running the corresponding python script:

    python3 -- gm_drumkit.py output_directory .
    
Also, direct use from a Maike project is supported. The `.` is needed since the
script takes the current directory from the command line.

### Control codes
Control codes less than 32 have a corresponding Least Significant Byte control
code obtained by adding 32 to the Most Significant Byte control code. Therefore,
these are not listed in `control_codes.txt`.
