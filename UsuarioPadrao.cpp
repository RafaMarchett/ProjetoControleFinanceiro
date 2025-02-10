// 
// 
// Project by Rafael Marchetti
// January 24, 2025
// 
//

#ifdef USUARIOPADRAO

#include "UsuarioPadrao.hpp"

//==================================================================
//--- Funcões-Membro ---

UsuarioPadrao::UsuarioPadrao(string nomeSalvamento) : _nomeSalvamento(nomeSalvamento),simboloMoeda("R$"), senha("0"), numeroCrip(0), orcamentoMaximo(999999999999999), gastoDoMes(0), saldo(0) , salario(0)
{
    cout <<  nomeSalvamento << endl;
    iniciarSistema();
}


UsuarioPadrao::~UsuarioPadrao()
{
    salvarDados();
}

void UsuarioPadrao::salvarDados()
{
    ofstream ArqParaSalvar(_nomeSalvamento, ios::out | ios::binary);
    if (!ArqParaSalvar) {
        cerr << boldVermelho << "Erro ao salvar o arquivo!\n";
        exit(1);
    }

    // Salvar data members
    criptografarValor(orcamentoMaximo); criptografarValor(gastoDoMes); criptografarValor(gastoMensal); criptografarValor(saldo); criptografarValor(salario); 
    ArqParaSalvar.write(reinterpret_cast<char*>(&numeroCrip), sizeof(numeroCrip));           //Dar isso em todas as variaveis, com o tamanho da própria variavel
    ArqParaSalvar.write(reinterpret_cast<char*>(&orcamentoMaximo), sizeof(orcamentoMaximo)); //MENOS PARA STRINGS
    ArqParaSalvar.write(reinterpret_cast<char*>(&gastoDoMes), sizeof(gastoDoMes));           //Pasa o primeiro elemento como referência
    ArqParaSalvar.write(reinterpret_cast<char*>(&gastoMensal), sizeof(gastoMensal));
    ArqParaSalvar.write(reinterpret_cast<char*>(&saldo), sizeof(saldo));
    ArqParaSalvar.write(reinterpret_cast<char*>(&salario), sizeof(salario));

    // Save strings
    criptografarValor(nome);
    size_t size = nome.size(); //Pega o tamanho de nome. Em um sistema 32 bits,e um u_int, em um de 64 e um u_long long
    ArqParaSalvar.write(reinterpret_cast<char*>(&size), sizeof(size)); //Escreve o próprio tamanho
    ArqParaSalvar.write(nome.c_str(), size);   //.c_str para um ponteiro char para o inicio na string, depois passa size. Salva a string sem '\0'

    size = simboloMoeda.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&size), sizeof(size));
    ArqParaSalvar.write(simboloMoeda.c_str(), size);

    size = senha.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&size), sizeof(size));
    ArqParaSalvar.write(senha.c_str(), size);

    // Save vectors<vectors>
    size_t tamanho = todasSimulacoes.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)); //reserva o tamanho

        // Salva cada um dos vetores internos
    for (auto& vetorInterno : todasSimulacoes) 
    {
        size_t tamanhoInterno = vetorInterno.size();
        ArqParaSalvar.write(reinterpret_cast<char*>(&tamanhoInterno), sizeof(tamanhoInterno));
    

        ArqParaSalvar.write(reinterpret_cast<char*>(vetorInterno.data()), tamanhoInterno * sizeof(double));
    }
    

    tamanho = _simulacoesComplemento.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));


        // Salva cada um dos vetores internos
    for (auto& vetorInterno : _simulacoesComplemento) 
    {
        size_t tamanhoInterno = vetorInterno.size();
        ArqParaSalvar.write(reinterpret_cast<char*>(&tamanhoInterno), sizeof(tamanhoInterno));
        ArqParaSalvar.write(reinterpret_cast<char*>(vetorInterno.data()), tamanhoInterno * sizeof(int));
    }

    // Map de string -> double
    tamanho = despesasDoMes.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
    mp::iterator ite;

    for (auto ite = despesasDoMes.begin(); ite != despesasDoMes.end(); ite++) 
    {

        // Salva o tamanho da string
        size_t tamanhoString = ite->first.size();
        ArqParaSalvar.write(reinterpret_cast<char*>(&tamanhoString),sizeof(tamanhoString));
        
        // Salva o conteudo da string
        ArqParaSalvar.write(ite->first.c_str(), tamanhoString);

        // Salva o conteudo do double
        ArqParaSalvar.write(reinterpret_cast<char*> (&ite->second), sizeof(ite->second));
    }

    tamanho = despesasMensais.size();
    ArqParaSalvar.write(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
    ite;

    for (auto ite = despesasMensais.begin(); ite != despesasMensais.end(); ite++) 
    {

        // Salva o tamanho da string
        size_t tamanhoString = ite->first.size();
        ArqParaSalvar.write(reinterpret_cast<char*>(&tamanhoString),sizeof(tamanhoString));
        
        // Salva o conteudo da string
        ArqParaSalvar.write(ite->first.c_str(), tamanhoString);
        
        // Salva o conteudo do double
        ArqParaSalvar.write(reinterpret_cast<char*> (&ite->second), sizeof(ite->second));
    }

    cout << boldVerde << "\n\n\nDados salvos e programa encerrado\n" << noBold;

    ArqParaSalvar.close();
}

