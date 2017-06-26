#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message_warning.png","dependencies":
#@				[{"ref":"message_warning.svg","rel":"misc"}
#@				,{"ref":"inkscape.py","rel":"misc"}]
#@			 }
#@      ]
#@  }

import inkscape
import sys

target_dir=sys.argv[1]
in_dir=sys.argv[2]

inkscape.run(in_dir+'/message_warning.svg',target_dir+'/'+in_dir+'/message_warning.png' \
	,64)
