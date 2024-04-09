#include <iostream>
#include <climits>

#define QTDE_MODS 2
#define QTDE_VERSOES 101

using namespace std;

enum Tag{
    CHAVE, COR, PAI, ESQ, DIR
};

class ArvoreRN{

    struct Noh;

    union Valor{
        int k;
        char c;
        Noh* p;
    };

    //Estrutura do Mod
    struct Mod {int versao;
                Tag tag;
                Valor valor;
    };
    
    
    //Estrutura do noh
    //Os campos de retorno e formado por apenas um no, ja que em uma arvore, cada no recebe em apontado por no max 1 ponteiro
    //Portanto, o campo de modificações terá apenas 2 espaços
    struct Noh{int chave; char cor; Noh* esq; Noh* dir; Noh* pai; Noh* retorno; Mod Mods[QTDE_MODS];};

    //Criando um nó sentinela, assim como o Cormen
    //O noh sentinela vai representar as folhas nulas e será o pai da raiz
    Noh sentinela;

    Valor ler(Noh* n, Tag campo, int v){
        
        Valor resposta;

        switch(campo){
            case CHAVE: resposta.k = n->chave; break;
            case COR:   resposta.c = n->cor;   break;
            case PAI:   resposta.p = n->pai;   break;
            case ESQ:   resposta.p = n->esq;   break;
            case DIR:   resposta.p = n->dir;   break;
        };

        int j = -1;
        int v_aux = 0;

        for(int i = 0; i < QTDE_MODS; i++){
            if(n->Mods[i].tag == campo and n->Mods[i].versao <= v and n->Mods[i].versao > v_aux){
                v_aux = n->Mods[i].versao;
                j = i;
            }
        }

        if(j != -1){
            switch(campo){
                case CHAVE: resposta.k = n->Mods[j].valor.k;   break;
                case COR:   resposta.c = n->Mods[j].valor.c;   break;
                default:    resposta.p = n->Mods[j].valor.p;   break;
            }
        }

        return resposta;
    }

    
    //Falta mexer nos campos de retorno de quem aponta para o noh duplicado
    //Falta fazer o controle de versões
    //Eu quero que a versão mude apenas quando eu for inserir ou retirar um noh
    //As mudanças de ponteiro de pai e filho e cor não devem alterar a versão
    void set(Noh* n, Tag campo, int& v, Valor valor){
        
        int i = 0;

        while(i < QTDE_MODS && n->Mods[i].versao != -1){
            i = i + 1;
        }

        //Caso ainda tenha espaço no mods
        if(i < QTDE_MODS){
            n->Mods[i].versao = v;
            n->Mods[i].valor = valor;
            n->Mods[i].tag = campo;
        }

        //Caso não tenha espaço no mods
        else{

            cout << "passei daqui\n";
            //Duplicando o no
            Noh* novo = new Noh {ler(n, {CHAVE}, v-1).k, 
                                 ler(n, {COR}  , v-1).c, 
                                 ler(n, {ESQ}  , v-1).p, 
                                 ler(n, {DIR}  , v-1).p,
                                 ler(n, {PAI}  , v-1).p,
                                 n->retorno, 
                                 {{-1, {CHAVE}, {0}},{-1, {CHAVE}, {0}}}
                                };

            switch(campo){
                case CHAVE: novo->chave = valor.k; break;
                case PAI:   novo->pai   = valor.p; break;
                case ESQ:   novo->esq   = valor.p; break;
                case DIR:   novo->dir   = valor.p; break;
                case COR:   novo->cor   = valor.c; break;
            }

            Valor val;
            val.p = {novo};
            
            //Caso de N ser raiz
            if(ler(n, {PAI}, v-1).p == &sentinela){
                raiz_versao[v] = novo;
                for(int i = v; i < QTDE_VERSOES; i++){
                    raiz_versao[i] = novo;
                }
            }

            //Caso N não seja raiz
            else{
                
                Noh* pai = ler(n, {PAI}, v).p;

                //Caso N seja filho esq
                if(ler(pai, {CHAVE}, v).k > novo->chave){
                    set(pai, ESQ, v, val);
                }

                //Caso N seja filho dir
                else{
                    set(pai, DIR, v, val);
                }
            }

            Noh* esq = ler(n, {ESQ}, v).p;
            Noh* dir = ler(n, {DIR}, v).p;

            //Avisando para os filhos trocarem de pai

            if(esq != &sentinela){
                set(esq, PAI, v, val);
            }

            if(dir != &sentinela){
                set(dir, PAI, v, val);
            }
        }
    }

