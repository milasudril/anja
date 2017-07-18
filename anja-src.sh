#@	{
#@	"targets":
#@		[{
#@		"name":"anja-src.tar.gz","dependencies":
#		Track `projectinfo.hpp`. This means that, in case `versioninfo.txt` is
#		being updated we will get the new version. If we track `versioninfo.txt`
#		instead, it may happen that this script will be executed before
#		`projectinfo.py` because `versioninfo.txt` may already exist.
#@			[{"ref":"projectinfo.hpp","rel":"misc"}
#@			,{"ref":"find","rel":"tool"}
#@			,{"ref":"grep","rel":"tool"}
#@			,{"ref":"tar","rel":"tool"}]
#@		}]
#@	}

abort()
	{
	exit -1
	}
trap 'abort' 0
set -eo pipefail

dir_target=$1
find . | grep -v '^.$' | grep -v '__.*' | grep -v 'gh-pages' | grep -v '/\..*' \
	| grep -v '.*\.blend1' \
	| tar '--transform=s,^\.,anja-src,' -czf "$dir_target"/anja-src.tar.gz -T -

trap : 0