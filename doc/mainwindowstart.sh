#@	{
#@	"targets":
#@		[{
#@			"name":"mainwindowstart.png","dependencies":
#@				[{"ref":"Xvfb","rel":"tool"}
#@				,{"ref":"import","rel":"tool"}
#@				,{"ref":"../anja","rel":"misc"}
#@				]
#@		}]
#@	}

set -e

target_dir=$1
in_dir=$2
Xvfb :5 -screen 0 1366x768x24 -fbdir /dev/shm &
server=$!
sleep 0.5
DISPLAY=:5 "$target_dir"/anja &
anja=$!
sleep 0.5
anjawin=$(DISPLAY=:5 xdotool search --any --onlyvisible --pid $anja)
DISPLAY=:5 import -window $anjawin "$target_dir"/"$in_dir"/mainwindowstart.png
kill $anja
kill $server
