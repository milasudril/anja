#@	{
#@	"dependencies_extra":
#@		[
#@			 {"ref":"xetex.xsl","rel":"misc"}
#@			,{"ref":"inputstub.xsl","rel":"misc"}
#@			,{"ref":"texscape","rel":"misc"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"xelatex","rel":"tool"}
#@		]
#@	}

set -eo pipefail

dest="$1"
src="$2"
in_dir="$3"
dest_dir=`dirname "$1"`
target_dir=`echo "$dest_dir" | sed 's,\(.*\)/.*,\1,'`

xsltproc --path "$dest_dir" "$in_dir"/inputstub.xsl "$src" > /dev/shm/temp.xml

xsltproc --path "$dest_dir" "$in_dir"/inputstub.xsl "$src" \
	| xsltproc "$in_dir"/xetex.xsl - \
	| "$dest_dir"/texscape > "$dest_dir"/manual.tex


cd "$dest_dir"
for svg in *.svg; do
	inkscape "$svg" --export-pdf="$svg".pdf "$svg"
done

xelatex -file-line-error-style -halt-on-error manual.tex \
	&& xelatex -file-line-error-style -halt-on-error manual.tex
