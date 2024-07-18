CPFLAG = -O2 -pthread -Iinclude
LDLIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

RM = rm --force

lol.out: main.o glad.o
	g++ main.o glad.o $(CPFLAG) $(LDLIBS) --output lol.out

main.o: src/main.cxx
	g++ -c src/main.cxx $(CPFLAG)

glad.o: glad/glad.c
	g++ -c glad/glad.c $(CPFLAG)

clean:
	$(RM) main.o glad.o
