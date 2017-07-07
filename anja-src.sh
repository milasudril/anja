#@	{
#@	"targets":
#@		[{
#@		"name":"anja-src.tar.gz","dependencies":
#		Track `projectinfo.hpp` instead of versioninfo, so this file is executed
#		after `projectinfo.py`. Also notice that we will always regenerate the
#		source archive, since `projectinfo.hpp` is dynamic.
#@			[{"ref":"projectinfo.hpp","rel":"misc"}]
#@		}]
#@	}

dir_target=$1
find . | grep -v '^.$' | grep -v '__.*' | grep -v 'gh-pages' | grep -v '/\..*' \
	| grep -v '.*\.blend1' \
	| tar '--transform=s,^\.,anja-src,' -czf "$dir_target"/anja-src.tar.gz -T -
