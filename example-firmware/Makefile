app: app/prep app/build

app/prep: build/Makefile

app/build:
	make -C build -j $(nproc)

build/Makefile: CMakeLists.txt
	mkdir -p build
	(cd build && cmake .. -DBOARD=video_booster)

flash: app
	cp ${FIRMWARE_BIN} ${FLASH_DIR}

clean:
	rm -rf build

.PHONY: app app/prep app/build clean

