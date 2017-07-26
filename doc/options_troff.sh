#@	{
#@	"targets":
#@		[{
#@		"name":"options.troff","dependencies":
#@			[{"ref":"ronn","rel":"tool"}
#@			,{"ref":"sed","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		}]
#@	}

abort()
	{
	exit -1
	}
trap 'abort' 0
set -eo pipefail

dir_target="$1"
in_dir="$2"

"$dir_target"/anja --help \
	| sed 's/\(^--[a-z][a-z\-]*\)\(\[\?=\)\([a-z ]*\)/`\1`\2*\3*/g' \
	| sed 's/\(JACK_DEFAULT_SERVER\)/`\1`/' \
	| ronn \
	| sed 's/^ \.SH/.SS/'> "$dir_target"/"$in_dir"/options.troff

trap : 0
