#pragma once

namespace tebd {

	class DbItem {
		public:
			//TODO definir retornos e argumentos do metodo
			virtual void insertIntoDb (void) = 0;	//metodo puramente abstrato que devera retornar dados necessarios para insercao no banco de dados
			//a keyword virtual indica que o metodo utiliza polimorfismo
			//quando um metodo virtual recebe o valor 0 em sua definicao isso indica que nao ha uma implementacao do metodo, ou seja, que ele eh abstrato
	};

};