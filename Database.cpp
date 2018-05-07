#include "Database.h"

//TODO implementar metodo de insercao de aluno no banco
tebd::Database::Database(){
    int result = sqlite3_open("TEBD.db", &db);
    if (result) {
        printf("Não foi possível abrir o banco de dados. Erro:%s ", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    //create_tables();
}


void tebd::Database::create_tables(){
    char *erro=NULL;
    sqlite3_exec(db, "CREATE TABLE ALUNO (NOME VARCHAR(50), CPF VARCHAR(15), UNIVERSIDADE VARCHAR(50), CURSO VARCHAR(50));", NULL, NULL, &erro);
    sqlite3_exec(db, "CREATE TABLE MATERIA (TAG VARCHAR(50),CODIGO VARCHAR(50),NOME VARCHAR(50),CREDITOS INTEGER,CONCEITO REAL, SITUACAO VARCHAR(50));", NULL, NULL, &erro);
    sqlite3_exec(db, "CREATE TABLE PERIODO (TAG VARCHAR(50),CPF_ALUNO VARCHAR(50),ANO VARCHAR(50), SEMESTRE VARCHAR(50));", NULL, NULL, &erro);
    sqlite3_exec(db, "CREATE TABLE MATERIA_PERIODO (CODIGO_MATERIA VARCHAR(50),ANO_PERIODO VARCHAR(50),SEMESTRE_PERIODO VARCHAR(50));", NULL, NULL, &erro);
    if (erro!=NULL) {      
    //      se a tabela já existe pode acontecer erro... ignorar
    //      printf(stderr, "Não foi possível criar");
        delete erro;
    }
}

tebd::Database::~Database() {
    sqlite3_close(db);
}

int tebd::Database::execute_insert(std::string tabela, std::string valores){
    std::string nonquery = "INSERT INTO "+tabela+" VALUES ("+valores+")";
    char *erro=NULL;
    int result = sqlite3_exec(db, nonquery.c_str(), NULL, NULL, &erro);
    if (erro!=NULL) {
        printf("Problema ao inserir o registro no banco de dados. Mensagem de erro: %s ", erro);
    }
    return result;
}
