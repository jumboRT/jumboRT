for i in $(seq ${2:-0} ${1:-1} 359); do
	echo \# FRAME $i
	name=$(printf mc%03d.ppm $i)
	./miniRT scenes/minecraft.rt -w 960 -h 540 -k $i -r cpu -r gpu -o $name -s 500
done