void UsuarioPadrao::criarArquivo()
{
    std::ifstream ArquivoTeste(_nomeSalvamento, std::ios::binary);
    if (ArquivoTeste) {
        cerr << boldVermelho << "Ja existem dados salvos nesse arquivo!" << noBold << endl;
        exit(0); // Sai do programa sem sobrescrever o arquivo
    }
    ArquivoTeste.close();

    // Agora, crie o arquivo porque sabemos que ele nao existe
    std::ofstream ArquivoCriar(_nomeSalvamento, std::ios::out | std::ios::binary);
    if (!ArquivoCriar) {
        cerr << boldVermelho << "Erro ao criar o arquivo!" << noBold << endl;
        exit(1); // Sai com erro
    }

    // O arquivo foi criado com sucesso
    cout << "Arquivo criado com sucesso!" << endl;
}//end criarArquivo

void UsuarioPadrao::carregarDados()
{
    ifstream CarregarAqr(_nomeSalvamento, ios::in | ios::binary);
    
    // Carrega os membros de dados
    
    CarregarAqr.read(reinterpret_cast<char*>(&numeroCrip), sizeof(numeroCrip)); //Bem parecido com os anteriores, mas dessa vez com read
    CarregarAqr.read(reinterpret_cast<char*>(&orcamentoMaximo), sizeof(orcamentoMaximo));
    CarregarAqr.read(reinterpret_cast<char*>(&gastoDoMes), sizeof(gastoDoMes));
    CarregarAqr.read(reinterpret_cast<char*>(&gastoMensal), sizeof(gastoMensal));
    CarregarAqr.read(reinterpret_cast<char*>(&saldo), sizeof(saldo));
    CarregarAqr.read(reinterpret_cast<char*>(&salario), sizeof(salario));
    
    // Load strings

    size_t size; //Pra strings, fica como um sizeoff(size)
    CarregarAqr.read(reinterpret_cast<char*>(&size), sizeof(size)); // O conteudo de size vai ser substituido com o conteuudo do arquivo
    nome.resize(size);  //A linha tem size caracteres
    CarregarAqr.read(&nome[0], size); //Ponteiro para o inicio da string, faz isso ate o fim dela (size)

    CarregarAqr.read(reinterpret_cast<char*>(&size), sizeof(size));
    simboloMoeda.resize(size);
    CarregarAqr.read(&simboloMoeda[0], size);

    CarregarAqr.read(reinterpret_cast<char*>(&size), sizeof(size));
    senha.resize(size);
    CarregarAqr.read(&senha[0], size);
    
    // Load vector<vector>

    size_t tamanho = todasSimulacoes.size();
    CarregarAqr.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)); //reserva o tamanho
    todasSimulacoes.resize(tamanho); //Como a variavel esta zerada, da um resize do tamanho que ela tem q ter

        // Salva cada um dos vetores internos
    for (auto& vetorInterno : todasSimulacoes) 
    {
        size_t tamanhoInterno;
        CarregarAqr.read(reinterpret_cast<char*>(&tamanhoInterno), sizeof(tamanhoInterno)); //Inicializa o tamanho interno aqui mesmo
        vetorInterno.resize(tamanhoInterno);

        CarregarAqr.read(reinterpret_cast<char*>(vetorInterno.data()), tamanhoInterno * sizeof(double));
    }
    
    // load vector 2
        tamanho = _simulacoesComplemento.size();
    CarregarAqr.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)); //reserva o tamanho
    _simulacoesComplemento.resize(tamanho); //Como a variavel esta zerada, da um resize do tamanho que ela tem q ter

        // Salva cada um dos vetores internos
    for (auto& vetorInterno : _simulacoesComplemento) 
    {
        size_t tamanhoInterno;
        CarregarAqr.read(reinterpret_cast<char*>(&tamanhoInterno), sizeof(tamanhoInterno)); //Inicializa o tamanho interno aqui mesmo
        vetorInterno.resize(tamanhoInterno);


       CarregarAqr.read(reinterpret_cast<char*>(vetorInterno.data()), tamanhoInterno * sizeof(int));
    }

    // load map<string,double>
    tamanho;
    CarregarAqr.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)); // Ler o numero de pares

    despesasDoMes.clear(); // Limpa qualquer lixo de despesasDoMes

    for (size_t i = 0; i < tamanho; i++) {
        // Ler o tamanho da string
        size_t tamanhoString;
        CarregarAqr.read(reinterpret_cast<char*>(&tamanhoString), sizeof(tamanhoString));

        // Ler a string
        string chave;
        chave.resize(tamanhoString); //resize em chave para ter o tamanho da string
        CarregarAqr.read(&chave[0], tamanhoString); //Le de 0 ao final da string

        // Ler o valor associado (double)
        double valor;
        CarregarAqr.read(reinterpret_cast<char*>(&valor), sizeof(valor));

        // Inserir no map
        despesasDoMes[chave] = valor;

    }

    // load map<string,double> 2
    tamanho;
    CarregarAqr.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho)); // Ler o numero de pares

    despesasMensais.clear(); // Limpa qualquer lixo de despesasMensais

    for (size_t i = 0; i < tamanho; i++) {
        // Ler o tamanho da string
        size_t tamanhoString;
        CarregarAqr.read(reinterpret_cast<char*>(&tamanhoString), sizeof(tamanhoString));

        // Ler a string
        string chave;
        chave.resize(tamanhoString); //resize em chave para ter o tamanho da string
        CarregarAqr.read(&chave[0], tamanhoString); //Le de 0 ao final da string

        // Ler o valor associado (double)
        double valor;
        CarregarAqr.read(reinterpret_cast<char*>(&valor), sizeof(valor));

        // Inserir no map
        despesasMensais[chave] = valor;


    }
    descriptografar(orcamentoMaximo); descriptografar(gastoDoMes); descriptografar(gastoMensal); descriptografar(saldo); descriptografar(salario); 
    descriptografar(nome);
    CarregarAqr.close();
}

