all:
	mkdir -p out; g++ main.cpp -Ofast -o out/main; ./out/main > out/img.ppm; feh out/img.ppm
