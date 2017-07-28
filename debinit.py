#!/usr/bin/python3

import sys
import os
import json
import string
import time
import shutil
import subprocess
import email.utils
import readline
import stat

changelog=string.Template('''$name_lower ($version-$package_distro_suffix) $package_distro_release; urgency=low

  * Packaged for $package_distro
  
 -- $packager_name <$packager_email>  $package_date''')


copyright=string.Template('''Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Upstream-Name: $name_lower
Source: $homepage

Files: *
Copyright: $years $author <$author_email>

License: $license_name

Files: debian/*
Copyright: $package_year $packager_name <$packager_email>
License: $license_name
$license_short
 .
 On Debian systems, you should find the complete license text in /usr/share/common-licenses/$license_file
''')

control=string.Template('''Source: $name_lower
Section: $debian_section
Priority: optional
Maintainer: $packager_name <$packager_email>
Build-Depends: debhelper (>=9)
Standards-Version: 3.9.6
Homepage: $homepage
Vcs-Git: $vcs

Package: $name_lower
Architecture: any
Depends: $${shlibs:Depends}, $${misc:Depends}
Description: $description
 $description_long''')

compat='''9
'''

rules='''#!/usr/bin/make -f

%:
	dh $@'''

source_format='''3.0 (quilt)
'''


def get_revision():
	if shutil.which('git')==None:
		with open('versioninfo-in.txt') as versionfile:
			result=versionfile.read().decode().strip()
	else:
		with subprocess.Popen(('git', 'describe','--tags','--dirty','--always') \
			,stdout=subprocess.PIPE) as git:
			result=git.stdout.read().decode().strip()
			git.wait()
			status=git.returncode

		if status:
			with open('versioninfo-in.txt') as versionfile:
				result=versionfile.read().strip()
	return result


def load_json(filename):
	with open(filename,encoding='utf-8') as input:
		return json.load(input)
	
def write_file(filename,content):
	with open(filename,'wb') as f:
		f.write(content.encode('utf-8'))
		
def get(projinfo,caption,key):
	res=input(caption%projinfo[key]).strip()
	if not res:
		return
	projinfo[key]=res
	print(projinfo[key])	


projinfo=load_json('projectinfo.json')
projinfo['version']=get_revision()
projinfo['name_lower']=projinfo['name'].lower()
now=time.time();
projinfo['package_date']=email.utils.formatdate(now)
projinfo['package_year']=time.strftime('%Y',time.gmtime(now))
projinfo['packager_name']='John Doe'
projinfo['packager_email']='john.doe@example.com'
projinfo['package_distro']='Ubuntu'
projinfo['package_distro_suffix']='0ubuntu1'
projinfo['package_distro_release']='xenial'
projinfo['license_short']=' '+'\n .\n '.join(projinfo['license_short'].split('\n\n'))

get(projinfo,'Your name (%s): ','packager_name')
get(projinfo,'Your e-mail (%s): ','packager_email')
get(projinfo,'Target distribution (%s): ','package_distro')
get(projinfo,'Target distribution suffix (%s): ','package_distro_suffix')
get(projinfo,'Target distribution release (%s): ','package_distro_release')

try:
	shutil.rmtree('debian')
except:
	pass

os.mkdir('debian')

write_file('debian/compat',compat)
write_file('debian/copyright',copyright.substitute(projinfo))
write_file('debian/changelog',changelog.substitute(projinfo))
write_file('debian/control',control.substitute(projinfo))
write_file('debian/rules',rules)
st = os.stat('debian/rules') #debian/ruls should be executable
os.chmod('debian/rules', st.st_mode | stat.S_IEXEC)
os.mkdir('debian/source')
write_file('debian/source/format',source_format)
