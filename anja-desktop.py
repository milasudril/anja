#@	{
#@	"targets":
#@		[{
#@		"name":"anja.desktop","dependencies":
#@			[
#@			 {"ref":"projectinfo.json","rel":"misc"}
#@			]
#@		}]
#@	}

import sys
import string
import json


def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

doc=string.Template('''[Desktop Entry]
# The type as listed above
Type=Application

# The version of the desktop entry specification to which this file complies
Version=1.0

# The name of the application
Name=$name

# A comment which can/will be used as a tooltip
Comment=$description

# The executable of the application, possibly with arguments.
Exec=$main_executable

# The name of the icon that will be used to display this entry
Icon=$icon

# Describes whether this application needs to be run in a terminal or not
Terminal=false

# Describes the categories in which this entry should be shown
Categories=$categories_joined
''')


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]

	subst=load_json('projectinfo.json')
	subst['categories_joined']=';'.join(subst['categories'])
	with open(target_dir + '/' + in_dir + '/anja.desktop','wb') as output:
		output.write(doc.substitute(subst).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
