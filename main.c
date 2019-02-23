#include <stdio.h>
#include <stdlib.h>
#define TAM 3

/**
* Struct do buffer, armazena o numero e a fita de onde o numero veio
*/
typedef struct bf{
    int elem;       // N�mero a ser ordenado
    int fita;       // Fita de onde o n�mero veio
}Buffer;

void insertionSort(int arr[], int n);       // Fun��o de ordena��o de um vetor
void insertionSortBuf(Buffer arr[], int n); // Fun��o de ordena��o de um vetor da struct Buffer
int divideArquivos(FILE *entrada, FILE *arquivo1, FILE *arquivo2, FILE *arquivo3);  // Faz a primeira divis�o
void gravarBuffer(int bf[], int tamanho, FILE *arquivo);    // Grava os elementos do buffer em um arquivo
void ordenarBloco(FILE *entrada1, FILE *entrada2, FILE *entrada3, FILE *saida); // Junta, ordena e grava o bloco dos arquivos de entrada para o arquivo de sa�da
int numeroDePassadas(float registros, int buffer);     // Calcula quantas passadas ser�o necess�rias fazer baseado em quantos registros h� no arquivo de entrada
void passadaArquivo(FILE *entrada1, FILE *entrada2, FILE *entrada3, FILE *saida1, FILE *saida2, FILE *saida3);
void converterArquivoSaida(FILE *entrada, FILE *saida, int quantidade); // Converte um arquivo com registros separados por \t para \n

int main(int argc, char *argv[ ]){

    int quant, passadas, cont = 1;
    FILE *entrada, *f1, *f2, *f3, *f4, *f5, *f6, *saida;

    entrada = fopen("entrada.txt", "r");  // Abre o arquivo de entrada em modo leitura
    f1 = fopen("f1.txt", "w");      // Abre a fita1 em modo grava��o
    f2 = fopen("f2.txt", "w");      // Abre a fita2 em modo grava��o
    f3 = fopen("f3.txt", "w");      // Abre a fita3 em modo grava��o

    quant = divideArquivos(entrada, f1, f2, f3);    // Faz a primeira divis�o do arquivo e pega o n�mero de registros dele
    fclose(entrada); fclose(f1); fclose(f2); fclose(f3);    // Fecha os arquivos

    passadas = numeroDePassadas(quant, TAM);  // Calcula o n�mero de vezes que ser� necess�rio ordenar ap�s a primeira divis�o

    while(cont <= passadas){   // Faz a passada de ordena��o quantas vezes for necess�rio, de acordo com o calculo acima
        if(cont % 2 != 0){          // Alterna os arquivos de entrada e de sa�da
            f1 = fopen("f1.txt", "r");  // Abre a fita1 em modo leitura
            f2 = fopen("f2.txt", "r");  // Abre a fita2 em modo leitura
            f3 = fopen("f3.txt", "r");  // Abre a fita3 em modo leitura
            f4 = fopen("f4.txt", "w");  // Abre a fita1 em modo grava��o
            f5 = fopen("f5.txt", "w");  // Abre a fita2 em modo grava��o
            f6 = fopen("f6.txt", "w");  // Abre a fita3 em modo grava��o
            passadaArquivo(f1, f2, f3, f4, f5, f6);    // Faz a ordena��o, usando f1, f2 e f3 de entrada, f4, f5 e f6 de sa�da
            fclose(f1); fclose(f2); fclose(f3); fclose(f4); fclose(f5); fclose(f6); // Fecha os arquivos
        }else{
            f1 = fopen("f1.txt", "w");  // Abre a fita1 em modo grava��o
            f2 = fopen("f2.txt", "w");  // Abre a fita2 em modo grava��o
            f3 = fopen("f3.txt", "w");  // Abre a fita3 em modo grava��o
            f4 = fopen("f4.txt", "r");  // Abre a fita4 em modo leitura
            f5 = fopen("f5.txt", "r");  // Abre a fita5 em modo leitura
            f6 = fopen("f6.txt", "r");  // Abre a fita6 em modo leitura
            passadaArquivo(f4, f5, f6, f1, f2, f3);    // Faz a ordena��o, usando f4, f5 e f6 de entrada, f1, f2 e f3 de sa�da
            fclose(f1); fclose(f2); fclose(f3); fclose(f4); fclose(f5); fclose(f6); // Fecha os arquivos
        }
        cont++;         // Incrementa o contador
    }

    // Converte o arquivo de saida separado por \t para \n
    saida = fopen("saida.txt", "w");    // Abre o arquivo de sa�da em modo grava��o

    if(passadas % 2 == 0){     // Verifica se a sa�da est� no f1.txt ou f4.txt e coverte o arquivo, armazenando o resultado em saida.txt
        f1 = fopen("f1.txt", "r");  // Abre a fita1 em modo leitura
        converterArquivoSaida(f1, saida, quant);    // Converte a fita1 para o arquivo de sa�da
        fclose(f1);                                 // Fecha a fita 1
    }else{
        f4 = fopen("f4.txt", "r");  // Abre a fita4 em modo leitura
        converterArquivoSaida(f4, saida, quant);    // Converte a fita4 para o arquivo de sa�da
        fclose(f4);                                 // Fecha a fita 4
    }
    fclose(saida);          // Fecha o arquivo de sa�da

    printf("\n\nArquivo ordenado!\n");          // Informa que o arquivo foi ordenado
    printf("Arquivo de saida: saida.txt\n");    // Informa o arquivo de sa�da

    return 0;
}

