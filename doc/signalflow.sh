#@	{
#@	"targets":
#@		[{
#@		"name":"signalflow.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			,{"ref":"signalflow.dot","rel":"misc"}
#@			,{"ref":"dotsvgfilter.xsl","rel":"misc"}]
#@		},{
#@		"name":"signalflow.svg.pdf","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"signalflow.dot","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
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

dot -Tsvg "$in_dir"/signalflow.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"signalflow.svg"

"$in_dir"/svgtopdf.sh "$target_dir"/"$in_dir"/"signalflow.svg" "$target_dir"/"$in_dir"/"signalflow.svg.pdf"

trap : 0
