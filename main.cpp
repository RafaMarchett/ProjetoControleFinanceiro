// 
// 
// Project by Rafael Marchetti
// January 24, 2025
// 
// 

#include "UsuarioPadrao.hpp"
int main(){
    srand(time(0));

    string arquivoSalvamento = "0";
    // cout << "Insira o nome do arquivo para salvemento COM COMPLEMENTO (.dat,.txt...)\n>>> ";
    cout << "Insira o nome do arquivo para salvamento SEM COMPLEMENTO\n>>> ";
    cin >> arquivoSalvamento;
    cin.ignore(1000,'\n');
    arquivoSalvamento += ".dat";
    
    UsuarioPadrao teste1(arquivoSalvamento);

    return 0;
}

