#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"logo.png","dependencies":
#@				[
#@					 {"ref":"logo.blend","rel":"misc"}
#@					,{"ref":"memorender.py","rel":"misc"}
#@				]
#@			 }
#@      ]
#@  }

import sys
import memorender

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	target_dir=sys.argv[1];
	in_dir=sys.argv[2];

	print('# Baking logo')
	memorender.render('logo.blend',in_dir,target_dir,'logo.png')
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
