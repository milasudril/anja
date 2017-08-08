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

xsltproc --path "$dest_dir" "$in_dir"/inputstub.xsl "$src" \
	| xsltproc "$in_dir"/xetex.xsl - \
	| "$dest_dir"/texscape > "$dest_dir"/anja-usersguide.tex

cd "$dest_dir"

xelatex -file-line-error-style -halt-on-error anja-usersguide.tex 1>&2 \
	&& xelatex -file-line-error-style -halt-on-error anja-usersguide.tex 1>&2 \
	&& xelatex -file-line-error-style -halt-on-error anja-usersguide.tex 1>&2
