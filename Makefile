watershed: watershed.c watershed.h lodepng.c lodepng.h hachage.h hachage.c main.c
		gcc watershed.c watershed.h lodepng.c lodepng.h hachage.c hachage.h main.c -o watershed -Wall

clean:
	rm -f *.o
distclean: clean
	rm watershed
