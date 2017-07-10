#@	{
#@	"targets":
#@		[{
#@		"name":"playseq.svg","dependencies":
#@			[{"ref":"dot","rel":"tool"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"playseq.dot","rel":"misc"}]
#@		}]
#@	}

set -e
if ! command -v dot>/dev/null; then
	echo "# Error: Dot is not installed"
	exit 1
fi

if ! command -v xsltproc>/dev/null; then
	echo "# Error: Dot is not installed"
	exit 1
fi

target_dir="$1"
in_dir="$2"

dot -Tsvg "$in_dir"/playseq.dot | xsltproc --novalid "$in_dir"/dotsvgfilter.xsl - \
	> "$target_dir"/"$in_dir"/"playseq.svg"
