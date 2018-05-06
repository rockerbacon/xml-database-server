CCX=g++ -std=c++11
#toda vez que um novo arquivo .cpp eh criado DEVE SER ADICIONADO AQUI com a terminacao .o
OBJS=Aluno.o FileManager.o Log.o
CFLAGS=-pthread
LFLAGS=-lstdc++fs -lxerces-c

all: $(OBJS)

%.o: %.cpp
	$(CCX) -c $<
	
test_%: test_%.cpp $(OBJS) %.o
	$(CCX) $(CFLAGS) $^ -o $@ $(LFLAGS)
	
clean:
	rm -f *.o
