#@	{
#@	"targets":
#@		[{
#@		 "name":"message_usererror.svg.pdf"
#@		,"dependencies":
#@			[
#@			 {"ref":"../icons/message_usererror.svg","rel":"misc"}
#@			,{"ref":"svgtopdf.sh","rel":"misc"}
#@			]
#@		}]
#@	}

dir_target="$1"
in_dir="$2"

"$in_dir"/svgtopdf.sh "$in_dir"/../icons/message_usererror.svg "$dir_target"/"$in_dir"/message_usererror.svg.pdf