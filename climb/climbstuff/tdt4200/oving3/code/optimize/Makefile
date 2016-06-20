.PHONY: clean

all: newImageIdea

newImageIdea: newImageIdea.c ppm.c
	gcc -O2 --std=c99 -o $@ newImageIdea.c ppm.c

clean:
	rm -f newImageIdea 
