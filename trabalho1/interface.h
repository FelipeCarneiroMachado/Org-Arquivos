/*
================================================
Arquivo com a interface para manipulacao da base de dados
Essecialmente, toda interacao com a base eh feita por este header
================================================
*/
#ifndef SQL_H
    #define SQL_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"binFile.h"
    void selectAll(char* filename); //Printa todo o arquivo de dados
    //Recebe 2 arrays de strings, 1 com os campos a serem comparados e outro com os valores
    //Os arrays devem ser pareados (campo[i] corresponde a valor[i])
    void selectWhere(char* filename, int numOfParameters, char** fields, char** values); //Printa os registros que batem com as condicoes
    void createTable(char* srcName, char* destName); //cria o binario a partir de um csv
#endif