void UsuarioPadrao::inserirSenha()
{
    bool x = 0;
    u_int8_t tentativas = 0;

    string tempSenha = getSenha();
    string inputSenha;
    descriptografar(tempSenha);
    do{
        if(tentativas < 3){
            cout << "Insira a sua senha\n>>> ";
            inputIgnore(inputSenha);
            #if DEBUG
            cout << "TEMP IMPUT: " << inputSenha << endl;
            #endif
            ++tentativas;
            if(inputSenha == tempSenha){

              x = 1;
        }//end if
            else if (tentativas < 3)
              cout << boldVermelho << "A senha inserida esta incorreta, você ainda tem " << (3 - tentativas) << " tentativas" << noBold << endl;
        }//end if

        else{
            cerr << boldVermelho  << "Sistema encerrado: 3 Erros consecutivos na insercao da senha" << noBold << endl;
            exit(0);
        }
        
    }while(x == 0);
    
}// end inserirSenha

void UsuarioPadrao::descriptografar(string& val)
{
    if (getNumeroCrip() != 0)
        for (char &c : val)
        {
            c -= numeroCrip;
        }
} // end descriptografar

void UsuarioPadrao::descriptografar(int& val)
{
    long localNumeroCrip = getNumeroCrip();
    val -= numeroCrip;
} // end descriptografar

void UsuarioPadrao::descriptografar(long& val)
{
    long localNumeroCrip = getNumeroCrip();
    val -= numeroCrip;
} // end descriptografar

void UsuarioPadrao::descriptografar(double& val)
{
    long localNumeroCrip = getNumeroCrip();
    val -= numeroCrip;
} // end descriptografar

void UsuarioPadrao::criptografarSenha()
{
    numeroCrip = 0; 
    string tempSenha = getSenha();
    if (numeroCrip == 0 && (getSenha().size()) >= 8)
    {
        for (char &c : getSenha())
        {
            if (numeroCrip % 2 == 0)
                numeroCrip += c;
            else{
                numeroCrip *= c;
            }
        } // end for
        
    } // end if

    for (char &c : tempSenha)
    {
        c += numeroCrip;
    }
    setSenha(tempSenha);
} // end criptografarSenha

void UsuarioPadrao::criptografarValor(string& valorCriptografar)
{
    #if DEBUG
    cout << "Criptografando " << valorCriptografar << endl;
    #endif
    long localNumeroCrip = getNumeroCrip();

    for (auto &c : valorCriptografar)
    {
        c += localNumeroCrip;
    }
    #if DEBUG
    cout << "Criptografado " << valorCriptografar << endl;
    #endif
}// end criptografarValor

void UsuarioPadrao::criptografarValor(int& valorCriptografar)
{
    long localNumeroCrip = getNumeroCrip();
    valorCriptografar += localNumeroCrip;
}// end criptografarValor

void UsuarioPadrao::criptografarValor(long& valorCriptografar)
{
    long localNumeroCrip = getNumeroCrip();
    valorCriptografar += localNumeroCrip;
}// end criptografarValor

void UsuarioPadrao::criptografarValor(double& valorCriptografar)
{
    long localNumeroCrip = getNumeroCrip();
    valorCriptografar += (localNumeroCrip);
}// end criptografarValor

void UsuarioPadrao::iniciarSistema()
{
    ifstream testeArq(_nomeSalvamento, ios::in | ios::binary);
    if (!testeArq) { 
        testeArq.close();
        criarArquivo();
        dadosIniciais();
    }
    else{  
        testeArq.close();
        carregarDados();
        login();
    }
    
    menus();

} // end iniciarSistema

void UsuarioPadrao::dadosIniciais()
{
    cout << boldBranco << "\n\n### GERENCIADOR FINANCEIRO ###" << endl
         << noBold;
    string val;

    cout << "Insira o seu nome completo: \n>>> ";
    getline(cin, val);

    setNome(val);

    while (getSenha().size() < 8)
    {
        cout << "Insira a sua senha (mais de 8 digitos): \n>>> ";
        inputIgnore(val);
        if (val.size() < 8)
            cout << boldVermelho << "Insira uma senha com pelo menos 8 caracteres" << noBold << endl;
        setSenha(val);
        criptografarSenha();
    }

    cout << "Insira o simbolo da sua moeda (EX: R$)\n>>> ";
    inputIgnore(val);
    setSimboloMoeda(val);

    double tempSaldo;
    cout << "Insira o seu saldo atual\n>>> ";
    inputIgnore(tempSaldo);
    setSaldo(tempSaldo);

    char opt = '0';

    cout << bold << "Gostaria de definir um salario mensal? (S/N)\n>>> " << noBold;
    inputIgnore(opt);

    if (opt == 's' || opt == 'S')
    {
        definirSalario();
    } // end if

    cout << bold << "Gostaria de definir um orcamento maximo mensal?(S/N)\n>>> " << noBold;
    inputIgnore(opt);

    if (opt == 's' || opt == 'S')
    {
        gerarOrcamentoMaximo();
    } // end if

}//end dadosIniciais

void UsuarioPadrao::login()
{
    cout << boldBranco << "\n\n### GERENCIADOR FINANCEIRO ###" << endl
         << noBold;
    string val;

    inserirSenha();

}// end login

