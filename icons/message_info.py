#@  {
#@  "targets":
#@      [
#@           {
#@			 "name":"message_info.png","dependencies":
#@				[{"ref":"message_info.svg","rel":"misc"}
#@				,{"ref":"inkscape.py","rel":"misc"}
#@				,{"ref":"linux-libertine","rel":"external_resource"}]
#@			 }
#@      ]
#@  }

import inkscape
import sys

target_dir=sys.argv[1]
in_dir=sys.argv[2]

inkscape.run(in_dir+'/message_info.svg',target_dir+'/'+in_dir+'/message_info.png' \
	,64)
