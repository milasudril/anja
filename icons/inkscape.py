#@	{"dependencies_extra":[{"ref":"inkscape","rel":"tool"}]}

import subprocess
import sys
import os

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

def run(source,dest,height):
	try:
		subprocess.run(['inkscape' \
			,'--export-png='+dest \
			,('-h %d')%height
			,source] \
			,check=True \
			,stdout=subprocess.DEVNULL)
		sys.exit(0)
	except Exception:
		write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
		sys.exit(-1)