void UsuarioPadrao::menus()
{
    char opt = '/';
    bool encerrar = 0;
    
    do 
    {
        opt = '/';
        cout << boldBranco << "\n\n### GERENCIADOR FINANCEIRO ###\n"
             << noBold
             << "0. Simular virada de mês\n"
             << "1. Registrar despesas por categorias\n"
             << "2. Exibir gasto total por categoria\n"
             << "3. Adicionar saldo\n"
             << "4. Gerar um orcamento maximo mensal\n"
             << "5. Simular investimentos\n"   
             << "6. Gerar relatórios de investimentos\n"   
             << "7. Exibir dados da conta *\n"   
             << "8. Alterar dados da conta **\n"
             << "9. Encerrar o programa\n\n"
             << "* Nome, Simbolo de moeda, Orcamento Maximo, Despesas Totais\n  Saldo Restante, Salario Mensal\n"
             << "** Nome, Simbolo de Moeda, Salario Mensal, Senha\n"
             << ">>> ";
        inputIgnore(opt);

        switch (opt)
        {
        case '0':
            passarMes();
            break;
        case '1':
            regDespesas();
            break;
        case '2':
            mostrarDespesas();
            break;
        case '3':
            adicionarSaldo();
            break;
        case '4':
            gerarOrcamentoMaximo();
            break;
        case '5':
            simularInvestimentos();
            break;

        case '6':
            gerarRelatorio();
            break;

        case '7':
            exibirMaisValores();
            break;
        case '8':
            alterarOutrosValores();
            break;
        case '9':
            encerrar = 1;
            break;

        case ' ':
        case '\n':
        case '\t':
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }
    } while (encerrar == 0);
    
} // end menus

void UsuarioPadrao::passarMes()
{
    setSaldo((getSaldo()+getSalario()));
    cout << "\nSeu saldo após o salario: " << (getSaldo() < 0 ? boldVermelho : boldVerde) << getSimboloMoeda() << ' ' << getSaldo() << endl << noBold;
    setGastoDoMes(0);
    despesasDoMes.clear();
    mp::iterator ite;
    int i = 0;

    for (ite = despesasMensais.begin(); ite != despesasMensais.end(); ite++){     
        setGastoDoMes((getGastoDoMes() + ite->second));
        cout << "GASTO DO MES TESTE: " << getGastoDoMes() << endl;
        setSaldo((getSaldo() - ite->second));
        cout << "SALDO TESTE: " << getSaldo() << endl;

    }//end for
    cout << "Seu saldo após os gastos do mes: " << (getSaldo() < 0 ? boldVermelho : boldVerde) << getSimboloMoeda() << ' ' << getSaldo() << endl << noBold;

    if(getSaldo() < 0)
    {
        cout << boldVermelho << "AVISO: SEUS GASTOS MENSAIS TE DEIXARAM COM DINHEIRO NEGATIVO, ADICIONE SALDO OU PAGARA JUROS MENSAIS!" << endl;
        setSaldo((getSaldo() + (getSaldo()/100)));
        cout << "Você perdeu " << (getSaldo()/100) << " em juros por estar com dinheiro negativo na conta!" << endl;
    }
    cout << "Aperte \"Enter\" para continuar\n>>> ";
    cin.ignore(10000,'\n');
}//end passarMes

void UsuarioPadrao::regDespesas()
{
    // inserirSenha();

    string inputTipo;
    double inputValor;
    char opt;
    cout << "Essa despeza sera mensal (S/N)?\n>>> ";
    inputIgnore(opt);
    if(opt == 's'||opt == 'S'){
        cout << "Insira o tipo de despeza mensal, ou \"N/A\" para cancelar" << endl
             << ">>> ";
        getline(cin, inputTipo);
        if (inputTipo == "N/A")
          menus();


        if (despesasMensais.find(inputTipo) != despesasMensais.end())
          cout << "Agora, insira o valor da despesa em " << getSimboloMoeda() << endl << ">>> ";

        else
          cout << "Agora, insira o que sera acrescentado na despesa " << inputTipo << " em " << getSimboloMoeda() << endl << ">>> ";


        inputIgnore(inputValor);
        for (auto &c : inputTipo)
            c = tolower(c);
        inputTipo[0] = toupper(inputTipo[0]);

        if (despesasMensais.find(inputTipo) != despesasMensais.end() && inputValor && (getGastoMensal() + inputValor <= getOrcamentoMaximo() && (getSaldo() - inputValor) >= 0))
        {
            despesasMensais[inputTipo] += inputValor;
            cout << boldVerde << "Valor da despesa " << inputTipo << " agora e " << fixed << setprecision(2) << ' ' << getSimboloMoeda() << despesasMensais[inputTipo] << noBold << endl;
            setGastoDoMes((getGastoMensal() + inputValor));
            setSaldo((getSaldo() - inputValor)); 
        } // end if
        else if (getGastoMensal() + inputValor <= getOrcamentoMaximo() && (getSaldo() - inputValor) >= 0)
        {
            despesasMensais[inputTipo] = inputValor;
            cout << boldVerde << "Despesa " << inputTipo << " adicionada com o valor " << getSimboloMoeda() << ' ' << inputValor << noBold << endl;
            setGastoMensal((getGastoMensal() + inputValor));
            setSaldo((getSaldo() - inputValor));
        } // end else
        else if ((getSaldo() - inputValor) >= 0)
            cout << boldVermelho << "O seu gasto mensal ultapassou o limite do orcamento. Aumente seu orcamento maximo mensal ou espere ate o próximo mes" << noBold << endl;
        else
            cout << boldVermelho << "Você ficou sem saldo para realizar esse gasto" << noBold << endl;
    }//if mensal

    else{
        cout << "Insira o tipo de despeza, ou \"N/A\" para cancelar" << endl
             << ">>> ";
        getline(cin, inputTipo);

        if (inputTipo == "N/A")
          menus();

        cout << "Agora, insira o valor da despesa em " << getSimboloMoeda() << endl
             << ">>> ";
        inputIgnore(inputValor);
        for (auto &c : inputTipo)
            c = tolower(c);

        if (despesasDoMes.find(inputTipo) != despesasDoMes.end() && inputValor && (getGastoDoMes() + inputValor <= getOrcamentoMaximo() && (getSaldo() - inputValor) >= 0))
        {
            despesasDoMes[inputTipo] += inputValor;
            cout << boldVerde << "Valor da despesa " << inputTipo << " agora e " << fixed << setprecision(2) << ' ' << getSimboloMoeda() << despesasDoMes[inputTipo] << noBold << endl;
            setGastoDoMes((getGastoDoMes() + inputValor));
            setSaldo((getSaldo() - inputValor));
        } // end if
        else if (getGastoDoMes() + inputValor <= getOrcamentoMaximo() && (getSaldo() - inputValor) >= 0)
        {
            despesasDoMes[inputTipo] = inputValor;
            cout << boldVerde << "Despesa " << inputTipo << " adicionada com o valor " << getSimboloMoeda() << ' ' << inputValor << noBold << endl;
            setGastoDoMes((getGastoDoMes() + inputValor));
            setSaldo((getSaldo() - inputValor));
        } // end else
        else if ((getSaldo() - inputValor) >= 0)
            cout << boldVermelho << "O seu gasto mensal ultapassou o limite do orcamento. Aumente seu orcamento maximo mensal ou espere ate o próximo mes" << noBold << endl;
        else
            cout << boldVermelho << "Você ficou sem saldo para realizar esse gasto" << noBold << endl;
    }//else naoMensal

    cout << "Registrar nova despeza? (S/N)\n>>> ";
    inputIgnore(opt);

    if(opt == 's' || opt == 'S')
      regDespesas();
    
} // end regDespesas

