#@	{
#@	"targets":
#@		[{
#@		"name":"anja.1.man","dependencies":
#@			[
#@			 {"ref":"../projectinfo.json","rel":"misc"}
#@			,{"ref":"options.troff","rel":"misc"}
#@			]
#@		}]
#@	}

import sys
import string
import json
import os

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

doc=string.Template('''.TH $NAME 1 2017-07-26 $source "General Commands"
.SH NAME
$name \- $description
.SH SYNOPSIS
.B $name_lowercase [options]
.SH DESCRIPTION
$description_long
.SH OPTIONS
$options
''')


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

def options(src):
	with open(src) as file:
		return file.read()
try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	subst=load_json('projectinfo.json')
	subst['source']=subst['name'].lower() + '/' + in_dir
	subst['name_lowercase']=subst['name'].lower()
	subst['NAME']=subst['name'].upper()
	subst['options']=options(target_dir+'/'+in_dir+'/options.troff')


	with open(target_dir + '/' + in_dir + '/anja.1.man','wb') as output:
		output.write(doc.substitute(subst).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
