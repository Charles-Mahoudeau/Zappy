##
## EPITECH PROJECT, 2026
## zappy
## File description:
## Makefile
##

all: zappy_server zappy_gui zappy_ai

cpp_build:
	cmake -S zappy -B build -DCMAKE_BUILD_TYPE=Release

zappy_server: | cpp_build
	cmake --build build --target zappy_server
	cp build/src/zappy/server/zappy_server .

zappy_gui: | cpp_build
	cmake --build build --target zappy_gui
	cp build/src/zappy/gui/zappy_gui .

zappy_ai:
	echo 'zappy_ai is not implemented yet'

clean:
	rm -rf build

fclean: clean
	rm -f zappy_server zappy_gui

.PHONY: all cpp_build clean fclean
