#@	{
#@	"targets":
#@		[{
#@		"name":"anja_layout.svg","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"mixerfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		},{
#@		"name":"statusarea.png","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"mixerfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		},{
#@		"name":"waveformsettings.png","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"mixerfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		},{
#@		"name":"waveformtrim.png","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"mixerfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		},{
#@		"name":"channels.png","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"mixerfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		},{
#@		"name":"session.png","dependencies":
#@			[
#@			 {"ref":"anja_layout.txt","rel":"misc"}
#@			,{"ref":"mainwindowstart.png","rel":"misc"}
#@			,{"ref":"waveformloaded.png","rel":"misc"}
#@			,{"ref":"sessfull.png","rel":"misc"}
#@			,{"ref":"numpy","rel":"external_resource"}
#@			]
#@		}]
#@	}

import sys
import os
import string
import numpy
import subprocess

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

anja_layout=string.Template('''<?xml version="1.0"?>
<svg width="$width" height="$height" xmlns="http://www.w3.org/2000/svg" version="1.1">
<style>
text
	{
	 font-size:16px;fill:#800040;font-family:"Latin Modern Roman",Serif;
	}
rect
	{
	 fill:#ffffff;stroke-width:2px,stroke:rgb(0,0,0)
	}
</style>
$rectangles
</svg>
''')

rectangle=string.Template('''<g transform="translate($mx,$my)">
    <rect x="0" y="0" width="$sx" height="$sy" style="fill:#ffffff;stroke-width:2px;stroke:rgb(0,0,0)" />
	<text x="$mMx" y="$mMy" text-anchor="middle" dominant-baseline="middle">$label</text>
  </g>''')

def rect(index,labels,mx,my,Mx,My):
	params=dict()
	params['mx']=mx[index]
	params['my']=my[index]
	params['sx']=Mx[index] - mx[index]
	params['sy']=My[index] - my[index]
	params['mMx']=0.5*( mx[index] + Mx[index] ) - mx[index]
	params['mMy']=0.5*( my[index] +  My[index] ) - my[index]
	params['label']=labels[index]
	return rectangle.substitute(params)

try:
	target_dir=sys.argv[1]
	in_dir=sys.argv[2]
	values=numpy.loadtxt(target_dir + '/' + in_dir + '/anja_layout.txt' \
		, delimiter=' ')
	indices=values[:,0]
	mx=values[:,1]
	my=values[:,2]
	Mx=values[:,3]
	My=values[:,4]

	params=dict()
	params['width']=max(Mx) + min(mx)
	params['height']=max(My) + min(my)
	labels=['Action panel','Keyboard view','Settings panel','Status area']
	rects=[]
	for k,v in enumerate(labels):
		rects.append( rect(k,labels,mx,my,Mx,My) )
	params['rectangles']='\n'.join(rects)

	with open(target_dir + '/' + in_dir + '/anja_layout.svg','wb') as output:
		output.write(anja_layout.substitute(params).encode('utf-8'))

	statusarea=(Mx[3] - mx[3],My[3] - my[3],mx[3],my[3])

	status=subprocess.call(['convert'\
		,target_dir + '/' + in_dir +'/mainwindowstart.png'\
		,'-crop','%dx%d+%d+%d'%statusarea\
		,'+repage'\
		,target_dir + '/' + in_dir +'/statusarea.png']\
		,shell=False \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.exit(-1)

	settingspanel=(Mx[2] - mx[2],My[2] - my[2],mx[2],my[2])
	status=subprocess.call(['convert'\
		,target_dir + '/' + in_dir +'/waveformloaded.png'\
		,'-crop','%dx%d+%d+%d'%settingspanel\
		,'+repage'\
		,target_dir + '/' + in_dir +'/waveformsettings.png']\
		,shell=False \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.exit(1)

	status=subprocess.call(['convert'\
		,target_dir + '/' + in_dir +'/mixerfull.png'\
		,'-crop','%dx%d+%d+%d'%settingspanel\
		,'+repage'\
		,target_dir + '/' + in_dir +'/channels.png']\
		,shell=False \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.exit(1)

	status=subprocess.call(['convert'\
		,target_dir + '/' + in_dir +'/sessfull.png'\
		,'-crop','%dx%d+%d+%d'%settingspanel\
		,'+repage'\
		,target_dir + '/' + in_dir +'/session.png']\
		,shell=False \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.exit(1)

	trim=(Mx[4] - mx[4],My[4] - my[4],mx[4],my[4])
	status=subprocess.call(['convert'\
		,target_dir + '/' + in_dir +'/waveformloaded.png'\
		,'-crop','%dx%d+%d+%d'%trim\
		,'+repage'\
		,target_dir + '/' + in_dir +'/waveformtrim.png']\
		,shell=False \
		,env={"LANG":"C.UTF-8"})

	if status!=0:
		sys.exit(1)

	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
