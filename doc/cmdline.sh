#@	{
#@	"targets":
#@		[{
#@		"name":"cmdline.xml","dependencies":
#@			[{"ref":"sed","rel":"tool"}
#@			,{"ref":"markdown","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}
#@			,{"ref":"cmdline.xsl","rel":"misc"}]
#@		}]
#@	}
dir_target="$1"
in_dir="$2"
set -e
set -o pipefail

__targets_dbg/anja --help | csplit --prefix="$dir_target"/"$in_dir"/cmdline_ - '/^--*/-1'  > /dev/null
csplit "$dir_target"/"$in_dir"/cmdline_01 --prefix="$dir_target"/"$in_dir"/cmdline_01_ '/^Common types$/' > /dev/null

sed 's/\. For.*/. The different argument types accepted by the different options are/' "$dir_target"/"$in_dir"/cmdline_00 \
	| (cat && tail -n +3 "$dir_target"/"$in_dir"/cmdline_01_01 \
		| sed 's/^\([a-z][a-z ]*\) \:=/ * *\1* :=/') \
	| cat - "$dir_target"/"$in_dir"/cmdline_01_00 \
	| sed 's/\(^--[a-z][a-z\-]*\)\(\[\?=\)\([a-z ]*\)/ * `\1`\2*\3*/g' \
	| sed 's/^    \(.*\)/\n \1/' \
	| markdown \
	| (cat <<EOF
<content>
EOF
cat
cat <<EOF
</content>
EOF
) | xsltproc "$in_dir"/cmdline.xsl - > "$dir_target"/"$in_dir"/cmdline.xml
