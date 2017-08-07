#@	{
#@	"targets":
#@		[{
#@		"name":"metatags.xml","dependencies":
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
<keywords>$categories, $debian_section</keywords>

<abstract>
<p>$description_long</p>
</abstract>

<chapter-star id="ack">Acknowledgements</chapter-star>
<p>Anja would not have been possible without these people:</p>

<ul><li>$acks</li></ul>

<p>The author, $thedate</p>

<tableofcontents />

<chapter-star id="legalbrief">Legal information</chapter-star>
<p>$license_short</p>

</content>
''')


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	projinfo=load_json('projectinfo.json')
	subst=dict()
	subst['acks']='</li>\n<li>'.join( sorted( projinfo['acknowledgement'] ) )
	subst['thedate']=projinfo['years'][-1]

	text=projinfo['license_short'].replace('>','&gt;');
	text=text.replace('<','<url>')
	text=text.replace('&gt;','</url>')
	subst['license_short']='</p><p>'.join( text.split('\n\n'))

	subst['categories']=', '.join(projinfo['categories'])
	subst['debian_section']=projinfo['debian_section']
	subst['description_long']=projinfo['description_long']

	with open(target_dir + '/' + in_dir + '/metatags.xml','wb') as output:
		output.write(doc.substitute(subst).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