/**
* Metodo de ordena��o
* @param arr Vetor de inteiros que sera ordenado
* @param n Quantidade de elementos no vetor para ordenar
*/
void insertionSort(int arr[], int n){       // M�todo de ordena��o de um vetor
   int i, key, j;
   for (i = 1; i < n; i++){
       key = arr[i];
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j] > key){
           arr[j+1] = arr[j];
           j = j-1;
       }
       arr[j+1] = key;
   }
}

/**
* Metodo de ordenacao de um vetor da struct Buffer
* @param arr Vetor de struct Buffer que sera ordenado
* @param n Quantidade de elementos no vetor para ordenar
*/
void insertionSortBuf(Buffer arr[], int n){      // M�todo de ordena��o usando um vetor da struct Buffer
   int i, key, fita, j;
   for (i = 1; i < n; i++){
       key = arr[i].elem;
       fita =  arr[i].fita;
       j = i-1;

       /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
       while (j >= 0 && arr[j].elem > key){
           arr[j+1].elem = arr[j].elem;
           arr[j+1].fita = arr[j].fita;
           j = j-1;
       }
       arr[j+1].elem = key;
       arr[j+1].fita = fita;
   }
}

/**
* Faz a primeira divisao: divide o arquivo de entrada em arquivos menores e retorna a quantidade de n�meros do arquivo de entrada
* @param entrada Arquivo de entrada: a primeira linha do arquivo indica quantas linhas tem nele (sem contar com essa primeira linha)
* @param arquivo1 Arquivo que ser� usado como fita 1
* @param arquivo2 Arquivo que ser� usado como fita 2
* @param arquivo3 Arquivo que ser� usado como fita 3
* @return Numero de registros do arquivo a serem ordenados
*/
int divideArquivos(FILE *entrada, FILE *arquivo1, FILE *arquivo2, FILE *arquivo3){
    int buffer[TAM], indice, cont, i, quant = 0;

    if(entrada == 0){
        printf("Erro na abertura do arquivo.\n");
        fclose(entrada);
    }else{
        indice = 0;
        cont = 1;
        fscanf(entrada, "%d\n", &quant);    // Pega o primeiro n�mero do arquivo, que indica a quantidade de n�meros a serem lidos

        for(i = 1; i <= quant; i++){
            fscanf(entrada, "%d\n", &buffer[indice]);
            if(i % TAM == 0){               // Verifica se foram preenchidas as 3 posi��es do buffer
                insertionSort(buffer, TAM); // Ordena o buffer
                switch(cont){               // Verifica qual a fita a ser gravada
                    case 1:
                        gravarBuffer(buffer, TAM, arquivo1);
                        putc('\n', arquivo1);           // Finaliza o bloco com uma quebra de linha
                        break;
                    case 2:
                        gravarBuffer(buffer, TAM, arquivo2);
                        putc('\n', arquivo2);           // Finaliza o bloco com uma quebra de linha
                        break;
                    case 3:
                        gravarBuffer(buffer, TAM, arquivo3);
                        putc('\n', arquivo3);           // Finaliza o bloco com uma quebra de linha
                        cont = 0;
                }
                indice = 0;                 // Reseta o �ndice do buffer
                cont++;                     // Incrementa o contador da fita
            }else{
                indice++;                   // Incrementa o contador de �ndice do buffer
            }
        }

        if(quant % TAM != 0){           // Verifica se sobraram n�meros no buffer a serem gravados
            switch(cont){                   // Verifica qual a fita a ser gravada
                case 1:
                    if(quant % TAM == 1){       // Grava o n�mero, se for apenas um
                        fprintf(arquivo1, "%d", buffer[0]);
                    }else{                      // Se forem dois n�meros, ordena os dois e grava
                        insertionSort(buffer, 2);
                        gravarBuffer(buffer, 2, arquivo1);
                    }
                    break;
                case 2:
                    if(quant % TAM == 1){       // Grava o n�mero, se for apenas um
                        fprintf(arquivo2, "%d", buffer[0]);
                    }else{                      // Se forem dois n�meros, ordena os dois e grava
                        insertionSort(buffer, 2);
                        gravarBuffer(buffer, 2, arquivo2);
                    }
                    break;
                case 3:
                    if(quant % TAM == 1){       // Grava o n�mero, se for apenas um
                        fprintf(arquivo3, "%d", buffer[0]);
                    }else{                      // Se forem dois n�meros, ordena os dois e grava
                        insertionSort(buffer, 2);
                        gravarBuffer(buffer, 2, arquivo3);
                    }
            }
        }
    }
    return quant;
}

