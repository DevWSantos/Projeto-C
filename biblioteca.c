#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*CONSTANTES*/
#define MAX_TITULO 100
#define MAX_AUTOR 80
#define MAX_ISBN 20
#define MAX_NOME 60

/* ESTRUTURA BASE: Livro */
typedef struct {
    char isbn[MAX_ISBN];
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int ano;
    int disponivel;
} Livro;

/*LISTA ENCADEADA*/
typedef struct NoLista {
    Livro livro;
    struct NoLista *proximo;
} NoLista;

typedef struct {
    NoLista *inicio; 
    int tamanho;
} ListaAcervo;

/* Inicializa lista */
void lista_inicializar(ListaAcervo *lista) {
    lista->inicio = NULL;
    lista->tamanho = 0;
}

int lista_inserir(ListaAcervo *lista, Livro livro) {
    NoLista *novo = (NoLista *)malloc(sizeof(NoLista));
    if (!novo) {
        printf("[ERRO] Memória insuficiente.\n");
        return 0;
    }
    novo->livro = livro;
    novo->proximo = lista->inicio;
    lista->inicio = novo;
    lista->tamanho++;
    return 1;
}

NoLista *lista_buscar_isbn(ListaAcervo *lista, const char *isbn) {
    NoLista *atual = lista->inicio;
    while (atual) {
        if (strcmp(atual->livro.isbn, isbn) == 0)
            return atual;
        atual = atual->proximo;
    }
    return NULL;
}

