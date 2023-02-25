#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE (20)

struct Registro {
    char *string;
    long long int tamanho;
};

int lerArquivo(FILE *arquivoIn, struct Registro *registro) {
    int lidos;
    lidos = fread(&registro->tamanho, sizeof(long long int), 1, arquivoIn);
    if(lidos != 0) {
        registro->string = (char*)malloc((registro->tamanho+1) * sizeof(char));
        fread(registro->string, sizeof(char), registro->tamanho, arquivoIn);
        registro->string[registro->tamanho] = '\0';
    }
    return lidos;
}

void escreveRegistro (struct Registro *registro, FILE *arquivoOut) {
    printf("%s\n", registro->string);
    fwrite(&registro->tamanho, sizeof(long long int), 1, arquivoOut); 
    fwrite(registro->string, sizeof(char), registro->tamanho, arquivoOut);
    free(registro->string);
}

void merge(FILE *arquivoIn1, FILE *arquivoIn2, FILE *arquivoOut) {
    struct Registro registro1, registro2;
    int lidosArq1, lidosArq2, comparacao;

    lidosArq1 = lerArquivo(arquivoIn1, &registro1);
    lidosArq2 = lerArquivo(arquivoIn2, &registro2);

    while (lidosArq1 != 0 || lidosArq2 != 0){

        if(lidosArq2 == 0) {
            escreveRegistro(&registro1,arquivoOut);
            lidosArq1 = lerArquivo(arquivoIn1, &registro1);
        } else if(lidosArq1 == 0) {
            escreveRegistro(&registro2,arquivoOut);
            lidosArq2 = lerArquivo(arquivoIn2, &registro2);
        } else {
            comparacao = strcmp(registro1.string,registro2.string);

            if(comparacao == 0) {
                escreveRegistro(&registro1,arquivoOut);
                lidosArq1 = lerArquivo(arquivoIn1, &registro1);
            } else if(comparacao < 0) {
                escreveRegistro(&registro1,arquivoOut);
                lidosArq1 = lerArquivo(arquivoIn1, &registro1);
            } else {
                escreveRegistro(&registro2,arquivoOut);
                lidosArq2 = lerArquivo(arquivoIn2, &registro2);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *arquivoIn1, *arquivoIn2, *arquivoOut;
    char nomeArquivoIn1[SIZE], nomeArquivoIn2[SIZE], nomeArquivoOut[SIZE];

    scanf("%s", nomeArquivoIn1);
    scanf("%s", nomeArquivoIn2);
    strcpy(nomeArquivoOut, nomeArquivoIn1);
    strcat(nomeArquivoOut, ".merge");


    arquivoIn1 = fopen(nomeArquivoIn1,"r");
    if(arquivoIn1 == NULL) {
        printf("ERRO: ao abrir arquivoIn1!");
    }
    arquivoIn2 = fopen(nomeArquivoIn2,"r");
    if(arquivoIn2 == NULL) {
        printf("ERRO: ao abrir arquivoIn2!");
    }
    arquivoOut = fopen(nomeArquivoOut,"w");
    if(arquivoOut == NULL) {
        printf("ERRO: ao abrir arquivoOut!");
    }

    merge(arquivoIn1, arquivoIn2, arquivoOut);

    fclose(arquivoIn1);
    fclose(arquivoIn2);
    fclose(arquivoOut); 

    return 0;
}
