#@	{
#@	"targets":
#@		[{
#@		"name":"playseq.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"playseq.dot","rel":"misc"}]
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

dot -Tsvg "$in_dir"/playseq.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"playseq.svg"

trap : 0
