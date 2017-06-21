#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"led_stop.png","dependencies":
#@				[{"ref":"led_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"},{"ref":"blender","rel":"tool"}]
#@			 }
#@           ,{
#@			 "name":"led_wait.png","dependencies":
#@				[{"ref":"led_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"},{"ref":"blender","rel":"tool"}]
#@			 }
#@           ,{
#@			 "name":"led_ready.png","dependencies":
#@				[{"ref":"led_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"},{"ref":"blender","rel":"tool"}]
#@			 }
#@           ,{
#@			 "name":"led_off.png","dependencies":
#@				[{"ref":"led_sprites.blend","rel":"misc"},{"ref":"../memorender.py","rel":"misc"},{"ref":"blender","rel":"tool"}]
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

	print('# Baking LED sprites')

	memorender.render('led_sprites.blend',in_dir,target_dir,'led_stop.png'\
		,'led_wait.png','led_ready.png','led_off.png')
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
