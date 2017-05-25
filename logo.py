#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"logo.png","dependencies":
#@				[{"ref":"logo.blend","rel":"file"},{"ref":"blender","rel":"tool"}]
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

	print('# Baking logo')

	blender=subprocess.Popen(['blender','-b',in_dir+'/logo.blend' \
		,'-s','1','-e','1','-o',target_dir+'/'+in_dir+'/logo_#.png','-a'] \
		,stdout=subprocess.PIPE)
	for lines in blender.stdout:
		progress=lines.decode('utf8').rstrip().split('|')
		print('# Blender: %s'%(progress[-1].strip()))

	os.rename(target_dir+'/'+in_dir+'/logo_1.png',target_dir+'/'+in_dir+'/logo.png')

	sys.exit(0)
except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
