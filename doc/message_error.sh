#@	{
#@	"targets":
#@		[{
#@		 "name":"message_error.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"../icons/message_error.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$in_dir"/../icons/message_error.svg "$dir_target"/"$in_dir"/message_error.svg.pdf