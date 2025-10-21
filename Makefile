install_deps_apt:
	apt update
	apt install -y cmake build-essential make gcc git libsfml-dev libudev-dev libopenal-dev libvorbis-dev libflac-dev libx11-dev xserver-xorg-dev xorg-dev

setup_cmake:
	mkdir -p cmake-build-debug
	cd cmake-build-debug && cmake ..

build: setup_cmake
	cd cmake-build-debug && cmake --build .

run: build
	cd cmake-build-debug && ./RayTracer