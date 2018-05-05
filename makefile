CCX=g++ -std=c++11
#toda vez que um novo arquivo .cpp eh criado DEVE SER ADICIONADO AQUI com a terminacao .o
OBJS=Aluno.o

%.o: %.cpp
	$(CCX) $^ -o $<