    //MUDAR O ALGORITMO DA ROTAÇÂO PRA FICAR IGUAL AO DO CORMEN
    //Rotação ensinada pelo Cormen
    /*
    void rotacionar_esq(Noh* x){
        Noh* y = x->dir;
        x->dir = y->esq;

        if(y->esq != &sentinela){
            y->esq->pai = x;
        }

        y->pai = x->pai;

        if(x->pai == &sentinela){
            raiz_versao[v] = y;
        }

        else{
            if(x == x->pai->esq){
                x->pai->esq = y;
            }
            else{
                x->pai->dir = y;
            }
        }

        y->esq = x;
        x->pai = y;
    }*/
    
    
    void rotacionar_esq(Noh* x, int& v){
        Valor val;
        
        //y = x->dir
        Noh* y = ler(x, DIR, v).p;

        //x->dir = y->esq;
        val = ler(y, ESQ, v);
        set(x, DIR, v, val);

        if(ler(y, ESQ, v).p != &sentinela){
            //y->esq->pai= x;
            val.p = x;
            set(ler(y, ESQ, v).p, PAI, v, val);
        }
        
        //y->pai = x->pai;
        val = ler(x, PAI, v);
        set(y, PAI, v, val);

        if(ler(x,PAI,v).p == &sentinela){
            raiz_versao[v] = y;
        }

        else{
            val.p = {y};
            if(x == ler(ler(x, PAI, v).p, ESQ, v).p){
                //x->pai->esq = y;
                set(ler(x, PAI, v).p, ESQ, v, val);
            }
            else{
                //x->pai->dir = y;
                set(ler(x, PAI, v).p, DIR, v, val);
            }
        }

        val.p = {x};
        set(y, ESQ, v, val);

        val.p = {y};
        set(y, PAI, v, val);

    }

    //Rotação a direita
    /*
    void rotacionar_dir(Noh* x){
        Noh* y = x->esq;
        x->esq = y->dir;

        if(y->dir != &sentinela){
            y->dir->pai = x;
        }

        y->pai = x->pai;

        if(x->pai == &sentinela){
            raiz_versao[v] = y;
        }

        else{
            if(x == x->pai->esq){
                x->pai->esq = y;
            }
            else{
                x->pai->dir = y;
            }
        }

        y->esq = x;
        x->pai = y;
    }*/

    void rotacionar_dir(Noh* x, int&v){
        Valor val;
        
        //y = x->dir
        Noh* y = ler(x, ESQ, v).p;

        //x->dir = y->esq;
        val = ler(y, DIR, v);
        set(x, ESQ, v, val);

        if(ler(y, DIR, v).p != &sentinela){
            //y->esq->pai= x;
            val.p = x;
            set(ler(y, DIR, v).p, PAI, v, val);
        }
        
        //y->pai = x->pai;
        val = ler(x, PAI, v);
        set(y, PAI, v, val);

        if(ler(x,PAI,v).p == &sentinela){
            raiz_versao[v] = y;
        }

        else{
            val.p = {y};
            if(x == ler(ler(x, PAI, v).p, DIR, v).p){
                //x->pai->esq = y;
                set(ler(x, PAI, v).p, DIR, v, val);
            }
            else{
                //x->pai->dir = y;
                set(ler(x, PAI, v).p, ESQ, v, val);
            }
        }

        val.p = {x};
        set(y, DIR, v, val);

        val.p = {y};
        set(y, PAI, v, val);
    }

    void imprimir_rec(Noh* r, int v){

        if(ler(r, {ESQ}, v).p  != &sentinela){
            imprimir_rec(ler(r, {ESQ}, v).p, v);
        }

        cout << "Chave: " << ler(r, {CHAVE}, v).k << " Cor: " << ler(r, {COR}, v).c << "\n";

        if(ler(r, {DIR}, v).p != &sentinela){
            imprimir_rec(ler(r, {DIR}, v).p, v);
        }
    }


