#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"index.html","dependencies":
#@				[
#@					 {"ref":"index.xml","rel":"misc"}
#@					,{"ref":"makepage.py","rel":"misc"}
#@				]
#@			 }
#@      ]
#@  }

import subprocess
import sys
import shlex
import locale
import shutil
import glob
import os

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	target_dir=sys.argv[1];
	in_dir=sys.argv[2];

	print('# Generating manual')
	status=subprocess.call(' '.join([shlex.quote(in_dir + '/makepage.py') \
		,'<',shlex.quote(in_dir+'/index.xml') \
		,'>',shlex.quote(target_dir+'/'+in_dir+'/index.html')]) \
		,shell=True \
		,env={"LANG":"C.UTF-8"})
	if status!=0:
		sys.exit(-1)

#TODO: This should be fixed when Maike#46 is fixed
# (the raster versions of these files do not exist yet if there is no dependency)
	for resource in glob.glob('icons/*.svg'):
		shutil.copyfile(resource,target_dir+'/'+in_dir+'/'+os.path.split(resource)[1])

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
