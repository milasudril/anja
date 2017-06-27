#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"knob_ambient_in.png","dependencies":
#@				[{"ref":"knob_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"}]
#@			 }
#@           ,{
#@			 "name":"knob_diffuse_in.png","dependencies":
#@				[{"ref":"knob_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"}]
#@			 }
#@           ,{
#@			 "name":"knob_mask_in.png","dependencies":
#@				[{"ref":"knob_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"}]
#@			 }
#@      ]
#@  }

import subprocess
import sys
import os
sys.path.append(os.getcwd())
import memorender

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	target_dir=sys.argv[1];
	in_dir=sys.argv[2];

	print('# Baking knob sprites')
	memorender.render(1,'knob_sprites.blend',in_dir,target_dir \
		,'knob_ambient_in.png','knob_diffuse_in.png','knob_mask_in.png')

	sys.exit(0)
except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