    void inserir_fixup(Noh* n, int& v){

        Noh* pai = ler(n,PAI,v).p;
        Valor val;

        while (ler(pai, COR, v).c == 'r'){

            if (pai == ler(ler(pai, PAI, v).p, ESQ, v).p){
                
                Noh* y = ler(ler(pai, PAI, v).p, DIR, v).p;

                if(ler(y, COR, v).c == 'r'){
                    val.c = {'b'};
                    set(pai, COR, v, val);

                    set(y, COR, v, val);
                    
                    val.c = {'r'};
                    set(ler(pai, PAI, v).p, COR, v, val);
                    
                    n = ler(pai, PAI, v).p;
                }

                
                else{
                    if(n == ler(pai, DIR, v).p){
                        n = pai;
                        rotacionar_esq(n, v);
                    }

                    val.c = {'b'};

                    set(pai, COR, v, val);
                    
                    val.c = {'r'};
                    set(ler(pai, PAI, v).p, COR, v, val);

                    rotacionar_dir(ler(pai, PAI, v).p, v);
                }
            }

            //continuar com set e get
            else{
                Noh* y = ler(ler(pai, PAI, v).p, ESQ, v).p;

                if(ler(y, COR, v).c == 'r'){
                    val.c = {'b'};
                    set(pai, COR, v, val);

                    set(y, COR, v, val);
                    
                    val.c = {'r'};
                    set(ler(pai, PAI, v).p, COR, v, val);
                    
                    n = ler(pai, PAI, v).p;
                }

                
                else{
                    if(n == ler(pai, ESQ, v).p){
                        n = pai;
                        rotacionar_dir(n, v);
                    }

                    val.c = {'b'};

                    set(pai, COR, v, val);
                    
                    val.c = {'r'};
                    set(ler(pai, PAI, v).p, COR, v, val);

                    rotacionar_esq(ler(pai, PAI, v).p, v);
                }      
            }
        }

        val.c = {'b'};
        set(raiz_versao[v], COR, v, val);
        v = v+1;
    }


    void transplantar(Noh* u, Noh* v){
        if(u->pai == &sentinela){
            sentinela.pai = v;
        }
        else{
            if(u == u->pai->esq){
                u->pai->esq = v;
            }
            else{
                u->pai->dir = v;
            }
        }

        v->pai = u->pai;
    }

    Noh* minimo(Noh* x){
        while(x->esq != &sentinela){
            x = x->esq;
        }

        return x;
    }

    void delete_fixup(Noh* x, int& v){
        Noh* w;

        while(x != raiz_versao[v] && x->cor == 'b'){
            if(x == x->pai->esq){
                w = x->pai->dir;
                
                if(w->cor == 'r'){
                    w->cor = 'b';
                    x->pai->cor = 'r';
                    rotacionar_esq(x->pai, v);
                    w = x->pai->dir;
                }

                if(w->esq->cor == 'b' && w->dir->cor == 'b'){
                    w->cor = 'r';
                    x = x->pai;
                }
                else{
                    if(w->dir->cor == 'b'){
                        w->esq->cor = 'b';
                        w->cor = 'r';
                        rotacionar_dir(w, v);
                        w = x->pai->dir;
                    }

                    w->cor = x->pai->cor;
                    x->pai->cor = 'b';
                    w->dir->cor = 'b';
                    rotacionar_esq(x->pai, v);
                    x = raiz_versao[v];
                }
            }

            else{
                w = x->pai->esq;
                
                if(w->cor == 'r'){
                    w->cor = 'b';
                    x->pai->cor = 'r';
                    rotacionar_dir(x->pai, v);
                    w = x->pai->esq;
                }

                if(w->dir->cor == 'b' && w->esq->cor == 'b'){
                    w->cor = 'r';
                    x = x->pai;
                }
                else{
                    if(w->esq->cor == 'b'){
                        w->dir->cor = 'b';
                        w->cor = 'r';
                        rotacionar_esq(w, v);
                        w = x->pai->esq;
                    }

                    w->cor = x->pai->cor;
                    x->pai->cor = 'b';
                    w->esq->cor = 'b';
                    rotacionar_dir(x->pai, v);
                    x = raiz_versao[v];
                }
            }
        }

        x->cor = 'b';
    }

    Noh* buscar_rec(Noh* r, int k){
        if(r == &sentinela){
            return &sentinela;
        }

        if(r->chave == k){
            return r;
        }

        if(r->chave < k){
            return buscar_rec(r->dir, k);
        }

        else{
            return buscar_rec(r->esq, k);
        }
    }

    public: 

    Noh* raiz_versao[QTDE_VERSOES];
    
    ArvoreRN(){
        sentinela.esq = sentinela.dir = sentinela.pai = &sentinela;
        sentinela.cor = 'b';

        for(int i = 0; i < QTDE_VERSOES; i++){
            raiz_versao[i] = &sentinela; 
        }
    }

