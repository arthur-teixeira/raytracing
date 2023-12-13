all:
	mkdir -p out; g++ main.cpp -Ofast -o out/main; ./out/main > out/img.ppm; feh out/img.ppm
preview:
	mkdir -p out; g++ main.cpp -Ofast -DPREVIEW=true -o out/main; ./out/main > out/preview.ppm; feh out/preview.ppm
