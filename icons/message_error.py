#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message_error.png","dependencies":
#@				[{"ref":"message_error.svg","rel":"misc"}
#@				,{"ref":"inkscape.py","rel":"misc"}]
#@			 }
#@      ]
#@  }

import inkscape
import sys

target_dir=sys.argv[1]
in_dir=sys.argv[2]

inkscape.run(in_dir+'/message_error.svg',target_dir+'/'+in_dir+'/message_error.png' \
	,64)
