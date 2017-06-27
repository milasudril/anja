#@	{"dependencies_extra":[{"ref":"makepage.py","rel":"misc"}]}

import subprocess
import sys
import shlex

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

try:
	print(sys.argv)
	dest=sys.argv[1]
	src=sys.argv[2]
	in_dir=sys.argv[3]

	print('# Generating manual from %s'%src)
	sys.stdout.flush()
	status=subprocess.call(' '.join([shlex.quote(in_dir + '/makepage.py') \
		,'<',shlex.quote(src) \
		,'>',shlex.quote(dest)]) \
		,shell=True \
		,env={"LANG":"C.UTF-8"})
	if status!=0:
		raise Exception('Could not generate manual')
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
