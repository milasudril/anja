#@	{
#@	"targets":
#@		[{
#@		 "name":"message_warning.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"../icons/message_warning.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$in_dir"/../icons/message_warning.svg "$dir_target"/"$in_dir"/message_warning.svg.pdf