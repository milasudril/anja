#@	{
#@	"targets":
#@		[{
#@		"name":"version.xml","dependencies":
#@			[
#@			 {"ref":"../versioninfo.txt","rel":"misc"}
#@			,{"ref":"cat","rel":"tool"}
#@			]
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

echo -e '<?xml version="1.0"?>
<content><subtitle>version ' > "$dir_target/$in_dir"/version.xml
cat "$dir_target/$in_dir"/../versioninfo.txt >> "$dir_target/$in_dir"/version.xml
echo '</subtitle></content>' >> "$dir_target/$in_dir"/version.xml

trap : 0
