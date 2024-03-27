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

        int j = 0;
        int v_aux = 0;

        for(int i = 0; i < QTDE_MODS; i++){
            if(n->Mods[i].tag == campo and n->Mods[i].versao <= v and n->Mods[i].versao > v_aux){
                v_aux = n->Mods[i].versao;
                j = i;
            }
        }

        if(j != 0){
            switch(campo){
                case CHAVE: resposta.k = n->Mods[j].valor.k;   break;
                case COR:   resposta.c = n->Mods[j].valor.c;   break;
                default:    resposta.p = n->Mods[j].valor.p;   break;
            }
        }

        return resposta;
    }

    //Rotação ensinada pelo Cormen
    void rotacionar_esq(Noh* x){
        Noh* y = x->dir;
        x->dir = y->esq;

        if(y->esq != &sentinela){
            y->esq->pai = x;
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
    }

    //Rotação a direita
    void rotacionar_dir(Noh* x){
        Noh* y = x->esq;
        x->esq = y->dir;

        if(y->dir != &sentinela){
            y->dir->pai = x;
        }

        else{
            if(x == x->pai->dir){
                x->pai->dir = y;
            }
            else{
                x->pai->dir = y;
            }
        }

        y->dir = x;
        x->pai = y;
    }


    void imprimir_rec(Noh* r, int &v){



        if(ler(r, {ESQ}, v).p  != &sentinela){
            imprimir_rec(ler(r, {ESQ}, v).p, v);
        }

        cout << "Chave: " << ler(r, {CHAVE}, v).k << " Cor: " << ler(r, {COR}, v).k << "\n";

        if(ler(r, {DIR}, v).p != &sentinela){
            imprimir_rec(ler(r, {DIR}, v).p, v);
        }
    }


    void inserir_fixup(Noh* n){
        while (n->pai->cor == 'r'){
            if (n->pai == n->pai->pai->esq){
                Noh* y = n->pai->pai->dir;

                if(y->cor = 'r'){
                    n->pai->cor = 'b';
                    y->cor = 'b';
                    n->pai->pai->cor = 'r';
                    n = n->pai->pai;
                }

                else{
                    if(n == n->pai->dir){
                        n = n->pai;
                        rotacionar_esq(n);
                    }

                    n->pai->cor = 'b';
                    n->pai->pai->cor = 'r';
                    rotacionar_dir(n->pai->pai);
                }
            }

            else{
                Noh* y = n->pai->pai->esq;

                if(y->cor = 'r'){
                    n->pai->cor = 'b';
                    y->cor = 'b';
                    n->pai->pai->cor = 'r';
                    n = n->pai->pai;
                }

                else{
                    if(n == n->pai->esq){
                        n = n->pai;
                        rotacionar_dir(n);
                    }

                    n->pai->cor = 'b';
                    n->pai->pai->cor = 'r';
                    rotacionar_esq(n->pai->pai);
                }             
            }
        }
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
                    rotacionar_esq(x->pai);
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
                        rotacionar_dir(w);
                        w = x->pai->dir;
                    }

                    w->cor = x->pai->cor;
                    x->pai->cor = 'b';
                    w->dir->cor = 'b';
                    rotacionar_esq(x->pai);
                    x = raiz_versao[v];
                }
            }

            else{
                w = x->pai->esq;
                
                if(w->cor == 'r'){
                    w->cor = 'b';
                    x->pai->cor = 'r';
                    rotacionar_dir(x->pai);
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
                        rotacionar_esq(w);
                        w = x->pai->esq;
                    }

                    w->cor = x->pai->cor;
                    x->pai->cor = 'b';
                    w->esq->cor = 'b';
                    rotacionar_dir(x->pai);
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

    void inserir(int k, int& v){
        Noh *n =  new Noh {k, 'r', &sentinela, &sentinela, &sentinela, &sentinela, {{-1, {CHAVE}, {0}},{-1, {CHAVE}, {0}}}};


        Noh *y = &sentinela;
        Noh *x = raiz_versao[v];


        while(x != &sentinela){
            y = x;
            if(n->chave < x->chave){ 
                x = x->esq;
            }
            else{
                x = x->dir;
            }
        }

        n->pai = y;

        if(y == &sentinela){
            sentinela.pai = n;
            raiz_versao[v] = n; 
            cout << v << "\n";
        }

        else{
            if(n->chave < y->chave){
                y->esq = n;
            }
            else{
                y->dir = n;
            }
        }

        inserir_fixup(n);
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

    void imprimir(int& v){
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
    
    ~ArvoreRN(){
        for(int i = 0; i < QTDE_VERSOES; i++)
            Deletar(raiz_versao[i], i);    
    }
};


int main(){

    ArvoreRN T;
    int v = 0;

    T.inserir(1, v);
    T.inserir(2, v);
    T.inserir(3, v);
    T.inserir(4, v);

    T.imprimir(v);

    T.deletar(3, v);

    cout << "\n\n";

    T.imprimir(v);

    cout << v << "\n";
}