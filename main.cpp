#include <iostream>
#include <climits>

using namespace std;


class ArvoreRN{

    template <typename T>
    struct Noh;

    //Estrutura do Mod
    template <typename T>
    struct Mod {int versao; char campo; T valor;};
    
    
    //Estrutura do noh
    //Os campos de retorno e formado por apenas um no, ja que em uma arvore, cada no recebe em apontado por no max 1 ponteiro
    //Portanto, o campo de modificações terá apenas 2 espaços
    template <typename T>
    struct Noh{int chave; char cor; Noh* esq; Noh* dir; Noh* pai; Noh* retorno; Mod<T> Mods[2];};

    //Criando um nó sentinela, assim como o Cormen
    //O noh sentinela vai representar as folhas nulas e será o pai da raiz
    Noh<int> sentinela;


    //Rotação ensinada pelo Cormen
    template <typename T>
    void rotacionar_esq(Noh* x){
        Noh<T>* y = x->dir;
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


    void imprimir_rec(Noh* r){

        if(r->esq != &sentinela){
            imprimir_rec(r->esq);
        }

        cout << "Chave: " << r->chave << " Cor: " << r->cor << "\n";

        if(r->dir != &sentinela){
            imprimir_rec(r->dir);
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

    void delete_fixup(Noh* x){
        Noh* w;

        while(x != raiz() && x->cor == 'b'){
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
                    x = raiz();
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
                    x = raiz();
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

    ArvoreRN(){
        sentinela.esq = sentinela.dir = sentinela.pai = &sentinela;
        sentinela.cor = 'b';
    }

    

    Noh* raiz(){
        if(sentinela.pai != nullptr) return sentinela.pai;

        return &sentinela;
    }


    void inserir(int k){
        Mod m = {-1,'n',0,&sentinela, 'n'};
        Noh *n =  new Noh {k, 'r', &sentinela, &sentinela, &sentinela, &sentinela, {m,m}};


        Noh *y = &sentinela;
        Noh *x = raiz();

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


    void deletar(int k){
        Noh* z = buscar(k);

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
            delete_fixup(x);
        }
    }

    Noh* buscar(int k){
        return buscar_rec(raiz(), k);
    }

    void imprimir(){
        imprimir_rec(raiz());
    }

    int sucessor(int k){
        Noh* pos = buscar(k);

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

    template <typename T>
    T ler(Noh* n, char campo, int v){
        
        T resposta = n->chave;
        int j = 0;
        int v_aux = 0;
        

        for (int i = 0; i < 2; i++){
            if(n->Mods[i].campo == campo && n->Mods[i].versao <= v && n->Mods[i].versao > v_aux){
                j = i;
                v_aux = n->Mods[i].versao;
            }
        }


        return resposta;
    }
    

    //Destrutor da árvore
    void Deletar(Noh* r){
        if(r != &sentinela){
            Deletar(r->esq);
            Deletar(r->dir);
            deletar(r->chave);
        }
    }
    
    ~ArvoreRN(){
        Deletar(raiz());    
    }
};


int main(){

    ArvoreRN T;

    T.inserir(1);
    T.inserir(2);
    T.inserir(3);
    T.inserir(4);

    T.imprimir();

    T.deletar(3);

    cout << "\n\n";

    T.imprimir();
    cout << T.ler<int>(T.raiz(), 'k', 0) << "\n";
}