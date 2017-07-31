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
from pathlib import Path

changelog=string.Template('''$name_lower ($version$package_version) $package_distro_release; urgency=low

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
Build-Depends: debhelper (>=9),$build_deps
Standards-Version: 3.9.7
Homepage: $homepage
Vcs-Git: $vcs

Package: $name_lower
Architecture: any
Depends: $${shlibs:Depends}, $${misc:Depends}, $runtime_deps
Recommends: $package_recommends
Description: $description
 $description_long''')

compat='''9
'''

rules='''#!/usr/bin/make -f

%:
	dh $@'''

source_format='''3.0 (quilt)
'''


def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

def get_revision():
	if shutil.which('git')==None:
		with open('versioninfo-in.txt') as versionfile:
			result=versionfile.read().decode().strip()
	else:
		with subprocess.Popen(('git', 'describe','--tags','--dirty','--always') \
			,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL) as git:
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
	if res=='*':
		projinfo[key]=''
	if not res:
		print('    '+projinfo[key])
		return
	projinfo[key]=res
	print('    '+projinfo[key])

def dpkg_search(filename,vercache):
	if filename==None:
		return ('','')
	if shutil.which('dpkg')==None:
		return ('','')

	with subprocess.Popen(('dpkg-query', '--search',filename) \
		,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL) as dpkg:
		result=dpkg.stdout.read().decode().split('\n')
		dpkg.wait()
		status=dpkg.returncode
		if status:
			return ('','')

	package=result[0].split(':')[0].strip()
	version=vercache.get(package,'')
	if version!='':
		return (package,version)

	with subprocess.Popen(('dpkg-query', '--showformat','${Version}','--show',package) \
		,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL) as dpkg:
		version=dpkg.stdout.read().decode().strip()
		dpkg.wait()
		status=dpkg.returncode
		if status:
			return package
	vercache[package]=version
	return (package,version)



def package_guess(kind,name,vercache):
	if kind=='dev files for':
		res=dpkg_search('*/'+name.replace('-','*')+'*.pc',vercache)
		if res[0]=='':
			return dpkg_search('*/'+name+'.h*',vercache)
		return res
	return dpkg_search(str(Path(shutil.which(name)).resolve()),vercache)

def get_dep(kind,name,vercache):
	guess=package_guess(kind,name,vercache)
	readline.add_history('%s|%s'%(guess[0],guess[1]))
	res=input('    Enter the name of the package containing the %s `%s` (%s|%s): '\
		%(kind,name,guess[0],guess[1])).strip()
	if res=='*':
		return ('','')
	if not res:
		print('      '+'|'.join(guess))
		return guess
	parts=res.split('|')
	parts[0]=parts[0].strip()
	parts[1]=parts[1].strip()
	vercache[parts[0]]=parts[1]
	print('      '+'|'.join(parts))
	return parts

def get_deps(projinfo,caption,deps,key,vercache):
	print('\n\n  %s (currently %s)\n\nI will guess the corresponding packages based on the database of installed packages. Before accepting a guess, *make sure that it REALLY is correct*. If it is not, enter the name of the correct package.\n'%(caption,projinfo[key]))

	deps_out=dict()
	if key=='build_deps':
		for tool in sorted( deps['tools'] ):
			dep=get_dep('tool',tool,vercache)
			if dep[0]!='':
				deps_out[dep[0]]=dep[1]

		for lib in sorted( deps['libraries'] ):
			dep=get_dep('dev files for',lib,vercache)
			if dep[0]!='':
				deps_out[dep[0]]=dep[1]
	else:
		for lib in sorted( deps['runtime_deps'] ):
			dep=get_dep('dev files for',lib,vercache)
			if dep[0]!='':
				deps_out[dep[0]]=dep[1]

	dep=input('    Other dependency (leave blank to complete this step): ')
	while dep!='':
		dep_pair=dep.split('|')
		dep_pair[0]=dep_pair[0].strip()
		dep_pair[1]=dep_pair[1].strip()
		deps_out[dep_pair[0]]=dep_pair[1]
		dep=input('    Other dependency (leave blank to complete this step): ')

	dep_strings=[]
	for package,version in sorted( deps_out.items() ) :
		dep_strings.append('%s (>=%s)'%(package,version))
	projinfo[key]=','.join(dep_strings)

def get_distinfo(projinfo):
	with open('/etc/lsb-release') as lsb_release:
		for line in lsb_release:
			(key,val)=line.partition('=')[::2]
			if key=='DISTRIB_ID':
				projinfo['package_distro']=val
			if key=='DISTRIB_CODENAME':
				projinfo['package_distro_release']=val



try:
	projinfo=load_json('projectinfo.json')
	projinfo['version']=get_revision()
	projinfo['name_lower']=projinfo['name'].lower()
	now=time.time();
	projinfo['package_date']=email.utils.formatdate(now)
	projinfo['package_year']=time.strftime('%Y',time.gmtime(now))
	projinfo['packager_name']='John Doe'
	projinfo['packager_email']='john.doe@example.com'
	projinfo['package_distro']='Ubuntu'
	projinfo['package_version']='-1a1'
	projinfo['package_distro_release']='xenial'
	projinfo['build_deps']=''
	projinfo['runtime_deps']=''
	projinfo['package_recommends']=''
	projinfo['license_short']=' '+'\n .\n '.join(projinfo['license_short'].split('\n\n'))

	deps=load_json('externals-in.json')
	print('''\nBefore creating the debian directory, I need some information about this package. Leave blank to keep the default value. To answer with blank enter *.\n''')

	get(projinfo,'  Your name (%s): ','packager_name')
	get(projinfo,'  Your e-mail (%s): ','packager_email')
	get(projinfo,'  Target distribution (%s): ','package_distro')
	get(projinfo,'  Package version suffix (%s): ','package_version')
	get(projinfo,'  Target distribution release (%s): ','package_distro_release')
	vercache=dict()
	get_deps(projinfo,'Build dependencies',deps,'build_deps',vercache)
	get_deps(projinfo,'Runtime dependencies',deps,'runtime_deps',vercache)
	get(projinfo,'  Recommended packages (%s): ','package_recommends')


	changefield=1
	while changefield!=0:
		print('''\nThis is the packaging information I have got:\n''')
		index=1
		for k in ['packager_name','packager_email','package_distro','package_version'\
			,'package_distro_release','build_deps','runtime_deps','package_recommends']:
			print('%d.  %s: %s'%(index,k,projinfo[k]))
			index=index+1

		change=input('\nDo you want to change any field? Enter 0 or leave blank to continue. ')
		if change=='':
			break
		changefield=int(change)
		if changefield==1: get(projinfo,'  Your name (%s): ','packager_name')
		if changefield==2: get(projinfo,'  Your e-mail (%s): ','packager_email')
		if changefield==3: get(projinfo,'  Target distribution (%s): ','package_distro')
		if changefield==4: get(projinfo,'  Package version suffix (%s): ','package_version')
		if changefield==5: get(projinfo,'  Target distribution release (%s): ','package_distro_release')
		if changefield==6: get_deps(projinfo,'Build dependencies',deps,'build_deps',vercache)
		if changefield==7: get_deps(projinfo,'Runtime dependencies',deps,'runtime_deps',vercache)
		if changefield==8: get(projinfo,'  Recommended packages (%s): ','package_recommends')


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
except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