/**
* Grava o buffer (vetor de inteiros de n posicoes) em um arquivo
* @param bf Vetor de inteiros que ser� gravado
* @param tamanho Quantidade de elementos que sera gravado
* @param arquivo Arquivo no qual os elementos serao gravados
*/
void gravarBuffer(int bf[], int tamanho, FILE *arquivo){
    int i;
    for(i = 0; i < tamanho; i++){   // Grava o buffer na fita
        fprintf(arquivo, "%d", bf[i]);
        if(i < tamanho - 1)
            putc('\t', arquivo);   // Separa os elementos com tab
    }
}

/**
* Grava no arquivo de sa�da o bloco de elementos dos arquivos de entrada ordenados
* @param entrada1 Arquivo de entrada 1
* @param entrada2 Arquivo de entrada 2
* @param entrada3 Arquivo de entrada 3
* @param saida Arquivo de saida
*/
void ordenarBloco(FILE *entrada1, FILE *entrada2, FILE *entrada3, FILE *saida){
    int f1v, f2v, f3v;      // Usadas para indicar se ainda h� n�meros no bloco
    char c;
    Buffer buffer[TAM];

    if(fscanf(entrada1, "%d", &buffer[0].elem) == 1){   // Pega o primeiro n�mero do bloco
        buffer[0].fita = 1;   f1v = 1;
    }else{
        f1v = 0;                                        // Ou indica que n�o h� n�meros nesse bloco
    }
    if(fscanf(entrada2, "%d", &buffer[1].elem) == 1){   // Pega o primeiro n�mero do bloco
        buffer[1].fita = 2;   f2v = 1;
    }else{
        f2v = 0;                                        // Ou indica que n�o h� n�meros nesse bloco
    }
    if(fscanf(entrada3, "%d", &buffer[2].elem) == 1){   // Pega o primeiro n�mero do bloco
        buffer[2].fita = 3;   f3v = 1;
    }else{
        f3v = 0;                                        // Ou indica que n�o h� n�meros nesse bloco
    }

    while(f1v || f2v || f3v){       // Verifica se ainda h� n�meros nos blocos

        insertionSortBuf(buffer, f1v + f2v + f3v);      // Ordena o buffer
        fprintf(saida, "%d", buffer[0].elem);           // Grava na sa�da o menor n�mero

        switch(buffer[0].fita){                         // Verifica de qual arquivo saiu o menor n�mero
            case 1:
                c = fgetc(entrada1);
                if(c != '\t'){          // Se n�o houver mais n�meros no bloco, indica na variavel de validade do bloco
                    f1v = 0;            // e puxa as posi��es seguintes do buffer
                    buffer[0].elem = buffer[1].elem;    buffer[0].fita = buffer[1].fita;
                    buffer[1].elem = buffer[2].elem;    buffer[1].fita = buffer[2].fita;
                }else{                  // Se ainda houver, pega o pr�ximo n�mero e insere no buffer
                    fscanf(entrada1, "%d", &buffer[0].elem);
                }
                break;
            case 2:
                c = fgetc(entrada2);
                if(c != '\t'){          // Se n�o houver mais n�meros no bloco, indica na variavel de validade do bloco
                    f2v = 0;            // e puxa as posi��es seguintes do buffer
                    buffer[0].elem = buffer[1].elem;    buffer[0].fita = buffer[1].fita;
                    buffer[1].elem = buffer[2].elem;    buffer[1].fita = buffer[2].fita;
                }else{                  // Se ainda houver, pega o pr�ximo n�mero e insere no buffer
                    fscanf(entrada2, "%d", &buffer[0].elem);
                }
                break;
            case 3:
                c = fgetc(entrada3);
                if(c != '\t'){          // Se n�o houver mais n�meros no bloco, indica na variavel de validade do bloco
                    f3v = 0;            // e puxa as posi��es seguintes do buffer
                    buffer[0].elem = buffer[1].elem;    buffer[0].fita = buffer[1].fita;
                    buffer[1].elem = buffer[2].elem;    buffer[1].fita = buffer[2].fita;
                }else{                  // Se ainda houver, pega o pr�ximo n�mero e insere no buffer
                    fscanf(entrada3, "%d", &buffer[0].elem);
                }
                break;
        }
        if(f1v || f2v || f3v){      // Se ainda houver n�meros em algum dos blocos, adiciona um tab para separar os n�meros no arquivo de sa�da
            fputc('\t', saida);
        }else{                      // Caso contr�rio, quebra a linha
            fputc('\n', saida);
        }
    }
}

