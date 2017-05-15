#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message_info.png","dependencies":
#@				[{"ref":"message_info.svg","rel":"file"},{"ref":"inkscape","rel":"tool"}]
#@			 }
#@      ]
#@  }

import subprocess
import sys
import os

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	target_dir=sys.argv[1];
	in_dir=sys.argv[2];
	#TODO call inkscape --export-png=info.png -w 64 info.svg
	sys.exit(0)
except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)




