CPFLAG = -O2 -pthread -Ilib
LDLIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

RM = rm --force

lol: main.o glad.o
	g++ main.o glad.o $(CPFLAG) $(LDLIBS) --output lol

main.o: src/main.cxx
	g++ -c src/main.cxx $(CPFLAG)

glad.o: src/glad.c
	g++ -c src/glad.c $(CPFLAG)

clean:
	$(RM) main.o glad.o
