#@	{
#@	"targets":
#@		[{
#@		"name":"legalbrief.xml","dependencies":
#@			[
#@			 {"ref":"../projectinfo.json","rel":"misc"}
#@			]
#@		}]
#@	}

import sys
import string
import json

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

doc=string.Template('''<?xml version="1.0"?>
<content>
<chapter id="legalbrief">Legal information</chapter>
<p>$legal_brief</p></content>
''')


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	projinfo=load_json('projectinfo.json')

	with open(target_dir + '/' + in_dir + '/legalbrief.xml','wb') as output:
		output.write(doc.substitute(projinfo).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
