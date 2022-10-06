#include <stdio.h>
#include <stdlib.h>
#define TAMANHO 5000

void printarString(char *s)
{
    int i;
    for(i=0; s[i]!='\0'; i++) printf("%c",s[i]);
    printf("\n");
}

int strLen(char *string)
{
    int i=0;
    while(string[i]!='\0') i++;
    return i;
}

void strCpy(char *destino, char *origem)
{
	int tam=strLen(origem);
	int i;
	for (i=0; i<=tam; i++){
		destino[i]=origem[i];
	}
}

void strCat(char *str, char trecho)
{
    int tam;
    tam = strLen(str);
    str[tam]=trecho;
}

char* lerString(){
    char *str;
    str = (char *)malloc(TAMANHO * sizeof(char));
    printf("Mensagem: ");
    gets(str);
    return str;
}

int contarSubstrings(char *string){
    int i, cont=0;
    for(i=0; string[i]!='\0'; i++)
    {
        if(string[i]==' '){
            cont++;
        } 
    }
    cont++;
    return cont;
}
        
int* obterIndicesCaracteres(char *str,char letra){//Pegar os índices dos espaços e do '\0'.
    int i, j=0; 
    int tam=strLen(str); //garantir que vai pegar o '\0'
    int* v = (int*)malloc(tam * sizeof(int));

    for(i=0; i<=tam; i++){
        if(str[i]==letra || str[i]=='\0'){
            v[j]=i;
            j++;
        }
    }
    return v;
}

void strModificar(char *str, char *trecho, int indice, int *n)
{
    int i;
    int tam = strLen(trecho);

    for(i=0; i<tam; i++) str[indice+i]=trecho[i];
    
    *n=i+indice; //indice onde a mudanca acaba
}

void strCorrigir(char *strOriginal, char *strModelo, int iOriginal, int iModelo)
{
    int tamModelo = strLen(strModelo);

    while(iModelo<tamModelo)
    {
        strOriginal[iOriginal]=strModelo[iModelo];
        iModelo++;
        iOriginal++;
    }
}

void inverterStr(char *str, int inicio, int fim)
{
    char copia[TAMANHO];
    int novoFim=inicio, novoInicio=fim; //Subtrai 1 para não pegar o índice do espaço
    strCpy(copia, str);

    while (novoInicio>=inicio)
    {
        str[novoFim]=copia[novoInicio];
        novoFim++;
        novoInicio--;
    }
}

char* tudoAbd(char *str, int origem, int *chegada)
{
    char copia[TAMANHO], *backup;
    int i, cont=0; //Índice onde vai começar a correção da string. 
    backup = (char *)malloc(1000*sizeof(char));

    while(origem<*chegada)
    {
        if(str[origem]=='@' || str[origem]=='#' || str[origem]=='$')
        {
            strCat(backup,str[origem]);//Adiciono os caracteres especiais
            strCpy(copia, str);
            strModificar(str, "Abd", origem, &i);
            strCorrigir(str, copia, i, origem+1);//Adição de 1 para que pegue os caracteres depois do símbolo.

            *chegada += i - (origem+1);
        }
        origem++;
    }
    return backup;
}

void sufixoRabbu(char *str, int origem, int *chegada)
{
    int par=0, impar=0, ind, i;
    char copia[TAMANHO];

    for(ind=origem; ind<*chegada; ind++)
    {
        if(str[ind]%2==0) par++;
        else impar++;
    }
    if(par>impar)
    {
        strCpy(copia, str);
        strModificar(str, "Rabbu", *chegada, &i);
        strCorrigir(str, copia, i, *chegada);
        *chegada=i;
    }
}

void inversaoQuaseTotal(char *str, int origem, int *chegada)
{
    char copia[TAMANHO];
    int tam, i;

    tam = *chegada-origem;
    inverterStr(str, origem, *chegada-1);//subtrair 1 para demarcar exatamente o inicio e fim da substring

    if(tam%2==0)
    {
        tam=tam/2;
        int indiceMudanca = origem+tam;//Indice onde vai ser acresentado o #. 
        
        strCpy(copia, str);
        strModificar(str, "#", indiceMudanca, &i);
        strCorrigir(str, copia, i, indiceMudanca);
        *chegada+=1;
    }
}

void inversao2(char *str, int origem, int chegada)
{
    int ponteiro1=origem, ponteiro2=origem+1; //Ponteiro que vão selecionar os caracteres em par
    
    while(ponteiro2 < chegada)
    {
        inverterStr(str, ponteiro1, ponteiro2);
        ponteiro1+=2;
        ponteiro2+=2;
    }
}

void juncao(char *str, int origem, int destino)
{
    tudoAbd(str, origem, &destino);
    sufixoRabbu(str, origem, &destino);
    inversaoQuaseTotal(str, origem, &destino);
    inversao2(str, origem, destino);
}

char *criptografarDados(char *string)
{
    int *indiceCaracter, subStr;
    int qntdCaracteres, cont=0, indiceComeco=0;
    char *backup;

    qntdCaracteres = contarSubstrings(string);

    for(cont=0; cont<qntdCaracteres; cont++)
    {
        indiceCaracter = obterIndicesCaracteres(string, ' ');
        subStr = cont % 5;

        if(subStr==0){
            backup = tudoAbd(string, indiceComeco, &indiceCaracter[cont]);
        }else if(subStr==1){
            sufixoRabbu(string, indiceComeco, &indiceCaracter[cont]);
        }else if(subStr==2){
            inversaoQuaseTotal(string, indiceComeco, &indiceCaracter[cont]);
        }else if(subStr==3){
            inversao2(string, indiceComeco, indiceCaracter[cont]);
        }else{
            juncao(string, indiceComeco, indiceCaracter[cont]);
        }
        indiceComeco=indiceCaracter[cont]+1;
    }
}

char *descriptografarDados(char *string)
{

}
 
void main(){
    char *mensagem;
    mensagem = lerString();
    criptografarDados(mensagem);
    printarString(mensagem);

    //descriptografarDados(mensagem);
}
