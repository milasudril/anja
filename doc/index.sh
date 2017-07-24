#@	{
#@	"dependencies_extra":
#@		[
#@			 {"ref":"makepage.py","rel":"misc"}
#@			,{"ref":"format.css","rel":"include_extra"}
#@			,{"ref":"color.css","rel":"include_extra"}
#@			,{"ref":"../icons/message_warning.svg","rel":"include_extra"}
#@			,{"ref":"../icons/message_info.svg","rel":"include_extra"}
#@			,{"ref":"../icons/message_error.svg","rel":"include_extra"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"rm","rel":"tool"}
#@			,{"ref":"cp","rel":"tool"}
#@		]
#@	}

set -eo pipefail

dest=$1
src=$2
in_dir=$3
dest_dir=`dirname "$1"`

export LC_ALL=C.UTF-8 #PEP 538
export LANG=C.UTF-8

stylesheets='{"stylesheets":["format.css","color.css"],"in_dir":"'"$in_dir"'"}'
xsltproc --path "$dest_dir" "$in_dir"/inputstub.xsl "$src" \
	| "$in_dir"/makepage.py "$stylesheets" > "$dest"

if [ -d gh-pages ]; then #Test if we have cloned the gh-pages repo
	rm -r gh-pages/*
	for k in "${@:4}"
	do
		if [[ "${k:0:2}" != "__" ]]; then
			cp "$k" "$dest_dir"
		fi
		cp "$k" gh-pages
	done
	cp "$dest" gh-pages
fi
