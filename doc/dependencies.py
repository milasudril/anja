#@	{
#@	"targets":
#@		[{
#@		"name":"dependencies.xml","dependencies":
#@			[
#@			 {"ref":"../externals.json","rel":"misc"}
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
<chapter id="deps">Dependencies</chapter>
<p>This chapter lists all external dependencies, as extracted by <prgname>maike</prgname>, used within the Anja project. The items listed in <ref>libs</ref> are also needed to run Anja. On some distributions, library packages are split into a runtime package and a development package. In this case, only the runtime package is needed to run Anja, but the development package is needed to compile Anja.</p>

<section id="libs">Libraries</section>
<p>The following list contains all libraries except for the C++ runtime library,  required by Anja.</p>
<ul><li>$libs</li></ul>

<section id="tools">Tools</section>
<p>The following list contains all tools, required to compile Anja.</p>
<ul><li>$tools</li></ul>

<section id="resources">Packages and resources</section>
<p>The following list contains all external resources, required to compile Anja. This list includes static resources like fonts, and packages for scripting languages.</p>
<ul><li>$resources</li></ul>

</content>
''')


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	deps=load_json(target_dir + '/externals.json')
	subst=dict()
	subst['tools']='</li>\n<li>'.join( sorted( deps['tools'] ) )
	subst['libs']='</li>\n<li>'.join( sorted( deps['libraries'] ) )
	subst['resources']='</li>\n<li>'.join( sorted( deps['resources'] ) )

	with open(target_dir + '/' + in_dir + '/dependencies.xml','wb') as output:
		output.write(doc.substitute(subst).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
