#@	{
#@	"dependencies_extra":
#@		[
#@			 {"ref":"makepage.py","rel":"misc"}
#@			,{"ref":"format.css","rel":"include_extra"}
#@			,{"ref":"color.css","rel":"include_extra"}
#@			,{"ref":"../icons/message_warning.svg","rel":"include_extra"}
#@			,{"ref":"../icons/message_info.svg","rel":"include_extra"}
#@			,{"ref":"../icons/message_error.svg","rel":"include_extra"}
#@		]
#@	}

import subprocess
import sys
import shlex
import os
import shutil

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	dest=sys.argv[1]
	src=sys.argv[2]
	in_dir=sys.argv[3]
	dest_dir=os.path.split(dest)[0]

	print('# Generating manual from %s'%src)
	sys.stdout.flush()
	status=subprocess.call(' '.join([shlex.quote(in_dir + '/makepage.py') \
		,shlex.quote('{"stylesheets":["format.css","color.css"]}') \
		,'<',shlex.quote(src) \
		,'>',shlex.quote(dest)]) \
		,shell=True \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.remove(dest)
		raise Exception('Could not generate manual')
	for deps in sys.argv[4:]:
		if os.path.isfile(deps) and deps[:2]!='__':
			shutil.copy(deps,dest_dir)

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
