#@	{
#@	"targets":
#@		[{
#@		 "name":"message_info.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"../icons/message_info.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$in_dir"/../icons/message_info.svg "$dir_target"/"$in_dir"/message_info.svg.pdf