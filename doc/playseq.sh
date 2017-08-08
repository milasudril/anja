#@	{
#@	"targets":
#@		[{
#@		"name":"playseq.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"playseq.dot","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}]
#@		},{
#@		"name":"playseq.svg.pdf","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"playseq.dot","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}]
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

"$in_dir"/svgtopdf.sh "$target_dir"/"$in_dir"/"playseq.svg" "$target_dir"/"$in_dir"/"playseq.svg.pdf"

trap : 0
