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

__targets_dbg/anja --help \
	| sed 's/\(^--[a-z][a-z]*\)\(\[\?=\)\([a-z ]*\)/ * `\1`\2*\3*/g' \
	| markdown | (cat <<EOF
<content>
EOF
cat
cat <<EOF
</content>
EOF
) | xsltproc "$in_dir"/cmdline.xsl - > "$dir_target"/"$in_dir"/cmdline.xml
