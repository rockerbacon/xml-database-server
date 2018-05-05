CCX=g++ -std=c++11
#toda vez que um novo arquivo .cpp eh criado DEVE SER ADICIONADO AQUI com a terminacao .o
OBJS=Aluno.o FileManager.o
CFLAGS=-pthread
LFLAGS=-lstdc++fs

all: $(OBJS)

%.o: %.cpp
	$(CCX) -c $<
	
test_%: $(OBJS) %.o
	$(CCX) $(CFLAGS) $@.cpp $^ -o $@ $(LFLAGS)
	
clean:
	rm -f *.o