    //Acho que terminei
    //Não textei pq tenho que mexer no fixup ainda
    void inserir(int k, int& v){
        Noh *n =  new Noh {k, 'r', &sentinela, &sentinela, &sentinela, &sentinela, {{-1, {CHAVE}, {0}},{-1, {CHAVE}, {0}}}};


        Noh *y = &sentinela;
        Noh *x = raiz_versao[v];

        if(v == 0){
            for(int i = 0; i < QTDE_VERSOES; i++){
                raiz_versao[i] = n;
            }
        }

        while(x != &sentinela){
            y = x;

            if(ler(n, CHAVE, v).k < ler(x, CHAVE, v).k){ 
                x = ler(x, ESQ, v).p;
            }
            else{
                x = ler(x, DIR, v).p;
            }
        }
        
        Valor val;
        val.p = {y};

        set(n, PAI, v, val);

        //cout << ler(ler(n,PAI,v).p, CHAVE, v).k << "\n";

        if(y == &sentinela){
            sentinela.pai = n;
            raiz_versao[v] = n; 
        }

        else{
            val.p = {n};
            if(ler(n, CHAVE, v).k < ler(y, CHAVE, v).k){    
                set(y, ESQ, v, val);
            }
            else{
                set(y, DIR, v, val);
            }
        }
        
        inserir_fixup(n, v);
    }


    void deletar(int k, int& v){
        Noh* z = buscar(k, v);

        if (z == &sentinela){
            return;
        }

        Noh* y = z;
        Noh* x;

        char y_cor_original = y->cor;

        if(z->esq == &sentinela){
            x = z->dir;
            transplantar(z,z->dir);
        }

        else{
            if(z->dir == &sentinela){
                x = z->esq;
                transplantar(z,z->esq);
            }

            else{
                y = minimo(z->dir);
                y_cor_original = y->cor;
                x = y->dir;
                
                if(y->pai == z){
                    x->pai = y;
                }

                else{
                    transplantar(y, y->dir);
                    y->dir = z->dir;
                    y->dir->pai = y;
                }

                transplantar(z,y);
                y->esq = z->esq;
                y->esq->pai = y;
                y->cor = z->cor;
            }
        }
        
        delete y;

        if(y_cor_original == 'b'){
            delete_fixup(x, v);
        }
    }

    Noh* buscar(int k, int& v){
        return buscar_rec(raiz_versao[v], k);
    }

    void imprimir(int v){
        imprimir_rec(raiz_versao[v], v);
    }

    int sucessor(int k, int v){
        Noh* pos = buscar(k, v);

        if (pos == &sentinela){
            return INT_MAX;
        }

        if(pos->dir != &sentinela){
            return minimo(pos->dir)->chave;
        }

        Noh* y = pos->pai;

        while(y != &sentinela && pos == y->dir){
            pos = y;
            y = y->pai;
        }

        return y->chave;
    }
    

    //Destrutor da árvore
    void Deletar(Noh* r, int& v){
        if(r != &sentinela){
            Deletar(r->esq, v);
            Deletar(r->dir, v);
            deletar(r->chave, v);
        }
    }
    
    /*
    ~ArvoreRN(){
        for(int i = 0; i < QTDE_VERSOES; i++)
            Deletar(raiz_versao[i], i);    
    }*/



    //ler(Noh* n, Tag campo, int v)
    //set (Noh* n, Tag campo, int& v, Valor valor)
    void teste(int& v){    

        //set(raiz_versao[v], CHAVE, v, {5});
        //set(raiz_versao[v], CHAVE, v, {3});
        
        //set(raiz_versao[v], CHAVE, v, {4});

        //set(raiz_versao[v], CHAVE, v, {5});

        imprimir(v);

        cout << v << "\n";
        
    }
};


int main(){

    ArvoreRN T;
    
    int v = 0;
    
    T.inserir(1, v);
    T.inserir(2, v);
    //T.inserir(3, v);
    //T.inserir(4, v);
    T.teste(v);
    

}

/*
    Caso o set seja de CHAVE, eu preciso balancear o noh, para continuar atendendo
    a propriedade de rubro negra (provavelmente vou ter que fazer o inserir noh).

    Então eu tenho que ajustar o inserir noh para depois balancear o set.

    Depois falta ajustar o Deletar noh.

    Depois formatar conforme a saida que o trabalho pede
*/

//FALTA TERMINAR O INSERIR