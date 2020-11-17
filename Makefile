play: play.cpp
	clang++ -std=c++2a -fcoroutines-ts -stdlib=libc++ play.cpp -o play
generator: generator.cpp mcnellis_generator.h
	clang++ -std=c++2a -fcoroutines-ts -stdlib=libc++ generator.cpp -o generator
clean:
	rm -f play generator a.out core *.o