void UsuarioPadrao::mostrarDespesas()
{
    // inserirSenha();
    mp::iterator ite;
    int i = 0;
    
    cout << "\n\n" << fixed << setprecision(2)  << bold << setw(40) << left << "TIPO DE DESPESA" << setw(20) << right << "VALOR DA DESPESA" << endl; 
    for (ite = despesasDoMes.begin(); ite != despesasDoMes.end(); ite++){
        cout << ++i << ". " << setw(40) << left << ite->first << setw(20) << right << getSimboloMoeda() << ' ' << ite->second << endl;

    }//end for
    cout << bold << setw(60) << right << "TOTAL: " << getSimboloMoeda() << ' ' << getGastoDoMes() << "\n\n" << noBold;
    
}//end mostrarDespesas

void UsuarioPadrao::gerarOrcamentoMaximo()
{
    double val, temp = getOrcamentoMaximo();
    setOrcamentoMaximo(0);
    cout << "Insira o orcamento maximo desejado em " << getSimboloMoeda() << " \n>>> ";
    inputIgnore(val);

    if((val >= getGastoDoMes())){
        setOrcamentoMaximo(val);
        cout << boldVerde << "O seu orcamento maximo agora e de: " << getSimboloMoeda() << ' ' << getOrcamentoMaximo() << endl << noBold;
    }//end if
    else{
        cout << boldVermelho << "O seu orcamento ja estrapolou os gastos" << noBold << endl;
        setOrcamentoMaximo(temp);
    }//end else
}//end gerarOrcamentoMaximo

void UsuarioPadrao::adicionarSaldo()
{
    double input;
    cout << "Insira quanto saldo sera inserido em " << getSimboloMoeda() << " \n>>> ";
    inputIgnore(input);
    setSaldo((getSaldo()+input));

    cout << boldVerde << "Seu saldo agora e de " << getSimboloMoeda() << ' ' << getSaldo()  << noBold;
}//end adicionarSaldo

void UsuarioPadrao::exibirMaisValores()
{
    // inserirSenha();
    cout << boldBranco << "\n\n### VALORES DA CONTA ###" << noBold << endl;
    cout << "Nome: " << getNome() << '\n'
         << "Simbolo de moeda preferido: " << boldBranco << getSimboloMoeda()     << '\n' << noBold
         << "Orcamento maximo:   "     << boldBranco;
    if(getOrcamentoMaximo() != 999999999999999) { cout << getOrcamentoMaximo() << '\n' << noBold; }
    else { cout << "NAO DEFINIDO AINDA" << '\n' << noBold; }
    
    cout << "Despesas desse mes: "         << boldBranco  << getGastoDoMes()      << '\n' << noBold
         << "Despesas mensais:   "         << boldBranco  << getGastoMensal()     << '\n' << noBold
         << "Saldo restante:     "         << boldBranco  << getSaldo()           << '\n' << noBold
         << "Salario mensal:     "         << boldBranco  << getSalario()         << endl << noBold;
    
    cout << "Aperte \"Enter\" para continuar\n>>> ";
    cin.ignore(10000,'\n');

}//end exibirMaisValores

