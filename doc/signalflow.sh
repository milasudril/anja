#@	{
#@	"targets":
#@		[{
#@		"name":"signalflow.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"signalflow.dot","rel":"misc"}]
#@		}]
#@	}

set -eo pipefail

target_dir="$1"
in_dir="$2"

dot -Tsvg "$in_dir"/signalflow.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"signalflow.svg"
