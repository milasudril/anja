#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"controlcodes.hpp","dependencies":
#@				[{"ref":"control_codes.txt","rel":"file"},{"ref":"compile.py","rel":"file"}]
#@			 }
#@      ]
#@  }

import compile
compile.compile('control_codes.txt')
