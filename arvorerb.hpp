#ifndef ARVORERB_H
#define ARVORERB_H

#include <iostream>
#include <climits>

#define QTDE_MODS 6
#define QTDE_VERSOES 100

using namespace std;

enum Tag{
    CHAVE, COR, PAI, ESQ, DIR, NULO
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
    struct Noh{int versao_criacao; int chave; char cor; Noh* esq; Noh* dir; Noh* pai; Noh* retorno; Noh* prox_versao; Mod Mods[QTDE_MODS];};

    //Criando um nó sentinela, assim como o Cormen
    //O noh sentinela vai representar as folhas nulas
    Noh sentinela;

    
    Valor ler_impressao(Noh* n, Tag campo, int v){
        
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
            if(n->Mods[i].tag    == campo and 
               n->Mods[i].versao <= v     and 
               n->Mods[i].versao >  v_aux){

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

    Valor ler(Noh* n, Tag campo, int v){
        

        //Coloando na ultima versão do no;
        while (n->prox_versao != &sentinela && n->prox_versao->versao_criacao <= v){
            n = n->prox_versao;
        }


        Valor resposta;

        switch(campo){
            case CHAVE: resposta.k = n->chave; break;
            case COR:   resposta.c = n->cor;   break;
            case PAI:   resposta.p = n->pai;   break;
            case ESQ:   resposta.p = n->esq;   break;
            case DIR:   resposta.p = n->dir;   break;
        };


        for(int i = 0; i < QTDE_MODS; i++){
            if(n->Mods[i].tag == campo and n->Mods[i].versao <= v){
                switch(campo){
                    case CHAVE: resposta.k = n->Mods[i].valor.k;   break;
                    case COR:   resposta.c = n->Mods[i].valor.c;   break;
                    default:    resposta.p = n->Mods[i].valor.p;   break;
                }
            }
        }

        return resposta;
    }

    
    //Eu quero que a versão mude apenas quando eu for inserir ou retirar um noh
    //As mudanças de ponteiro de pai e filho e cor não devem alterar a versão
    void set(Noh* n, Tag campo, int& v, Valor valor){
        
        if (n == &sentinela){
            if (campo == PAI) n->pai = valor.p;
            return;
        }

        //Colocando na ultima versão do noh
        while(n->prox_versao != &sentinela){
            n = n->prox_versao;
        }


        int i = 0;

        //Procurando MODS vazio
        while (i < QTDE_MODS && 
                (n->Mods[i].versao != v || n->Mods[i].tag != campo) &&
                n->Mods[i].versao != -1){

            i = i + 1;  
        }

        //Caso ainda tenha espaço no mods
        if(i < QTDE_MODS){
            n->Mods[i].versao = v;
            n->Mods[i].valor = valor;
            n->Mods[i].tag = campo;

            if(campo == PAI && valor.p == &sentinela){
                for(int i = v; i < QTDE_VERSOES; i++){
                    raiz_versao[i] = n;
                }
            }
        }

        //Caso não tenha espaço no mods
        else{
            //Duplicando o no
            Noh* novo = new Noh {v,
                                 ler(n, {CHAVE}, v).k, 
                                 ler(n, {COR}  , v).c, 
                                 ler(n, {ESQ}  , v).p, 
                                 ler(n, {DIR}  , v).p,
                                 ler(n, {PAI}  , v).p,
                                 n->retorno,
                                 &sentinela,

                                 {
                                    {-1, {NULO}, {0}},{-1, {NULO}, {0}},
                                    {-1, {NULO}, {0}},{-1, {NULO}, {0}},
                                    {-1, {NULO}, {0}},{-1, {NULO}, {0}}
                                 
                                 }

                                };

            switch(campo){
                case CHAVE: novo->chave = valor.k; break;
                case PAI:   novo->pai   = valor.p; 
                            if(valor.p == &sentinela){
                                for(int i = v; i < QTDE_VERSOES; i++){
                                    raiz_versao[i] = novo;
                                }
                            }
                            break;
                case ESQ:   novo->esq   = valor.p; break;
                case DIR:   novo->dir   = valor.p; break;
                case COR:   novo->cor   = valor.c; break;
            }

            n->prox_versao = novo;


            Valor val;
            val.p = {novo};


            //Caso de N ser raiz
            if(novo->pai == &sentinela){

                raiz_versao[v] = novo;

                for(int i = v; i < QTDE_VERSOES; i++){
                    raiz_versao[i] = novo;
                }
            }

            //Caso N não seja raiz
            else{
                Noh* pai = novo->pai;


                if (pai != &sentinela){

                    //Caso N seja filho esq
                    if(ler(pai, ESQ, v).p == n){
                        set(pai, ESQ, v, val);
                    }

                    //Caso N seja filho dir
                    else{
                        set(pai, DIR, v, val);
                    }
                }
            }


            Noh* esq = novo->esq;
            Noh* dir = novo->dir;

            //Avisando para os filhos trocarem de pai
            if(esq != &sentinela){
                set(esq, PAI, v, val);
            }

            if(dir != &sentinela){
                set(dir, PAI, v, val);
            }


        }
    }

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
        set(x, PAI, v, val);
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
        set(x, PAI, v, val);
    }

    void imprimir_rec(Noh* r, int v, int i){
        
        if(r != &sentinela){
            if(ler(r, {ESQ}, v).p  != &sentinela){
                imprimir_rec(ler(r, {ESQ}, v).p, v, i+1);
            }

            cout << ler(r, {CHAVE}, v).k
                << "," << i
                << ","  << ler(r, {COR}, v).c << "  ";

            if(ler(r, {DIR}, v).p != &sentinela){
                imprimir_rec(ler(r, {DIR}, v).p, v, i+1);
            }
        }
    }

    
    void inserir_fixup(Noh* n, int& v){

        Valor val;


        while (ler(ler(n,PAI,v).p, COR, v).c == 'r'){
            Noh* pai = ler(n, PAI, v).p;

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
    }

    /*
    void inserir_fixup(Noh *z, int& v){
        Valor val;

        while(ler(ler(z, PAI, v).p, COR, v).c == 'r'){
            if( ler(z, PAI, v).p == ler( ler( ler(z, PAI, v).p, PAI, v).p, ESQ, v).p){
                cout << "o pai é filho esq\n";
            }
            else{
                cout << "o pai é filho dir\n";
            }
            val.c = 'b';
            set(ler(z, PAI, v).p, COR, v, val);
        }
    }*/


    /*
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
    }*/

    void transplantar(Noh* u, Noh* n, int& v){
        Valor val;

        if(ler(u, PAI, v).p == &sentinela){
            for(int i = v; i < QTDE_VERSOES; i++){
                raiz_versao[v] = n;
            }
        }
        else{
            val.p = {n};
            if( u == ler(ler(u, PAI, v).p, ESQ, v).p){
                set(ler(u, PAI, v).p, ESQ, v, val);
            }
            else{
                set(ler(u, PAI, v).p, DIR, v, val);
            }
        }

        val.p = {ler(u, PAI, v).p};
        set(n, PAI, v, val);
    }

    Noh* minimo(Noh* x, int& v){
        while(ler(x, ESQ, v).p != &sentinela){
            x = ler(x, ESQ, v).p;
        }

        return x;
    }


    /*
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
    */


    void delete_fixup(Noh* x, int&v){
        Noh* w;
        Valor val;

        while(x != raiz_versao[v] && ler(x, COR, v).c == 'b'){
            if(x == ler(ler(x, PAI, v).p, ESQ, v).p){
                w = ler(ler(x, PAI, v).p, DIR, v).p;

                if(ler(w, COR, v).c == 'r'){
                    val.c = {'b'};
                    set(w, COR, v, val);

                    val.c = {'r'};
                    set(ler(x, PAI, v).p, COR, v, val);

                    rotacionar_esq(ler(x, PAI, v).p, v);

                    w = ler(ler(x, PAI, v).p, DIR, v).p;
                }

                if(ler(ler(w, ESQ, v).p, COR, v).c == 'b' &&
                   ler(ler(w, DIR, v).p, COR, v).c == 'b'){

                    Noh* aux = x;

                    val.c = 'r';
                    set(w, COR, v, val);

                    x = ler(x, PAI, v).p;
                }

                else{
                    if(ler(ler(w, DIR, v).p, COR, v).c == 'b'){
                        val.c = 'b';
                        set(ler(w, ESQ, v).p, COR, v, val);

                        val.c = 'r';
                        set(w, COR, v, val);

                        rotacionar_dir(w, v);

                        w = ler(ler(x, PAI, v).p, DIR, v).p;
                    }

                    val.c = ler(ler(x, PAI, v).p, COR, v).c;
                    set(w, COR, v, val);

                    val.c = 'b';
                    set(ler(x, PAI, v).p, COR, v, val);

                    set(ler(w, DIR, v).p, COR, v, val);

                    rotacionar_esq(ler(x, PAI, v).p, v);

                    x = raiz_versao[v];
                }
            }

            else{
                w = ler(ler(x, PAI, v).p, ESQ, v).p;

                if(ler(w, COR, v).c == 'r'){
                    val.c = {'b'};
                    set(w, COR, v, val);

                    val.c = {'r'};
                    set(ler(x, PAI, v).p, COR, v, val);

                    rotacionar_dir(ler(x, PAI, v).p, v);

                    w = ler(ler(x, PAI, v).p, ESQ, v).p;
                }

                if(ler(ler(w, ESQ, v).p, COR, v).c == 'b' &&
                   ler(ler(w, DIR, v).p, COR, v).c == 'b'){
                    
                    val.c = 'r';
                    set(w, COR, v, val);

                    x = ler(x, PAI, v).p;
                }
                else{
                    if(ler(ler(w, ESQ, v).p, COR, v).c == 'b'){
                        val.c = 'b';
                        set(ler(w, DIR, v).p, COR, v, val);

                        val.c = 'r';
                        set(w, COR, v, val);

                        rotacionar_esq(w, v);

                        w = ler(ler(x, PAI, v).p, ESQ, v).p;
                    }

                    val.c = ler(ler(x, PAI, v).p, COR, v).c;
                    set(w, COR, v, val);

                    val.c = 'b';
                    set(ler(x, PAI, v).p, COR, v, val);

                    set(ler(w, ESQ, v).p, COR, v, val);

                    rotacionar_dir(ler(x, PAI, v).p, v);

                    x = raiz_versao[v];
                }
            }
        }

        val.c = 'b';
        set(x, COR, v, val);
    }

    Noh* buscar_rec(Noh* r, int k, int &v){
        if(r == &sentinela){
            return &sentinela;
        }

        if(ler(r, CHAVE, v).k == k){
            return r;
        }

        if(ler(r, CHAVE, v).k < k){
            return buscar_rec(ler(r, DIR, v).p, k, v);
        }

        else{
            return buscar_rec(ler(r, ESQ, v).p, k, v);
        }
    }

    public: 

    Noh* raiz_versao[QTDE_VERSOES];
    
    ArvoreRN(){
        sentinela.esq = sentinela.dir = 
        sentinela.pai = sentinela.prox_versao = &sentinela;
        sentinela.cor = 'b';

        for(int i = 0; i < QTDE_VERSOES; i++){
            raiz_versao[i] = &sentinela; 
        }
    }

    void inserir(int k, int& v){

        if(v >= QTDE_VERSOES-1){
            cout << "NÃO HÁ ESPAÇO PARA NOVAS VERSÔES\n";
            return;
        }

        Noh *n =  new Noh {v, k, 'r', &sentinela, &sentinela, 
                                   &sentinela, &sentinela, &sentinela,
                           {
                             {-1, {NULO}, {0}},{-1, {NULO}, {0}},
                             {-1, {NULO}, {0}},{-1, {NULO}, {0}},
                             {-1, {NULO}, {0}},{-1, {NULO}, {0}}
                           }};


        Noh *y = &sentinela;
        Noh *x = raiz_versao[v];

        v = v+1;

        if(v == 1){
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
        
        n->pai = y;
        Valor val;
        val.p = {y};
        
        if(y == &sentinela){
            sentinela.pai = n;
            raiz_versao[v] = n; 
            
            for(int i = v; i < QTDE_VERSOES; i++){
                raiz_versao[i] = n;
            }
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


    /*
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
    }*/

    void deletar(int k, int& v){
        Noh* z = buscar(k, v);

        Valor val;
        
        v = v+1;

        if(v >= QTDE_VERSOES-1){
            cout << "NÃO HÁ ESPAÇO PARA NOVAS VERSÔES\n";
        }

        if( z == &sentinela){
            return;
        }

       Noh* y = z;
       Noh* x;
       char y_original_cor = ler(y, COR, v).c;
    
       if(ler(z, ESQ, v).p == &sentinela){
            x = ler(z, DIR, v).p;
            transplantar(z, ler(z, DIR, v).p, v);
        }

        else{
            if(ler(z, DIR, v).p == &sentinela){
                x = ler(z, ESQ, v).p;
                transplantar(z, ler(z, ESQ, v).p, v);
            }
            else{
                y = minimo(ler(z, DIR, v).p, v);
                y_original_cor = ler(y, COR, v).c;
                x = ler(y, DIR, v).p;

                if( y != ler(z, DIR, v).p){
                    transplantar(y, ler(y, DIR, v).p, v);

                    val.p = ler(z, DIR, v).p;
                    set(y, DIR, v, val);
                    
                    val.p = y;
                    set(ler(y, DIR, v).p, PAI, v, val);
                }

                else{
                    val.p = y;
                    set(x, PAI, v, val);

                    transplantar(z, y, v);

                    val.p = ler(z, ESQ, v).p;
                    set(y, ESQ, v, val);

                    val.p = y;
                    set(ler(y, ESQ, v).p, PAI, v, val);

                    val.c = ler(z, COR, v).c;
                    set(y, COR, v, val);
                }
            }
        }

        if(y_original_cor == 'b'){
            delete_fixup(x, v);
        }
    }

    Noh* buscar(int k, int& v){
        return buscar_rec(raiz_versao[v], k, v);
    }

    void imprimir(int v, int v_atual){

        if(v > QTDE_VERSOES-1){
            cout << "Não há como imprimir essa versão.\n";
            cout << "O limite de versões é " << QTDE_VERSOES-1 << "\n";
        }

        else{
            
            if(v > v_atual) v = v_atual;

            imprimir_rec(raiz_versao[v], v, 0);

            cout << "\n";
        }
    }

    /*
    int sucessor(int k, int v){
        Noh* pos = buscar(k, v);

        if (pos == &sentinela){
            return INT_MAX;
        }

        if(pos->dir != &sentinela){
            return minimo(pos->dir, v)->chave;
        }

        Noh* y = pos->pai;

        while(y != &sentinela && pos == y->dir){
            pos = y;
            y = y->pai;
        }

        return y->chave;
    }*/
    
    int sucessor(int k, int v, int v_atual){

        if (v > v_atual) v = v_atual;

        Noh* pos = buscar(k, v);

        if(pos == &sentinela){
            return INT_MAX;
        }

        if(ler(pos, DIR, v).p != &sentinela){
            return ler(minimo(ler(pos, DIR, v).p, v), CHAVE, v).k;
        }

        Noh* y = ler(pos, PAI, v).p;

        while(y != &sentinela && pos == ler(y, DIR, v).p){
            pos = y;
            y = ler(y, PAI, v).p;
        }

        return ler(y, CHAVE, v).k;
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

};

#endif