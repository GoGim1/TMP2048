SHELL = /bin/bash

help:
	@echo "Usage:"
	@echo "  make start		start or restart the game"
	@echo "  make left		move the numbers to the left"
	@echo "  make right		move the numbers to the right"
	@echo "  make up		move the numbers to the top"
	@echo "  make down		move the numbers to the buttom"

start:
	@clear && g++ -std=c++17 -DRANDOM=$$RANDOM -o 2048 main.cc && ./2048
right:
	@clear && g++ -std=c++17 -DRANDOM=$$RANDOM -DRIGHT -o 2048 main.cc && ./2048
left:
	@clear && g++ -std=c++17 -DRANDOM=$$RANDOM -DLEFT -o 2048 main.cc && ./2048
up:
	@clear && g++ -std=c++17 -DRANDOM=$$RANDOM -DUP -o 2048 main.cc && ./2048
down:
	@clear && g++ -std=c++17 -DRANDOM=$$RANDOM -DDOWN -o 2048 main.cc && ./2048