/**
* Calcula e retorna quantas passadas nos arquivos, apos a primeira divisao, serao necessarias para a ordenacao
* @param registros numero de elementos do arquivo de entrada a serem ordenados
* @param buffer numero de posicoes do buffer utilizado na ordena��o
* @return numero de passadas que ser�o necessarias para a ordena��o
*/
int numeroDePassadas(float registros, int buffer){
    if(registros <= buffer)
        return 0;
    return 1 + numeroDePassadas(registros / buffer, buffer);
}

/**
* Faz a ordena��o de todos os blocos de elementos dos arquivos de entrada para os arquivos de sa�da
* @param registros numero de elementos do arquivo de entrada a serem ordenados
* @param entrada1 Arquivo de entrada 1
* @param entrada2 Arquivo de entrada 2
* @param entrada3 Arquivo de entrada 3
* @param saida1 Arquivo de saida 1
* @param saida2 Arquivo de saida 2
* @param saida3 Arquivo de saida 3
*/
void passadaArquivo(FILE *entrada1, FILE *entrada2, FILE *entrada3, FILE *saida1, FILE *saida2, FILE *saida3){
    int cont = 1;
    while(!feof(entrada1) && !feof(entrada2) && !feof(entrada3)){       // Verifica se chegou ao fim dos tr�s arquivos de entrada
        switch(cont){   // Usa o contador para alternar entre os arquivos de sa�da
            case 1:
                ordenarBloco(entrada1, entrada2, entrada3, saida1);     // Passa or blocos dos arquivos de entrada, ordenados, para o arquivo de sa�da1
                break;
            case 2:
                ordenarBloco(entrada1, entrada2, entrada3, saida2);     // Passa or blocos dos arquivos de entrada, ordenados, para o arquivo de sa�da2
                break;
            case 3:
                ordenarBloco(entrada1, entrada2, entrada3, saida3);     // Passa or blocos dos arquivos de entrada, ordenados, para o arquivo de sa�da3
                cont = 0;       // Reseta o contador
        }
        cont++;     // Incrementa o contador
    }
}

/**
* Converte o arquivo de entrada, onde os numeros separados por \t para serem separados por \n, no arquivo de saida
* @param entrada Arquivo de entrada
* @param saida Arquivo de saida
* @param quantidade Quantidade de elementos do arquivo de entrada
*/
void converterArquivoSaida(FILE *entrada, FILE *saida, int quantidade){
    int i, num;
    for(i = 0; i < quantidade; i++){        // Executa n vezes, de acordo com a quantidade de registros do arquivo de entrada
        fscanf(entrada, "%d", &num);        // Pega o n�merop do arquivo de entrada
        printf("%i\n", num);                // Printa na tela os n�meros
        fprintf(saida, "%d", num);          // o n�mero no arquivo de sa�da
        if(i < quantidade - 1)              // Verifica se n�o � p �ltimo n�mero
            putc('\n', saida);                  // Quebra a linha
    }
}
