all: compile link

compile:
	g++ -c sourceFiles\\*.cpp -I"C:\Users\kaich\Documents\VS_library\SFML-2.5.1\include" -DSFML_STATIC

link:
	g++ *.o -o dinosaur_game -L"C:\Users\kaich\Documents\VS_library\SFML-2.5.1\lib" -lstdc++fs -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lgdi32 -mwindows -lsfml-main

clean:
	rm -f main *.o