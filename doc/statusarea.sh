#@	{
#@	"targets":
#@		[{
#@		 "name":"statusarea.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"statusarea.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$in_dir"/statusarea.svg "$dir_target"/"$in_dir"/statusarea.svg.pdf