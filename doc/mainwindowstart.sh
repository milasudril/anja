#@	{
#@	"targets":
#@		[{
#@		"name":"mainwindowstart.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"waveformloaded.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"mixerfull.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"sessfull.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"portselector.png","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"anja_layout.txt","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		},{
#@		"name":"anja_jackports.txt","dependencies":
#@			[{"ref":"Xvfb","rel":"tool"}
#@			,{"ref":"import","rel":"tool"}
#@			,{"ref":"xdotool","rel":"tool"}
#@			,{"ref":"xdpyinfo","rel":"tool"}
#@			,{"ref":"../anja","rel":"misc"}]
#@		}]
#@	}


set -xeo pipefail

x11_init()
	{
	Xvfb :5 -screen 0 1366x768x24 -fbdir /dev/shm &
	xserver=$!
	export DISPLAY=:5
	while ! xdpyinfo >/dev/null 2>&1; do
		sleep 1
	done
	}

x11_kill()
	{
	kill $xserver 2>/dev/null
	}

jack_init()
	{
	export JACK_DEFAULT_SERVER=dummy
	jackd --no-mlock -p 64 --no-realtime -d dummy -p 4096 &
	jack=$!
	>&2 echo "Waiting for JACK"
	timeout 60 jack_wait -w
	}

jack_kill()
	{
	kill -9 $jack 2>/dev/null #KxStudio buggy
	}

fifo_init()
	{
	tmpdir=$(mktemp -d)
	mkfifo "$tmpdir/anja_fifo"
	}

fifo_kill()
	{
	rm -rf "$tmpdir" 2>/dev/null
	}

anja_kill()
	{
	kill $anja 2>/dev/null
	}

cleanup()
	{
	set +e
	anja_kill
	fifo_kill
	jack_kill
	x11_kill
	}

anja_wait()
	{
	for i in `seq 1 5`; do
		sleep 1
		echo $JACK_DEFAULT_SERVER
		if ! jack_lsp | grep anja >/dev/null 2>&1; then
			>&2 echo "Waiting for Anja"
		else
			return 0
		fi
	done
	>&2 echo "No Anja ports in JACK. Killed?"
	return 1
	}

target_dir=$1
in_dir=$2

trap 'cleanup;exit -1' EXIT INT TERM HUP

x11_init
jack_init
fifo_init

"$target_dir"/anja --theme=light --script="$tmpdir/anja_fifo" > "$target_dir"/"$in_dir"/anja_layout.txt &
anja=$!
anja_wait
>&2 echo "Anja is alive"

jack_lsp | grep '\.anja' > "$target_dir"/"$in_dir"/anja_jackports.txt
anjawin=$(xdotool search --all --onlyvisible --pid $anja)

import -window $anjawin "$target_dir"/"$in_dir"/mainwindowstart.png
exec 3>"$tmpdir/anja_fifo"
echo "layout inspect" >&3
echo "port selector open,18" >&3
sleep 1
anjawin=$(xdotool search --all --name "Master out: Port selection")
import -window $anjawin "$target_dir"/"$in_dir"/portselector.png
echo "port selector close" >&3
echo "waveform load,0,testbank/alien_scanner.wav" >&3
sleep 5
anjawin=$(xdotool search --all --onlyvisible --pid $anja)
import -window $anjawin "$target_dir"/"$in_dir"/waveformloaded.png
echo "settings,channels" >&3
sleep 1
import -window $anjawin "$target_dir"/"$in_dir"/mixerfull.png
echo "settings,session" >&3
sleep 1
import -window $anjawin "$target_dir"/"$in_dir"/sessfull.png
echo "exit" >&3
exec 3<&-
wait $anja
cleanup
trap : 0
