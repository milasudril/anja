#@	{
#@	"targets":
#@		[{
#@		"name":"anja.man1","dependencies":
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
import time

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

doc=string.Template('''.TH $NAME 1 $now $source "General Commands"
.SH NAME
$name \- $description
.SH SYNOPSIS
.B $name_lowercase [options]
.SH DESCRIPTION
$description_long
.SH OPTIONS
$options
.SH ENVIRONMENT
.B $name
is affected by the same environment variables as any GTK+\-3. For a complete list of these, see
https://developer.gnome.org/gtk3/stable/gtk-running.html. $name is also affected by \\fBJACK_DEFAULT_SERVER\\fR, which sets the name of the default JACK(1) server.
.SH BUGS
.IP \(bu 4
The \\fB\-\-theme\\fR option only works when the current GTK+\-3 theme supports different variants.
.IP \(bu 4
$name does not fully honour right-to-left layouts. This means that the user interface may look strange under certain locals.
.SH SEE ALSO
.IP \(bu 4
\\fBjackd\\fR(1)
.IP \(bu 4
Anja user's guide availible at https://milasudril.github.io/anja
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
	subst['now']=time.strftime('%Y-%m-%d')

	with open(target_dir + '/' + in_dir + '/anja.man1','wb') as output:
		output.write(doc.substitute(subst).encode('utf-8'))

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