void UsuarioPadrao::alterarOutrosValores()
{
    inserirSenha();
    char opt = '0';
    bool x = 0;
    string inputString = "0";
    double inputDouble = 0.0;
    do
    {
        cout << "\nInsira qual dos valores deseja alterar\n"
            << "1. Nome\n"
            << "2. Simbolo de moeda preferido\n"
            << "3. Salario mensal\n"
            << "4. Alterar senha\n"
            << "5. Sair\n"
            << ">>> ";
        inputIgnore(opt);
        switch (opt)
        {
        case '1':
        {
            cout << "Insira o novo nome da conta\n>>> ";
            inputIgnore(inputString);
            setNome(inputString);
            break;
        }//end case '1'

        case '2':
        {
            cout << "Insira o novo simbolo de moeda preferido da conta\n>>> ";
            inputIgnore(inputString);
            setSimboloMoeda(inputString);
            break;
        }//end case '2'
        case '3':
        {
            cout << "Insira o novo salario mensal\n>>> ";
            inputIgnore(inputDouble);
            setSalario(inputDouble);
            break;
        }//end case '3'
        case '4':
        {
            inputString = "0";
            while (inputString.size() < 8)
            {
                cout << "Insira a sua nova senha (mais de 8 digitos): \n>>> ";
                inputIgnore(inputString);
                if (inputString.size() < 8)
                    cout << boldVermelho << "Insira uma senha com pelo menos 8 caracteres" << noBold << endl;
                setSenha(inputString);
                criptografarSenha();
            }
            break;
        }//end case '3'
        case '5':
            return;
            break;
        case ' ':
        case '\n':
        case '\t':
            break;
        default:
            cout << "Opcao invalida" << endl;
            break;
        }

    } while (x == 0);
    

}//end alterarOutrosValores

void UsuarioPadrao::simularInvestimentos()
{
    char opt = '0';
    short periodo = 0;
    double taxa = 0.0, valorInvestido = 0.0, valorFinal = 0.0, valorMensal = 0.0;

  
        opt = '0';
        cout << "\n1. Simular investimento de renda fixa\n"
             << "2. Simular investimento de renda variavel\n"
             << "3. Sair\n>>> ";

        inputIgnore(opt);

        switch (opt)
        {
        case '1':
        {
            cout << boldBranco << "Insira a taxa do investimento em % ao ano (EX: 10.5%)\n>>> " << noBold;
            inputIgnore(taxa);
            taxa /= 12;
            cout << boldBranco << "Agora, insira o valor inicial investido em " << getSimboloMoeda() << " \n>>> " << noBold;
            inputIgnore(valorInvestido);
            cout << boldBranco << "Agora, insira o valor mensal investido em " << getSimboloMoeda() << " \n>>> " << noBold;
            inputIgnore(valorMensal);
            cout << boldBranco << "Por ultimo, insira o periodo em meses\n>>> " << noBold;
            inputIgnore(periodo);
            
            vector<double> vetorParaGrafico;       //Isso vai ser usado no salvemento

            valorFinal = calcularJuroscompostosFixo(valorInvestido,taxa,periodo,valorMensal,vetorParaGrafico);
            printarGraficoInvestimentos(vetorParaGrafico,periodo);

            cout << boldVerde << "O valor final e de : " << getSimboloMoeda() << ' ' << valorFinal << noBold << endl;
            todasSimulacoes.push_back(vetorParaGrafico);

            cout << "\n\n"; int i = 1;
            cout << setfill('_') << setw(104) << "_\n"; cout << setfill(' ');
            for(auto& c : vetorParaGrafico){
                cout << fixed << setprecision(2) << setw(10) << c << (i % 8 == 0 && i != 0 ? " | \n" : " | "); 
                i++;
            }// end for tabela
            cout << '\n';
            cout << setfill('-') << setw(104) << "_"; cout << setfill(' ');
            cout << "\n\n";
            cout << "Pressione \"Enter\" para continuar\n>>> ";
            cin.ignore(1000,'\n');

            cout << "\nGostaria de fazer uma nova simulacao?(S/N)\n>>> ";
            inputIgnore(opt);

            if(opt == 's'||opt == 'S') simularInvestimentos();
            break;
        } // end case: '1'

        case '2':
        {
            cout << boldBranco << "Agora, insira o valor inicial investido em " << getSimboloMoeda() << " \n>>> " << noBold;
            inputIgnore(valorInvestido);
            cout << boldBranco << "Agora, insira o valor mensal investido em " << getSimboloMoeda() << " \n>>> " << noBold;
            inputIgnore(valorMensal);
            cout << boldBranco << "Por ultimo, insira o periodo em meses\n>>> " << noBold;
            inputIgnore(periodo);

            vector<double> vetorParaGrafico;
            valorFinal = calcularJuroscompostosVariavel(valorInvestido,0.0,periodo,valorMensal,vetorParaGrafico);
            printarGraficoInvestimentos(vetorParaGrafico,periodo);
            todasSimulacoes.push_back(vetorParaGrafico);

            #if DEBUG
            cout << "TEMP VECTOR: \n";
                for(int i = 0; i < (vetorParaGrafico.size() - 1); i++)
                  cout << vetorParaGrafico[i] << (i % 20 == 0 ? "\n" : ", ");
                cout << '\n';
            #endif
            
            cout << boldVerde << "O valor final e de : " << getSimboloMoeda() << ' ' << valorFinal << noBold << endl;

            cout << "\n\n"; int i = 1;
            cout << setfill('_') << setw(104) << "_\n"; cout << setfill(' ');
            for(auto& c : vetorParaGrafico){
                cout << fixed << setprecision(2) << setw(10) << c << (i % 8 == 0 && i != 0 ? " | \n" : " | "); 
                i++;
            }// end for tabela
            cout << '\n';
            cout << setfill('-') << setw(104) << "_"; cout << setfill(' ');
            cout << "\n\n";
            cout << "Pressione \"Enter\" para continuar\n>>> ";
            cin.ignore(1000,'\n');

            cout << "\nGostaria de fazer uma nova simulacao?(S/N)\n>>> ";
            inputIgnore(opt);

            if(opt == 's'||opt == 'S') simularInvestimentos();
            break;
        } // end case: '2'
            
        case '3': return; break;
        case ' ':
        case '\n':
        case '\t':
        break;

        default:
            cout << "Opcao Invalida" << endl;
            break;
        } // end switch

}//end simularInvestimentos

