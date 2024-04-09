#ifndef SQL_H
    //Este arquivo define a interface com fragmentos da linguagem SQL para interrragir com o arquivo de dados
    #define SQL_H
    #include<stdbool.h>
    #include<stdlib.h>
    #include<string.h>
    #include<stdio.h>
    #include<stdint.h>
    #include"player.h"
    #include"binFile.h"
    #include"parser.h"
    void selectSQL(char* filename);
    void createTable(char* srcName, char* destName);
#endif