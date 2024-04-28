
/*
    ARVORE RUBRO NEGRA
    JOÃO MARCOS BRITO MATIAS - 494952
    
    PARA RODAR O CODIGO BASTA COMPILAR COM G++
    "g++ main.cpp"

    Se você não colocar nome do arquivo de saída (usando o "-o" no compilador)
    o nome do arquivo de saída será "a.out"

    basta rodar o arquivo de saida colocando o arquivo txt das instruções como parametro
    " ./"nome do arquivo de saida".out "comandos".txt "
*/


#include "arvorerb.hpp"
#include <fstream>


int main(int nargs, char* argv[]){

    if(nargs <= 1){
        cout << "não há argumentos suficientes para o programa rodar\n";
        cout << "Tente colocar como argumento o txt para executar ações na árvore\n";
        return 1;
    }

    string entrada = argv[1];

    ifstream arq (entrada.data());

    string s;

    ArvoreRN T;
    int v = 0;
    int linha = 0;

    while(!arq.eof()){
        string instrucao;

        try{
            if(getline(arq, s)){
                instrucao = s.substr(0, 3);
                linha = linha + 1;

                if("INC" == s.substr(0,3)){ 
                    T.inserir(stoi(s.substr(4)), v);
                }

                if("IMP" == s.substr(0,3)){
                    T.imprimir(stoi(s.substr(4)), v);
                }

                if("REM" == s.substr(0,3)){
                    T.deletar(stoi(s.substr(4)), v);
                }

                if("SUC" == s.substr(0,3)){
                    string n1;

                    int i = 4;
                    while(s[i] != ' '){
                        n1.push_back(s[i]);
                        i = i + 1;
                    }

                    string n2 = s.substr(i+1);

                    int res =  T.sucessor(stoi(n1), stoi(n2), v);

                    if(res == INT_MAX) cout << "INFINITO\n";
                    else cout << res << "\n";
                }
            }
        }
        catch(exception& e){
            cout << "\nERRO: A INSTRUÇÃO '" << instrucao <<"' NA LINHA " << linha <<" NÃO ESTÁ ESCRITA CORRETAMENTE\n";

            
            if("INC" == instrucao) cout << "O correto é 'INC' 'valor que será incluso'\n";
            if("REM" == instrucao) cout << "O correto é 'REM' 'valor que será removido'\n";
            if("SUC" == instrucao) cout << "O correto é 'SUC' 'valor que será buscado' 'versão'\n";
            if("IMP" == instrucao) cout << "O correto é 'IMP' 'versão'\n";

            cout << "Corrija o arquivo de texto\n\n";

        

            return 1;
        }
    }

}

