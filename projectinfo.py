#@  {
#@  "targets":
#@      [{
#@		 "name":"projinfo.hpp"
#@		,"dependencies":[{"ref":"externals.json","rel":"generated"}
#@			,{"ref":"maikeconfig.json","rel":"generated"}
#@			,{"ref":"projectinfo.json","rel":"file"}]
#@		}]
#@  }

import sys
import json
import string
import time
import subprocess

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

projinfo_template=string.Template('''/*Information about $projname
*
* This file has been generated by $srcfile on $date.
* Any changes to this file may be overwritten during compilation.
*/

#ifndef ANJA_PROJECTINFO_HPP
#define ANJA_PROJECTINFO_HPP

namespace Anja
	{
	constexpr const char* LIBRARIES[]={"$libraries"};
	constexpr const char* TOOLS[]={"$tools"};
	constexpr const char* COMPILATION_DATE="$date";
	constexpr const char* COMPILER="$compiler";
	constexpr const char* ARCHITECTURE="$architecture";
	constexpr const char* AUTHOR="$author";
	constexpr const char* LEGALBRIEF="$legal_brief";
	constexpr const char* YEARS="$years";
	constexpr const char* ACKNOWLEDGEMENT[]={"$acknowledgement"};
	constexpr const char* REVISION="$revision";
	}

#endif
''')

def load(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)

def compiler_name(config):
	for hook in config['target_hooks']:
		if hook['name']=='targetcxx_default':
			return hook['config']['objcompile']['name']

def compiler_version(exename):
	compiler=subprocess.Popen([exename,'--version'] \
		,stdout=subprocess.PIPE)
	for lines in compiler.stdout:
		return lines.decode('utf8').rstrip()

def get_revision():
	with subprocess.Popen(('git', 'describe','--tags','--dirty','--always')\
		,stdout=subprocess.PIPE) as git:
		result=git.stdout.read().decode().strip()
		git.wait()
		status=git.returncode

	if status:
		with open('versioninfo.txt') as versionfile:
			result=versionfile.read().strip()
	else:
		with open('versioninfo.txt','w') as versionfile:
			versionfile.write(result)

	return result

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]
	substitutes=dict()
	substitutes['srcfile']=sys.argv[0]
	substitutes['date']=time.strftime('%Y-%m-%d %H:%M %Z')

	projinfo=load(in_dir + '/projectinfo.json')
	substitutes['projname']=projinfo['name']
	substitutes['acknowledgement']='","'.join(projinfo['acknowledgement'])
	substitutes['author']=projinfo['author']
	substitutes['legal_brief']=projinfo['legal_brief']
	substitutes['years']=str(projinfo['years']).replace(', ','–').strip('[]')
	substitutes['revision']=get_revision()

	externals=load(target_dir + '/externals.json')
	substitutes['libraries']='","'.join(externals['libraries'])
	substitutes['tools']='","'.join(externals['tools'])

	config=load(target_dir + '/maikeconfig.json')
	substitutes['compiler']=compiler_version(compiler_name(config))
	substitutes['architecture']=config['targetinfo']['architecture']

	with open('/dev/stdout','wb') as output:
		output.write(projinfo_template.substitute(substitutes).encode('utf-8'))
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
