#@	{
#@	"targets":
#@		[{
#@		"name":"modinteract.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"modinteract.dot","rel":"misc"}
#@			,{"ref":"dotsvgfilter.xsl","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}]
#@		},{
#@		"name":"modinteract.svg.pdf","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"playseq.dot","rel":"misc"}
#@			,{"ref":"dotsvgfilter.xsl","rel":"misc"}
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

dot -Tsvg "$in_dir"/modinteract.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"modinteract.svg"

"$in_dir"/svgtopdf.sh "$target_dir"/"$in_dir"/"modinteract.svg" "$target_dir"/"$in_dir"/"modinteract.svg.pdf"


trap : 0
