#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"gmprograms.hpp","dependencies":
#@				[{"ref":"gm_programs.txt","rel":"file"},{"ref":"compile.py","rel":"file"}]
#@			 }
#@      ]
#@  }

import compile
compile.compile('gm_programs.txt')
