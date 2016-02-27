for var in "$@"
do
	echo "\n$var:"
	echo "MODO 1:"
	./Trabalho_2/TRABALHO2 "$var" ./lossless.wav ./lossy.wav 480 5 1
	echo "MODO 2:"
	./Trabalho_2/TRABALHO2 "$var" ./lossless.wav ./lossy.wav 480 5 2
done
