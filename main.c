#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

char *getTermo(FILE *fp){
    char termo[100];
    int ch, i = 0;

    while((ch = fgetc(fp))!= EOF && !isalpha(ch));
    if(ch == EOF)
        return NULL;
    do{
        termo[i++] = tolower(ch);
    }while((ch = fgetc(fp))!= EOF && isalpha(ch));
    termo[i]='\0';

    return strdup(termo);
}

typedef struct arvore {
    char* info;
    int qtd;
    struct arvore *esq;
    struct arvore *dir;
} Arvore;

Arvore* inserir (Arvore * a, char * c) {
    if(a == NULL){
        a = (Arvore*)malloc(sizeof(Arvore));
        a->info = c;
        a->qtd = 1;
        a->esq = a->dir = NULL;
    }else if(strcmp(c, a->info)< 0)
        a->esq = inserir(a->esq, c);
    else if(strcmp(c, a->info)> 0)
        a->dir = inserir(a->dir, c);
    else if(strcmp(c, a->info) == 0)
        a->qtd++;
    return a;
}

void imprime(Arvore *a){
    if(!a)
        return;
    else
        imprime(a->esq);
    printf("\t%s: %d\n", a->info, a->qtd);
    imprime(a->dir);
}

void ordena(Arvore *a, int N, int arr[], char* str[]){
    if(!a)
        return;
    else
        ordena(a->esq, N, arr, str);
    if(a->qtd > arr[N-1]){

        int x = 0, y;
        char* c;

        while(arr[x] >= a->qtd)
            x++;

        for (int i = x; i < N; i++){
            for (int j = i + 1; j < N; j++){
                if(arr[i] < arr[j]){
                    y = arr[i];
                    c = str[i];

                    arr[i] = arr[j];
                    str[i] = str[j];

                    arr[j] = y;
                    str[j] = c;
                }
            }
        }

        arr[x] = a->qtd;
        str[x] = a->info;
    }
    ordena(a->dir, N, arr, str);
}

int main(int argc, char *argv[]){
    int cont = 0, N;
    //char* arq = "C:/Users/Pietro Giacomitti/Documents/Small_texts/128mb.txt";

    Arvore *a;
    a = inserir(NULL, "A");

    if(strcmp(argv[1], "--freq") == 0 && sscanf(argv[2], "%d", &N) == 1 && argv[3] != NULL){
        FILE *fp = fopen(argv[3], "r+");
        int termos_qtd[N];
        char *termos_str[N+1];
        if(fp == NULL) printf("\n\t**Arquivo nao encontrado: %s**\n", argv[3]);
        else{
            char *termo;
            printf("\n\tLendo arquivo \"%s\"...\n", argv[3]);
            while(termo = getTermo(fp)){
                if(strlen(termo)>2)
                    a = inserir(a, termo);
                    cont++;
            }
        }
        fclose(fp);
        //imprime(a);

        for(int i = 0; i < N; i++)
            termos_qtd[i] = 0;

        ordena(a, N, termos_qtd, termos_str);

        for(int i = 0; i < N; i++){
            if(termos_qtd[i] > 0)
                printf("\n\t#%d - %s: %d", i+1, termos_str[i], termos_qtd[i]);
        }
        printf("\n");

    }else if(strcmp(argv[1], "--freq-word") == 0 && argv[2] != NULL && argv[3] != NULL){
        FILE *fp = fopen(argv[3], "r+");
        if(fp == NULL) printf("\n\t**Arquivo nao encontrado: %s**\n", argv[3]);
        else{
            char* termo;
            printf("\n\tLendo arquivo \"%s\"...\n", argv[3]);
            while(termo = getTermo(fp)){
                if(strcmp(termo, argv[2]) == 0)
                    cont++;
            }
            printf("\n\tNumero de ocorrencias de \"%s\": %d\n", argv[2], cont);
        }
        fclose(fp);
    }else if(strcmp(argv[1], "--search") == 0 && argv[2] != NULL && argv[3] != NULL){
        int n_doc = argc - 3, num_pres = 0;
        double tf[n_doc];
        char* doc_name[n_doc];
        for(int i = 0; i < n_doc; i++){
            FILE *fp = fopen(argv[i+3], "r+");
            if(fp == NULL) printf("\n\t**Arquivo nao encontrado: %s**\n", argv[i+3]);
            else{
                char* termo;
                printf("\n\tLendo arquivo \"%s\"...\n", argv[i+3]);
                int cont1 = 0;
                int cont2 = 0;

                while(termo = getTermo(fp)){
                    if(strcmp(termo, argv[2]) == 0)
                        cont1++;
                    cont2++;
                }
                if(cont1 > 0){
                    num_pres++;
                    tf[i] = ((float)cont1/cont2);
                    doc_name[i] = argv[i+3];
                }
            }
            fclose(fp);
        }
        if(num_pres == 0){
            printf("\n\tNenhum arquivo possui o termo \"%s\".\n", argv[2]);
            exit(1);
        }else if(num_pres == n_doc){
            printf("\n\tTodos os arquivos sao relevantes ao termo \"%s\".\n", argv[2]);
            exit(1);
        }
        else{
            float idf = log10((float)n_doc/num_pres), a = 0.0;

            //if(((float)n_doc/num_pres) > 1.f){
                for(int i = 0; i < n_doc; i++){
                    tf[i] *= idf;
                }
            //}
            char* str;
            for(int i = 0; i < n_doc; i++){
                for (int j = i + 1; j < n_doc; j++){
                    if(tf[i] < tf[j]){
                        a = tf[i];
                        str = doc_name[i];

                        tf[i] = tf[j];
                        doc_name[i] = doc_name[j];

                        tf[j] = a;
                        doc_name[j] = str;
                    }
                }
            }
            printf("\n\tRelevancia de documentos para \"%s\":\n", argv[2]);
            for(int i = 0; i < n_doc; i++){
                printf("\t%d - %s\n", i+1, doc_name[i]);
            }
        }
    }else{
        printf("\n\t**Parametros invalidos passados em: ");
        for(int i = 0; i < argc; i++)
            printf("%s ", argv[i]);
        printf("**\n");
    }
}
