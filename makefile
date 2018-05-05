CCX=g++ -std=c++11
#toda vez que um novo arquivo .cpp eh criado DEVE SER ADICIONADO AQUI com a terminacao .o
OBJS=Aluno.o FileManager.o
CFLAGS=-pthread

%.o: %.cpp
	$(CCX) -c $(CFLAGS) $<
	
clean:
	rm -f *.o
