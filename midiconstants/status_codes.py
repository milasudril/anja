#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"statuscodes.hpp","dependencies":
#@				[{"ref":"status_codes.txt","rel":"file"},{"ref":"compile.py","rel":"file"}]
#@			 }
#@      ]
#@  }

import compile
compile.compile('status_codes.txt')
