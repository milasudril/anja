#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message_usererror.png","dependencies":
#@				[{"ref":"message_usererror.svg","rel":"misc"}
#@				,{"ref":"inkscape.py","rel":"misc"}]
#@			 }
#@      ]
#@  }

import inkscape
import sys

target_dir=sys.argv[1]
in_dir=sys.argv[2]

inkscape.run(in_dir+'/message_usererror.svg',target_dir+'/'+in_dir+'/message_usererror.png' \
	,64)
