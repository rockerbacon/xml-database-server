#include "Database.h"
#include <sqlite3.h>
#include <iostream>

//Construtor
tebd::Database::Database () {
    //Instacia o DB
    int result = sqlite3_open("TEBD.db", &db);
    if (result) {
        printf("Não foi possível abrir o banco de dados. Erro:%s ", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *erro=NULL;
    sqlite3_exec(db, "CREATE TABLE ALUNO (NOME VARCHAR(50), CPF VARCHAR(15), UNIVERSIDADE VARCHAR(50), CURSO VARCHAR(50));", NULL, NULL, &erro);
    if (erro!=NULL) {      
    //      se a tabela já existe pode acontecer erro... ignorar
    //      printf(stderr, "Não foi possível criar");
        delete erro;
    }
}

//Destrutor
tebd::Database::~Database(){ 
   sqlite3_close(db);
}

int tebd::Database::execute_insert(std::string tabela, std::string valores){
    std::string nonquery = "INSERT INTO "+tabela+" VALUES ("+valores+")";
    std::cout << nonquery;
    char *erro=NULL;
    int result = sqlite3_exec(db, nonquery.c_str(), NULL, NULL, &erro);
    if (erro!=NULL) {
        printf("Problema ao inserir o registro no banco de dados. Mensagem de erro: %s ", erro);
    }
    return result;
}



