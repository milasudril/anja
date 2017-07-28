#@	{
#@	"targets":
#@		[{
#@		"name":"anja-src.tar.gz","dependencies":
#		Track `versioninfo.txt` so we run `versioninfo.py` before this script
#@			[{"ref":"versioninfo.txt","rel":"misc"}
#@			,{"ref":"find","rel":"tool"}
#@			,{"ref":"grep","rel":"tool"}
#@			,{"ref":"tar","rel":"tool"}]
#@		,"status_check":"dynamic"
#@		}]
#@	}

abort()
	{
	set +e
	exit -1
	}

trap 'abort' 0
set -eo pipefail

dir_target="$1"

find . | grep -v '^.$' | grep -v '__.*' | grep -v 'gh-pages' | grep -v '/\..*' \
	| grep -v '.*\.blend1' \
	| tar '--transform=s,^\.,anja-src,' -czf "$dir_target"/anja-src.tar.gz -T -

trap : 0
