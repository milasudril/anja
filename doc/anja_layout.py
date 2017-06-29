#@	{
#@	"targets":
#@		[{
#@		"name":"anja_layout.svg","dependencies":
#@			[{"ref":"anja_layout.txt","rel":"misc"}]
#@		}]
#@	}

import sys
import os
import string
import numpy

def write_error(*args, **kwargs):
    print(*args,file=sys.stderr,**kwargs)

anja_layout=string.Template('''<?xml version="1.0"?>
<svg width="$width" height="$height" xmlns="http://www.w3.org/2000/svg" version="1.1">
<style>
text
	{
	 font-size:16px;fill:#800040;font-family:"Latin Modern Roman",Serif;
	}
</style>
  <g transform="translate($ctrl_mx,$ctrl_my)">
    <rect x="0" y="0" width="$ctrl_sx" height="$ctrl_sy" style="fill:#ffffff;stroke-width:2px;stroke:rgb(0,0,0)" />
	<text x="$ctrl_mMx" y="$ctrl_mMy" text-anchor="middle" dominant-baseline="middle">$ctrl_label</text>
  </g>
  <g transform="translate($keyb_mx,$keyb_my)">
    <rect x="0" y="0" width="$keyb_sx" height="$keyb_sy" style="fill:#ffffff;stroke-width:2px;stroke:rgb(0,0,0)" />
	<text x="$keyb_mMx" y="$keyb_mMy" text-anchor="middle" dominant-baseline="middle">$keyb_label</text>
  </g>
  <g transform="translate($settings_mx,$settings_my)">
    <rect x="0" y="0" width="$settings_sx" height="$settings_sy" style="fill:#ffffff;stroke-width:2px;stroke:rgb(0,0,0)" />
	<text x="$settings_mMx" y="$settings_mMy" text-anchor="middle" dominant-baseline="middle">$settings_label</text>
  </g>
</svg>
''')

def rect(index,keys,labels,mx,my,Mx,My,params):
	params[keys[index] + '_mx']=mx[index]
	params[keys[index] + '_my']=my[index]
	params[keys[index] + '_sx']=Mx[index] - mx[index]
	params[keys[index] + '_sy']=My[index] - my[index]
	params[keys[index] + '_mMx']=0.5*( mx[index] + Mx[index] ) - mx[index]
	params[keys[index] + '_mMy']=0.5*( my[index] +  My[index] ) - my[index]
	params[keys[index] + '_label']=labels[index]

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
	keys=['ctrl','keyb','settings']
	labels=['Action panel','Keyboard view','Settings panel']
	for k,v in enumerate(keys):
		rect(k,keys,labels,mx,my,Mx,My,params)

	with open(target_dir + '/' + in_dir + '/anja_layout.svg','wb') as output:
		output.write(anja_layout.substitute(params).encode('utf-8'))
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