template <typename T>
void UsuarioPadrao::inputIgnore(T& val)
{
    cin >> val;
    cin.ignore(1000,'\n');
}//end inputIgnore

void UsuarioPadrao::definirSalario()
{
    double input = 0.0;
    cout << "Insira um valor para o seu salario mensal em " << getSimboloMoeda() << " \n>>> ";
    inputIgnore(input);
    setSalario(input);
    cout << "O seu salario agora e de: "  << getSimboloMoeda() << ' ' << getSalario() << endl;

}//end definirSalario
double UsuarioPadrao::calcularJuroscompostosFixo(double valor, double taxa, int meses, double valorMensal, vector<double>& vetorParaGrafico)
{
    
    if (meses == 0)
        return valor;
    
    valor = (valor + valorMensal) * (1 + (taxa / 100));
    vetorParaGrafico.push_back(valor);
    return calcularJuroscompostosFixo(valor, taxa, meses - 1, valorMensal,vetorParaGrafico);  

}//end calcularJuroscompostos

double UsuarioPadrao::calcularJuroscompostosVariavel(double valor, double taxa, int meses, double valorMensal, vector<double>& vetorParaGrafico)
{
    int valorAleatorio = rand() % 100 + 1;
    double temp = valor;
    if (meses == 0)
    {
        return valor;
    }

    if(valorAleatorio < 10)
    {
        taxa = -0.017;
        valor = (valor + valorMensal) * (1 + (taxa / 100));
    }

    else if (valorAleatorio < 40)
    {
        taxa = -0.011;
        valor = (valor + valorMensal) * (1 + (taxa / 100));
    }
    else if (valorAleatorio < 76)
    {
        taxa = 0.013;
        valor = (valor + valorMensal) * (1 + (taxa / 100));
    }
    else
    {
        taxa = 0.18;
        valor = (valor + valorMensal) * (1.04 + (taxa / 100));
    }
    #if DEBUG
    cout << "NUMERO ALEATORIO: " << valorAleatorio << '\t' << "TEMP VALOR " << valor << '\t' << "RENDIMENTO: " << (valor)-(temp+valorMensal) << endl;
    #endif

    vetorParaGrafico.push_back(valor);
    return calcularJuroscompostosVariavel(valor, taxa, meses - 1, valorMensal, vetorParaGrafico);
}//end calcularJuroscompostosVariavel

void UsuarioPadrao::printarGraficoInvestimentos(const vector<double>& investmentValues, int maxBars) {
    int contadorDiminuicao = 0;
    char opt = '/';
    int maxBarsSave = maxBars;
    cout << "\n\n" << boldBranco << setfill('#') << setw(50) << " Grafico " << setw(50) << ' ' << setfill(' ') << noBold
         << "\nLinhas representam os meses e colunas a evolucao patrimonial\n";

    //Faz uma diminuicao controlada no tamanho das linhas, caso o usuario quiser
    if((investmentValues.size()) >= 70){
        while(opt != '1' && opt != '0'){
            cout << "AVISO: Grafico com muitas linhas" << boldBranco << ". Digite 0 para fazer uma diminuicao controlada, ou 1 para continuar\n>>> " << noBold;
            cin >> opt;
            cin.ignore(10000,'\n');

        }//end while opt

        while (maxBars > 70 && opt == '0')
        {
            (maxBars % 2 == 0 ? maxBars /= 2 : ((maxBars+=1) /=2) );
            contadorDiminuicao++;
        }//end while diminuicao

    }//end if
    else {cout << "Aperte enter para continuar e exibir o grafico\n>>> "; cin.ignore(1000,'\n');}
    cout << "\n\n" << bold;
    double maxInvestment = *max_element(investmentValues.begin(), investmentValues.end()); // Numero maximo de barras que podemos mostrar (*max_elements pega o maior valor do intervalo, desreferenciado)
                                                                                           //ISSO DEIXA AS BARRAS PROPORCIONAIS

    // Para cada "linha" do grafico
    for (int i = maxBars; i > 0; --i) {
        cout << right << setw(4) << i << " | "; // Exibe a linha com o numero
        for (size_t j = 0; j < investmentValues.size(); j++) {
            int numBars = static_cast<int>((investmentValues[j] / maxInvestment) * maxBars); // Calcula a quantidade de barras
            if (numBars >= i) {
                cout << '#';
            } else {
                cout << ' ';
            }
            cout << ' '; // Espaco entre as barras
        }
        cout << endl;
    }

    // Exibe a linha com os numeros das colunas
    cout << '\t';
    for (size_t j = 0; j < investmentValues.size(); ++j) {
        cout << "- ";
    }
    cout << "\n\n";

    if(contadorDiminuicao != 0) cout << noBold << "Cada linha no grafico representam aproximadamente " << (contadorDiminuicao*2) << " meses" << endl;


    vector<int> complementoParaGrafico; //Isso vai ser usado no salvamento
    complementoParaGrafico.push_back(maxBarsSave);
    complementoParaGrafico.push_back(contadorDiminuicao);
    _simulacoesComplemento.push_back(complementoParaGrafico);
} // end printarGraficoInvestimentos

