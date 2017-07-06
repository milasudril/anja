 #@	{
#@	"targets":
#@		[{
#@		"name":"mainwindowstart.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"portselector.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"anja_layout.txt","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"anja_jackports.txt","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		}]
#@	}

set -e

target_dir=$1
in_dir=$2

Xvfb :5 -screen 0 1366x768x24 -fbdir /dev/shm &
server=$!
export DISPLAY=:5
while ! xdpyinfo >/dev/null 2>&1; do
	sleep 1
done

export JACK_DEFAULT_SERVER=dummy
jackd --no-mlock -p 64 --no-realtime -d dummy -p 4096 &
jack=$!
jack_wait -w --server $JACK_DEFAULT_SERVER
tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT INT TERM HUP
mkfifo "$tmpdir/anja_fifo"
"$target_dir"/anja --theme=light --script="$tmpdir/anja_fifo" > "$target_dir"/"$in_dir"/anja_layout.txt &
anja=$!
while ! jack_lsp | grep anja >/dev/null 2>&1; do
	sleep 1
done
jack_lsp | grep '\.anja' > "$target_dir"/"$in_dir"/anja_jackports.txt
anjawin=$(xdotool search --all --onlyvisible --pid $anja)
import -window $anjawin "$target_dir"/"$in_dir"/mainwindowstart.png
echo "layout inspect" > "$tmpdir/anja_fifo"
echo "port selector open,18" > "$tmpdir/anja_fifo"
sleep 1
anjawin=$(xdotool search --all --name "Master out: Port selection")
import -window $anjawin "$target_dir"/"$in_dir"/portselector.png
echo "exit" > "$tmpdir/anja_fifo"
wait $anja
kill -9 $jack
kill $server
exit 0
