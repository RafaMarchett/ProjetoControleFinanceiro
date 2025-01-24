// 
// 
// Project by Rafael Marchetti
// January 24, 2025
// 
//

#ifndef USUARIOPADRAO
#define USUARIOPADRAO
#include "header.hpp"


typedef map<string,double, less<string>>       mp;   //Para criar Containers MAP de tipo inteiro

class UsuarioPadrao
{

public:
//--- Funções-Membro ---
    UsuarioPadrao(string);
    ~UsuarioPadrao();




protected:
    void carregarDados();
    void salvarDados();
    void criarArquivo();
    
    void criptografarSenha();

    void criptografarValor(string&);
    void criptografarValor(int&);
    void criptografarValor(long&);
    void criptografarValor(double&);
    void descriptografar(string&);
    void descriptografar(int&);
    void descriptografar(long&);
    void descriptografar(double&);

    void dadosIniciais();
    void login();
    void iniciarSistema();

    void inserirSenha();
  
    void menus();

    void passarMes();

    void regDespesas();
    void mostrarDespesas();

    void gerarOrcamentoMaximo();

    virtual void gerarRelatorio();

    void simularInvestimentos();

    double calcularJuroscompostosFixo(double, double, int, double,vector<double>&);
    double calcularJuroscompostosVariavel(double, double, int, double, vector<double>&);

    void printarGraficoInvestimentos(const vector<double>&, int);

    template <typename T>
    void inputIgnore(T& val);

    void adicionarSaldo();

    void exibirMaisValores();

    void alterarOutrosValores();

    void definirSalario();

    //--- Getter e Setter ---
    string getNome() const;
    void   setNome(string val);
    
    string getSimboloMoeda() const;
    void   setSimboloMoeda(string val);
    
    string getSenha() const;
    void   setSenha(string val);
    
    long getNumeroCrip() const;
    void setNumeroCrip(long val);

    double getOrcamentoMaximo() const;
    void   setOrcamentoMaximo(double val);

    double getGastoDoMes() const;
    void   setGastoDoMes(double val);
    
    double getGastoMensal() const;
    void   setGastoMensal(double val);

    void printarGraficoGuardado();
    void exibirTabela();

    double getSaldo() const;
    void   setSaldo(double val);

    double getSalario() const;
    void   setSalario(double val);

    vector<vector<double>> todasSimulacoes;        // Evolução patrimonial
    vector<vector<int>>    _simulacoesComplemento; // Periodo/contador
private:
    string nome, simboloMoeda, senha;
    long numeroCrip;
    double orcamentoMaximo, gastoDoMes, gastoMensal, saldo, salario;

    mp despesasDoMes, despesasMensais;
    

    string _nomeSalvamento;
};

#include "UsuarioPadrao.cpp"

#endif