void UsuarioPadrao::printarGraficoGuardado()
{
    unsigned short contador = 0;
    for(auto& graficoIndividual : todasSimulacoes){
        
        int maxBars;
        maxBars = _simulacoesComplemento[contador][0];
        
        const vector<double>& investmentValues = graficoIndividual;
        int contadorDiminuicao = 0;
        char opt = '/';
        int maxBarsSave = maxBars;
        cout << "\n\n" << boldBranco << setfill('#') << setw(50) << " Grafico " << setw(50) << ' ' << setfill(' ') << noBold
            << "\nLinhas representam os meses e colunas a evolucao patrimonial\n";

        //Faz uma diminuicao controlada no tamanho das linhas, caso o usuario quiser
        if((investmentValues.size()) >= 70){
            while(opt != '1' && opt != '0' && opt != '2'){
                cout << "AVISO: Grafico com muitas linhas" << boldBranco << ". Digite 0 para fazer uma diminuicao controlada, ou 1 para continuar. OU PRESSIONE 2 PARA SAIR\n>>> " << noBold;
                cin >> opt;
                cin.ignore(10000,'\n');
            }//end while opt
            if(opt == '2') 
              menus();
            while (maxBars > 70 && opt == '0')
            {
                (maxBars % 2 == 0 ? maxBars /= 2 : ((maxBars+=1) /=2) );
                contadorDiminuicao++;
            }//end while diminuicao

        }//end if
        else {cout << "Aperte enter para continuar e exibir o grafico\n>>> "; cin.ignore(1000,'\n');}
        cout << "\n\n" << bold;
        double maxInvestment = *max_element(investmentValues.begin(), investmentValues.end()); // Numero maximo de barras que podemos mostrar (*max_elements pega o maior valor do intervalo, desreferenciado)
                                                                                            //ISSO DEIXA AS BARRAS PROPORCIONAIS

        // Para cada "linha" do grafico
        for (int i = maxBars; i > 0; --i) {
            cout << setw(2) << i << " | "; // Exibe a linha com o numero
            for (size_t j = 0; j < investmentValues.size(); j++) {
                int numBars = static_cast<int>((investmentValues[j] / maxInvestment) * maxBars); // Calcula a quantidade de barras
                if (numBars >= i) {
                    cout << '#';
                } else {
                    cout << ' ';
                }
                cout << ' '; // Espaco entre as barras
            }
            cout << endl;
        }

        // Exibe a linha com os numeros das colunas
        cout << '\t';
        for (size_t j = 0; j < investmentValues.size(); ++j) {
            cout << "- ";
        }
        cout << "\n\n";

        if(contadorDiminuicao != 0) cout << noBold << "Cada linha no grafico representam aproximadamente " << (contadorDiminuicao*2) << " meses" << endl;

        cout << "\n\n"; int i = 1;
        cout << setfill('_') << setw(104) << "_\n"; cout << setfill(' ');
        for(auto& c : graficoIndividual){
            cout << fixed << setprecision(2) << setw(10) << c << (i % 8 == 0 && i != 0 ? " | \n" : " | "); 
            i++;
        }// end for tabela
        cout << '\n';
        cout << setfill('-') << setw(104) << "_"; cout << setfill(' ');
        cout << "\n\n";
        cout << "Pressione \"Enter\" para continuar\n>>> ";
        cin.ignore(1000,'\n');
        contador++;
    }


}//end printarGraficoPadrao

void UsuarioPadrao::exibirTabela()
{
    for(auto& graficoIndividual : todasSimulacoes){
        cout << "\n\n";
        // for(auto& _graficoIndividual : _simulacoesComplemento){
            for (int i = 0; i < graficoIndividual.size(); i++)
            {
                cout << graficoIndividual[i] << (i % 8 == 0 ? "\n" : " | "); 
            }
            
        }//end for interno
    // }//end for externo
}//end exibir tabela

void UsuarioPadrao::gerarRelatorio()
{
    printarGraficoGuardado();
    exibirTabela();
}//end gerarRelatorio

//==================================================================
//--- Getter e Setter ---

string UsuarioPadrao::getNome() const
{
    return nome;
}

void UsuarioPadrao::setNome(string val)
{
    nome = val;
}

string UsuarioPadrao::getSimboloMoeda() const
{
    return simboloMoeda;
}

void UsuarioPadrao::setSimboloMoeda(string val)
{
    simboloMoeda = val;
}

string UsuarioPadrao::getSenha() const
{
    return senha;
}

void UsuarioPadrao::setSenha(string val)
{
    senha = val;
    // criptografarSenha();
}

long UsuarioPadrao::getNumeroCrip() const
{
    return numeroCrip;
}

void UsuarioPadrao::setNumeroCrip(long val)
{
    numeroCrip = val;
    
}

double UsuarioPadrao::getOrcamentoMaximo() const
{
    return orcamentoMaximo;
}

void UsuarioPadrao::setOrcamentoMaximo(double val)
{
    orcamentoMaximo = val;
    
}

double UsuarioPadrao::getGastoDoMes() const
{
    return gastoDoMes;
}

void UsuarioPadrao::setGastoDoMes(double val)
{
    gastoDoMes = val;
    
}

double UsuarioPadrao::getGastoMensal() const
{
    return gastoDoMes;
}

void UsuarioPadrao::setGastoMensal(double val)
{
    gastoDoMes = val;
    
}

double UsuarioPadrao::getSaldo() const
{
    return saldo;
}

void UsuarioPadrao::setSaldo(double val)
{
    saldo = val;
}

double UsuarioPadrao::getSalario() const
{
    return salario;
}

void UsuarioPadrao::setSalario(double val)
{
    salario = val;
}

#endif // USUARIOPADRAO