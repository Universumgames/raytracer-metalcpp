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

run_cicd: build
	cd cmake-build-debug && ./RayTracer --no-window --sequential -s scene/scene_simple.json -o raytraced.jpg
	cp cmake-build-debug/raytraced.jpg .

xcode_create:
	mkdir -p cmake-xcode
	cd cmake-xcode && cmake .. -G Xcode
	cd cmake-xcode && open RayTracer.xcodeproj