int lista_remover(ListaAcervo *lista, const char *isbn) {
    NoLista *atual = lista->inicio;
    NoLista *anterior = NULL;
    while (atual) {
        if (strcmp(atual->livro.isbn, isbn) == 0) {
            if (anterior)
                anterior->proximo = atual->proximo;
            else
                lista->inicio = atual->proximo;
            free(atual);
            lista->tamanho--;
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

void lista_para_array(ListaAcervo *lista, Livro *arr) {
    NoLista *atual = lista->inicio;
    int i = 0;
    while (atual) {
        arr[i++] = atual->livro;
        atual = atual->proximo;
    }
}

void lista_exibir(ListaAcervo *lista) {
    if (!lista->inicio) {
        printf("(Acervo vazio)\n");
        return;
    }
    NoLista *atual = lista->inicio;
    int i = 1;
    while (atual) {
        printf("%d.[%s]\"%s\"—%s(%d)|%s\n",
               i++,
               atual->livro.isbn,
               atual->livro.titulo,
               atual->livro.autor,
               atual->livro.ano,
               atual->livro.disponivel ? "DISPONÍVEL" : "EMPRESTADO");
        atual = atual->proximo;
    }
}

void lista_destruir(ListaAcervo *lista) {
    NoLista *atual = lista->inicio;
    while (atual) {
        NoLista *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
}
/*  ESTRUTURA 2: FILA → Espera por Empréstimo*/
typedef struct NoFila {
    char nome_usuario[MAX_NOME];
    char isbn_livro[MAX_ISBN];
    struct NoFila *proximo;
} NoFila;

typedef struct {
    NoFila *frente;
    NoFila *fundo;
    int tamanho;
} FilaEspera;

void fila_inicializar(FilaEspera *fila) {
    fila->frente = NULL;
    fila->fundo = NULL;
    fila->tamanho = 0;
}

int fila_enfileirar(FilaEspera *fila, const char *usuario, const char *isbn) {
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    if (!novo) {
        printf("[ERRO] Memória insuficiente.\n");
        return 0;
    }
    strncpy(novo->nome_usuario, usuario, MAX_NOME -1);
    strncpy(novo->isbn_livro, isbn, MAX_ISBN -1);
    novo->proximo = NULL;

    if (!fila->fundo) {
        fila->frente = novo;
        fila->fundo = novo;
    } else {
        fila->fundo->proximo = novo;
        fila->fundo = novo;
    }
    fila->tamanho++;
    return 1;
}

int fila_desenfileirar(FilaEspera *fila, char *usuario_out, char *isbn_out) {
    if (!fila->frente) return 0;

    NoFila *removido = fila->frente;
    strncpy(usuario_out, removido->nome_usuario, MAX_NOME -1);
    strncpy(isbn_out, removido->isbn_livro, MAX_ISBN -1);

    fila->frente = removido->proximo;
    if (!fila->frente) fila->fundo = NULL;

    free(removido);
    fila->tamanho--;
    return 1;
}

void fila_exibir(FilaEspera *fila) {
    if (!fila->frente) {
        printf("(Fila vazia)\n");
        return;
    }
    NoFila *atual = fila->frente;
    int i = 1;
    while (atual) {
        printf("%d. Usuário: %-30s | ISBN: %s\n",
               i++, atual->nome_usuario, atual->isbn_livro);
        atual = atual->proximo;
    }
}

void fila_destruir(FilaEspera *fila) {
    char u[MAX_NOME], b[MAX_ISBN];
    while (fila_desenfileirar(fila, u, b));
}
/* PILHA*/

typedef struct NoPilha {
    char nome_usuario[MAX_NOME];
    char titulo_livro[MAX_TITULO];
    char isbn[MAX_ISBN];
    struct NoPilha *abaixo;
} NoPilha;

typedef struct {
    NoPilha *topo;
    int tamanho;
} PilhaHistorico;

void pilha_inicializar(PilhaHistorico *pilha) {
    pilha->topo = NULL;
    pilha->tamanho = 0;
}

int pilha_empilhar(PilhaHistorico *pilha, const char *usuario,
                   const char *titulo, const char *isbn) {
    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (!novo) {
        printf("[ERRO] Memória insuficiente.\n");
        return 0;
    }
    strncpy(novo->nome_usuario, usuario, MAX_NOME -1);
    strncpy(novo->titulo_livro, titulo, MAX_TITULO -1);
    strncpy(novo->isbn, isbn, MAX_ISBN -1);
    novo->abaixo = pilha->topo;
    pilha->topo = novo;
    pilha->tamanho++;
    return 1;
}

int pilha_desempilhar(PilhaHistorico *pilha,
                      char *usuario_out, char *titulo_out, char *isbn_out) {
    if (!pilha->topo) return 0;
    NoPilha *removido = pilha->topo;
    strncpy(usuario_out, removido->nome_usuario, MAX_NOME -1);
    strncpy(titulo_out, removido->titulo_livro, MAX_TITULO -1);
    strncpy(isbn_out, removido->isbn, MAX_ISBN -1);
    pilha->topo = removido->abaixo;
    free(removido);
    pilha->tamanho--;
    return 1;
}

void pilha_exibir(PilhaHistorico *pilha) {
    if (!pilha->topo) {
        printf("  (Histórico vazio)\n");
        return;
    }
    NoPilha *atual = pilha->topo;
    int i = 1;
    while (atual) {
        printf("  %d. %-30s devolveu \"%s\"\n",
               i++, atual->nome_usuario, atual->titulo_livro);
        atual = atual->abaixo;
    }
}

/* Libera pilha*/
void pilha_destruir(PilhaHistorico *pilha) {
    char u[MAX_NOME], t[MAX_TITULO], b[MAX_ISBN];
    while (pilha_desempilhar(pilha, u, t, b));
}

/*ÁRVORE BINÁRIA*/

typedef struct NoArvore {
    Livro           livro;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

/* Insere na árvore (ordenado por título) */
NoArvore *arvore_inserir(NoArvore *raiz, Livro livro) {
    if (!raiz) {
        NoArvore *novo = (NoArvore *)malloc(sizeof(NoArvore));
        if (!novo) return NULL;
        novo->livro    = livro;
        novo->esquerda = NULL;
        novo->direita  = NULL;
        return novo;
    }
    int cmp = strcmp(livro.titulo, raiz->livro.titulo);
    if (cmp < 0)
        raiz->esquerda = arvore_inserir(raiz->esquerda, livro);
    else if (cmp > 0)
        raiz->direita  = arvore_inserir(raiz->direita,  livro);
    /* Duplicatas ignoradas */
    return raiz;
}

/* Busca por título (parcial, case-sensitive) */
void arvore_buscar_titulo(NoArvore *raiz, const char *termo, int *encontrados) {
    if (!raiz) return;
    if (strstr(raiz->livro.titulo, termo)) {
        printf("  → [%s] \"%s\" — %s (%d) | %s\n",
               raiz->livro.isbn,
               raiz->livro.titulo,
               raiz->livro.autor,
               raiz->livro.ano,
               raiz->livro.disponivel ? "DISPONÍVEL" : "EMPRESTADO");
        (*encontrados)++;
    }
    /* Percorre ambos os lados (busca parcial não é binária) */
    arvore_buscar_titulo(raiz->esquerda, termo, encontrados);
    arvore_buscar_titulo(raiz->direita,  termo, encontrados);
}

/* Exibe árvore em ordem (in-order = títulos em ordem alfabética) */
void arvore_em_ordem(NoArvore *raiz) {
    if (!raiz) return;
    arvore_em_ordem(raiz->esquerda);
    printf("  [%s] \"%s\" — %s\n",
           raiz->livro.isbn, raiz->livro.titulo, raiz->livro.autor);
    arvore_em_ordem(raiz->direita);
}

/* Libera árvore */
void arvore_destruir(NoArvore *raiz) {
    if (!raiz) return;
    arvore_destruir(raiz->esquerda);
    arvore_destruir(raiz->direita);
    free(raiz);
}

/* BUBBLE SORT */
void bubble_sort_livros(Livro *arr, int n) {
    int trocou;
    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(arr[j].titulo, arr[j + 1].titulo) > 0) {
                Livro temp  = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                trocou = 1;
            }
        }
        if (!trocou) break; 
    }
}
void exibir_acervo_ordenado(ListaAcervo *lista) {
    if (lista->tamanho == 0) {
        printf(" (Acervo vazio)\n");
        return;
    }
    Livro *arr = (Livro *)malloc(lista->tamanho * sizeof(Livro));
    if (!arr) { printf("[ERRO] Memória insuficiente.\n"); return; }

    lista_para_array(lista, arr);
    bubble_sort_livros(arr, lista->tamanho);

    printf("%-5s %-20s %-40s %-20s %-6s %-12s\n",
           "Nº", "ISBN", "Título", "Autor", "Ano", "Status");
    printf("%s\n", "───");
    for (int i = 0; i < lista->tamanho; i++) {
        printf("%-5d %-20s %-40s %-20s %-6d %-12s\n",
               i + 1,
               arr[i].isbn,
               arr[i].titulo,
               arr[i].autor,
               arr[i].ano,
               arr[i].disponivel ? "DISPONÍVEL" : "EMPRESTADO");
    }
    free(arr);
}

Livro criar_livro(const char *isbn, const char *titulo,
                  const char *autor, int ano) {
    Livro l;
    strncpy(l.isbn, isbn, MAX_ISBN  - 1);
    strncpy(l.titulo, titulo, MAX_TITULO - 1);
    strncpy(l.autor, autor, MAX_AUTOR - 1);
    l.isbn[MAX_ISBN - 1] = '\0';
    l.titulo[MAX_TITULO - 1] = '\0';
    l.autor[MAX_AUTOR - 1] = '\0';
    l.ano = ano;
    l.disponivel = 1;
    return l;
}

void linha_divisoria(char c, int n) {
    for (int i = 0; i < n; i++) putchar(c);
    putchar('\n');
}

void cabecalho(const char *titulo) {
    putchar('\n');
    linha_divisoria('=', 60);
    printf("%s\n", titulo);
    linha_divisoria('=', 60);
}

void pausar(void) {
    printf("\n [Pressione ENTER para continuar...]");
    while (getchar() != '\n');
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
void realizar_emprestimo(ListaAcervo *lista, FilaEspera *fila,
                         const char *isbn, const char *usuario) {
    NoLista *no = lista_buscar_isbn(lista, isbn);
    if (!no) {
        printf("\n Livro com ISBN '%s' não encontrado.\n", isbn);
        return;
    }
    if (no->livro.disponivel) {
        no->livro.disponivel = 0;
        printf("\n  \"%s\" emprestado para %s.\n",
               no->livro.titulo, usuario);
    } else {
        fila_enfileirar(fila, usuario, isbn);
        printf("\n  Livro indisponível. %s entrou na fila de espera.\n", usuario);
    }
}
void realizar_devolucao(ListaAcervo *lista, PilhaHistorico *pilha,
                        FilaEspera *fila,
                        const char *isbn, const char *usuario) {
    NoLista *no = lista_buscar_isbn(lista, isbn);
    if (!no) {
        printf("\n Livro com ISBN '%s' não encontrado.\n", isbn);
        return;
    }
    if (no->livro.disponivel) {
        printf("\n Livro já está disponível (não foi emprestado).\n");
        return;
    }
    no->livro.disponivel = 1;
    pilha_empilhar(pilha, usuario, no->livro.titulo, isbn);
    printf("\n [OK] \"%s\" devolvido por %s. Histórico registrado.\n",
           no->livro.titulo, usuario);

    if (fila->frente && strcmp(fila->frente->isbn_livro, isbn) == 0) {
        char proximo_usuario[MAX_NOME];
        char isbn_fila[MAX_ISBN];
        fila_desenfileirar(fila, proximo_usuario, isbn_fila);
        no->livro.disponivel = 0;
        printf("Livro automaticamente emprestado para o próximo da fila: %s\n",
               proximo_usuario);
        pilha_empilhar(pilha, proximo_usuario, no->livro.titulo, isbn);
    }
}

/*  MENU PRINCIPAL  */
void menu(void) {
    printf("\n");
    linha_divisoria('-', 50);
    printf("  BIBLIOTECA CENTRAL — MENU PRINCIPAL\n");
    linha_divisoria('-', 50);
    printf("[1] Exibir acervo completo\n");
    printf("[2] Exibir acervo ORDENADO (Bubble Sort)\n");
    printf("[3] Buscar livro por título (Árvore Binária)\n");
    printf("[4] Buscar livro por ISBN   (Lista Encadeada)\n");
    printf("[5] Realizar empréstimo\n");
    printf("[6] Realizar devolução\n");
    printf("[7] Exibir fila de espera\n");
    printf("[8] Exibir histórico de devoluções (Pilha)\n");
    printf("[9] Adicionar livro ao acervo\n");
    printf("[0] Sair\n");
    linha_divisoria('-', 50);
    printf(" Opção: ");
}
void carregar_dados(ListaAcervo *lista, NoArvore **arvore) {
    Livro livros[] = {
        criar_livro("978-85-01", "O Senhor dos Aneis", "J.R.R. Tolkien", 1954),
        criar_livro("978-85-02", "Dom Casmurro", "Machado de Assis", 1899),
        criar_livro("978-85-03", "1984", "George Orwell", 1949),
        criar_livro("978-85-04", "Cem Anos de Solidao", "Gabriel Garcia Marquez", 1967),
        criar_livro("978-85-05", "A Metamorfose", "Franz Kafka", 1915),
        criar_livro("978-85-06", "Memorias Postumas", "Machado de Assis", 1881),
        criar_livro("978-85-07", "Coracao das Trevas", "Joseph Conrad", 1899),
        criar_livro("978-85-08", "Admiravel Mundo Novo", "Aldous Huxley", 1932),
        criar_livro("978-85-09", "Grande Sertao: Veredas", "Joao Guimaraes Rosa", 1956),
        criar_livro("978-85-10", "O Processo", "Franz Kafka", 1925),
    };

    int n = sizeof(livros) / sizeof(livros[0]);
    for (int i = 0; i < n; i++) {
        lista_inserir(lista, livros[i]);
        *arvore = arvore_inserir(*arvore, livros[i]);
    }

    NoLista *no;
    no = lista_buscar_isbn(lista, "978-85-02");
    if (no) no->livro.disponivel = 0;
    no = lista_buscar_isbn(lista, "978-85-07");
    if (no) no->livro.disponivel = 0;
}
int main(void) {
    ListaAcervo acervo;
    FilaEspera fila;
    PilhaHistorico historico;
    NoArvore *arvore = NULL;

    lista_inicializar(&acervo);
    fila_inicializar(&fila);
    pilha_inicializar(&historico);

    carregar_dados(&acervo, &arvore);
    cabecalho("BEM-VINDO AO SISTEMA DE BIBLIOTECA");
    printf("  %d livros carregados no acervo.\n", acervo.tamanho);

    int opcao;
    char isbn[MAX_ISBN], usuario[MAX_NOME], termo[MAX_TITULO];
    char titulo[MAX_TITULO], autor[MAX_AUTOR];
    int  ano;

    do {
        menu();
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); continue; }
        limpar_buffer();
        switch (opcao) {

        case 1:
            cabecalho("ACERVO COMPLETO");
            lista_exibir(&acervo);
            pausar();
            break;

        case 2:
            cabecalho("ACERVO ORDENADO POR TÍTULO — Bubble Sort");
            exibir_acervo_ordenado(&acervo);
            pausar();
            break;

        case 3:
            cabecalho("BUSCA POR TÍTULO — Árvore Binária");
            printf("Termo de busca: ");
            fgets(termo, MAX_TITULO, stdin);
            termo[strcspn(termo, "\n")] = '\0';
            {
                int encontrados = 0;
                arvore_buscar_titulo(arvore, termo, &encontrados);
                if (!encontrados)
                    printf(" Nenhum livro encontrado com '%s'.\n", termo);
                else
                    printf("\n %d livro(s) encontrado(s).\n", encontrados);
            }
            pausar();
            break;

        case 4:
            cabecalho("BUSCA POR ISBN — Lista Encadeada");
            printf("ISBN:");
            fgets(isbn, MAX_ISBN, stdin);
            isbn[strcspn(isbn, "\n")] = '\0';
            {
                NoLista *no = lista_buscar_isbn(&acervo, isbn);
                if (no) {
                    printf("\n Livro encontrado:\n");
                    printf("Título: %s\n", no->livro.titulo);
                    printf("Autor: %s\n", no->livro.autor);
                    printf("Ano: %d\n", no->livro.ano);
                    printf("Status: %s\n",
                           no->livro.disponivel ? "DISPONÍVEL" : "EMPRESTADO");
                } else {
                    printf("\n Livro não encontrado.\n");
                }
            }
            pausar();
            break;

        case 5:
            cabecalho("REALIZAR EMPRÉSTIMO");
            printf("ISBN do livro: ");
            fgets(isbn, MAX_ISBN, stdin);
            isbn[strcspn(isbn, "\n")] = '\0';
            printf("Nome do usuário: ");
            fgets(usuario, MAX_NOME, stdin);
            usuario[strcspn(usuario, "\n")] = '\0';
            realizar_emprestimo(&acervo, &fila, isbn, usuario);
            pausar();
            break;

        case 6:
            cabecalho("REALIZAR DEVOLUÇÃO");
            printf("ISBN do livro: ");
            fgets(isbn, MAX_ISBN, stdin);
            isbn[strcspn(isbn, "\n")] = '\0';
            printf("Nome do usuário: ");
            fgets(usuario, MAX_NOME, stdin);
            usuario[strcspn(usuario, "\n")] = '\0';
            realizar_devolucao(&acervo, &historico, &fila, isbn, usuario);
            pausar();
            break;

        case 7:
            cabecalho("FILA DE ESPERA — Estrutura FILA (FIFO)");
            printf("Posições na fila: %d\n\n", fila.tamanho);
            fila_exibir(&fila);
            pausar();
            break;

        case 8:
            cabecalho("HISTÓRICO DE DEVOLUÇÕES — Estrutura PILHA (LIFO)");
            printf("Registros: %d\n\n", historico.tamanho);
            pilha_exibir(&historico);
            pausar();
            break;

        case 9:
            cabecalho("ADICIONAR LIVRO AO ACERVO");
            printf("ISBN:"); fgets(isbn,   MAX_ISBN,   stdin); isbn[strcspn(isbn,"\n")] = '\0';
            printf("Título:"); fgets(titulo,  MAX_TITULO, stdin); titulo[strcspn(titulo,"\n")] = '\0';
            printf("Autor:"); fgets(autor,   MAX_AUTOR,  stdin); autor[strcspn(autor,"\n")] = '\0';
            printf("Ano:");
            if (scanf("%d", &ano) != 1) ano = 0;
            limpar_buffer();
            {
                if (lista_buscar_isbn(&acervo, isbn)) {
                    printf("\n  [!] ISBN já cadastrado.\n");
                } else {
                    Livro novo = criar_livro(isbn, titulo, autor, ano);
                    lista_inserir(&acervo, novo);
                    arvore = arvore_inserir(arvore, novo);
                    printf("\n Livro \"%s\" adicionado com sucesso!\n", titulo);
                }
            }
            pausar();
            break;

        case 0:
            printf("\n  Encerrando sistema. Até logo!\n\n");
            break;

        default:
            printf("\n Opção inválida.\n");
        }

    } while (opcao != 0); 

    lista_destruir(&acervo);
    fila_destruir(&fila);
    pilha_destruir(&historico);
    arvore_destruir(arvore);

    return 0;
}
