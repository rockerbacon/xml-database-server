#include "Aluno.h"
#include "Database.h"

//TODO implementar metodo de insercao de aluno no banco
void tebd::Aluno::insertIntoDb (void) {
    Database db;
    std::string valores = "'"+nome+"','"+cpf+"','"+universidade+"','"+curso+"'";
    db.execute_insert("ALUNO",valores);
};

