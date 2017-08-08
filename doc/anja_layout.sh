#@	{
#@	"targets":
#@		[{
#@		 "name":"anja_layout.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"anja_layout.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$dir_target"/"$in_dir"/anja_layout.svg "$dir_target"/"$in_dir"/anja_layout.svg.pdf
