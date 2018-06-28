#@	{
#@	"dependencies_extra":
#@		[
#@			 {"ref":"format.css","rel":"include_extra"}
#@			,{"ref":"color.css","rel":"include_extra"}
#@			,{"ref":"anja-usersguide.pdf","rel":"include_extra"}
#@			,{"ref":"makepage.py","rel":"misc"}
#@			,{"ref":"xsltproc","rel":"tool"}
#@			,{"ref":"awk","rel":"tool"}
#@			,{"ref":"tee","rel":"tool"}
#@			,{"ref":"cp","rel":"tool"}
#@			,{"ref":"grep","rel":"tool"}
#@			,{"ref":"sort","rel":"tool"}
#@			,{"ref":"uniq","rel":"tool"}
#@			,{"ref":"xargs","rel":"tool"}
#@			,{"ref":"tar","rel":"tool"}
#@			,{"ref":"cat","rel":"tool"}
#@		]
#@	}

set -eo pipefail

dest="$1"
src="$2"
in_dir="$3"
dest_dir=`dirname "$1"`
target_dir=`echo "$dest_dir" | sed 's,\(.*\)/.*,\1,'`

locals=`locale -a`
for value in C.UTF-8 en_US.UTF-8 C.en_UK.UTF-8; do
	if echo $locals | grep $value > /dev/null; then
		export LC_ALL=C.UTF-8
		break
	fi
done

for k in "${@:4}"; do
	echo "$k"
done | tee >(grep -v '^__' | tee >(xargs -d '\n' cp -t "$dest_dir") \
	| awk -v prefix="$target_dir" '{print prefix "/" $0}' ) \
	| grep '^__' | sort | uniq > "$dest_dir"/archive.txt

echo "$dest_dir"/index.html >> "$dest_dir"/archive.txt

stylesheets='{"stylesheets":["format.css","color.css"],"in_dir":"'"$target_dir"/"$in_dir"'"}'
xsltproc --path "$dest_dir" "$in_dir"/inputstub.xsl "$src" \
	| "$in_dir"/makepage.py "$stylesheets" > "$dest_dir"/index.html

extract_dir=`basename "${dest%%.*}"`
tar "--transform=s|^$target_dir/$in_dir|$extract_dir|" -czf "$1" -T "$dest_dir"/archive.txt

if [[ -d gh-pages ]]; then
	cat "$dest_dir"/archive.txt| xargs -d'\n' cp -t gh-pages
fi
