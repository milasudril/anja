#@	{
#@	"targets":
#@		[{
#@		"name":"modinteract.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"modinteract.dot","rel":"misc"}
#@			,{"ref":"dotsvgfilter.xsl","rel":"misc"}]
#@		}]
#@	}

abort()
	{
	exit -1
	}
trap 'abort' 0
set -eo pipefail

target_dir="$1"
in_dir="$2"

dot -Tsvg "$in_dir"/modinteract.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"modinteract.svg"

trap : 0
