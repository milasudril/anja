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
	params['rectangles']=rects

	with open(target_dir + '/' + in_dir + '/anja_layout.svg','wb') as output:
		output.write(anja_layout.substitute(params).encode('utf-8'))
	sys.exit(0)

except Exception:
	write_error('%s:%d: error: %s\n'%(sys.argv[0],sys.exc_info()[2].tb_lineno,sys.exc_info()[1]))
	sys.exit(-1)
