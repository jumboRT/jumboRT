mkdir -p mc
for i in $(seq ${2:-0} ${1:-1} 3599); do
	echo \# FRAME $i
	name=$(printf mc/mc%04d.ppm $i)
	./miniRT scenes/minecraft.rt -k $i -r net -o $name -s 5000 -j 32
done
