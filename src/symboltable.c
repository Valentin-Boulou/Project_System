#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symboltable.h"
#include "conditionaljump.h"
#include "write.h"
#include "functable.h"


extern void initTvartable(void);
extern void initCondJump(void);
extern void initWrite(void);
extern int mylineno;

void init(void){
    var = (Array*)malloc(sizeof(Array));
    var->size = 0;
    var->nbvar = 0;
    offset = 8000;
    initTvartable();
    initCondJump();
    initWrite();
    initFuncTable();
    //var->tab = (lvar*) malloc(sizeof(lvar));
}
Array* newArray(void){
    Array *res = (Array*)malloc(sizeof(Array));
    res->size = 0;
    res->nbvar = 0;
    return res;
}

/*int set_var_to_local_int(char* a, bool cst, bool init, int depth){
    int fin = 0;
    int index = 0;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            var->tab[index].cst = cst;
            var->tab[index].init = init;
            var->tab[index].depth = depth;
            fin = 1;
            res = var->tab[index].address;

        }
        index ++;
    }
    if (!fin){
        var->size++;
        var->nbvar++;
        int pos = var->size-1;
        res = (var->nbvar - 1) * 4 + 8000;
        var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
        var->tab[pos].varname = (char*)malloc(256*sizeof(char));
        strcpy(var->tab[pos].varname, a);
        var->tab[pos].address = res;
        var->tab[pos].cst = cst;
        var->tab[pos].init = init;
        var->tab[pos].depth = depth;

    }else{
        fprintf(stderr,"Error: variable %s \n",a);exit(EXIT_FAILURE);
    }
    return res;
} */

int assign_var_to_local_int(int deb,char* a){
    int fin = 0;
    int index = deb;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            if(var->tab[index].cst == false){
                var->tab[index].init = true;
                var->tab[index].depth = var->tab[index].depth;
                res = var->tab[index].address;
            }else{
                if(var->tab[index].init == true){
                    fprintf(stderr,"Error l%d: variable %s is constant and already intialized\n",mylineno,a);exit(EXIT_FAILURE);
                }else{
                    var->tab[index].init = true;
                    var->tab[index].depth = var->tab[index].depth;
                    res = var->tab[index].address;
                }
            }
            fin = 1;
        }
        index ++;
    }
    if (!fin){
        fprintf(stderr,"Error l%d: variable %s not declared or out of scope.\n",mylineno,a);exit(EXIT_FAILURE);
    }
    return res;
}
/*int assign_ptr_to_local_ptr(char* a){
    int fin = 0;
    int index = 0;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            if((var->tab[index].ptr == true)){
                if(var->tab[index].cst == false){
                var->tab[index].init = true;
                var->tab[index].depth = var->tab[index].depth;
                res = var->tab[index].address;
            }else{
                if(var->tab[index].init == true){
                    printf("ERROR assign_ptr_to_local_ptr1\n");
                    exit(4);
                }else{
                    var->tab[index].init = true;
                    var->tab[index].depth = var->tab[index].depth;
                    res = var->tab[index].address;
                }
            }
            fin = 1;
            }
            
        }
        index ++;
    }
    if (!fin){
        printf("ERROR assign_ptr_to_local_ptr2\n");
    }
    return res;
}*/




int get_local_var_addr(int deb,char* a){
    int fin = 0;
    int index = deb;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            fin = 1;
            res = var->tab[index].address;

        }
        index ++;
    }
    if (!fin){
        fprintf(stderr,"Error l%d: variable %s not declared or out of scope.\n",mylineno,a);exit(EXIT_FAILURE);
    }
    return res;
}
/*int get_local_ptr_addr(char *a){
    int fin = 0;
    int index = 0;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a) && var->tab[index].ptr == true){
            fin = 1;
            res = var->tab[index].address;

        }
        index ++;
    }
    if (!fin){
        printf("ERROR get_local_ptr_addr de:%s\n", a);
    }
    return res;
}*/

int varname_to_address(int deb,char* a){
    int fin = 0;
    int index = deb;
    int res = -1;
    while(!fin && index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            res = var->tab[index].address;
            fin = 1;
        }
        index ++;
    }
    if (!fin){
        fprintf(stderr,"Error l%d: variable %s not declared or out of scope.\n",mylineno,a);exit(EXIT_FAILURE);

    }
    return res;
}

int initialize_var_to_local_int(int deb,char* a, bool cst, bool init, int depth){

    int index = deb;
    int res = -1;
    while(index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            fprintf(stderr,"Error l%d: variable %s already declared.\n",mylineno,a);exit(EXIT_FAILURE);
            exit(EXIT_FAILURE);
        }
        index ++;
    }
    var->size++;
    var->nbvar++;
    int pos = var->size-1;
    res = (var->nbvar - 1) * 4 + offset;
    var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    var->tab[pos].varname = (char*)malloc(256*sizeof(char));
    strcpy(var->tab[pos].varname, a);
    var->tab[pos].address = res;
    var->tab[pos].cst = cst;
    var->tab[pos].init = init;
    var->tab[pos].depth = depth;


    return res;
}

/*int initialize_var_to_local_ptr(char* a,bool cst, bool init, int depth, int addr){

    int index = 0;
    int res = -1;
    while(index < var->size){
        if (!strcmp(var->tab[index].varname, a)){
            printAll();
            printf("ERROR initialize_var_to_local_int1 de : %s\n", a);
            exit(EXIT_FAILURE);
        }
        index ++;
    }

    var->size++;
    var->nbvar++;
    int pos = var->size-1;
    res = addr;
    var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    var->tab[pos].varname = (char*)malloc(256*sizeof(char));
    strcpy(var->tab[pos].varname, a);
    var->tab[pos].address = res;
    var->tab[pos].cst = cst;
    var->tab[pos].init = init;
    var->tab[pos].depth = depth;
    var->tab[pos].ptr = true;

    return res;
}*/

void del_var_name(int deb,char* dvar){
    if (var->size == 0){
        printf("ERROR del_var_name1");
    }else if (deb == 0 && var->size == 1){
        var->size = 0;
        free(var->tab[0].varname);
        var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    }else{
        int fin = 0;
        int i = deb;
        while(i < var->size && !fin){
            if(!strcmp(var->tab[i].varname,dvar)){
                fin =true;
                if(var->size - 1 != i){
                    strcpy(var->tab[i].varname, var->tab[var->size - 1].varname);
                    var->tab[i].depth = var->tab[var->size - 1].depth;
                    var->tab[i].cst = var->tab[var->size - 1].cst;
                    var->tab[i].init = var->tab[var->size - 1].init;
                    var->tab[i].address= var->tab[var->size - 1].address;

                }
                free(var->tab[var->size - 1].varname);
                var->size--;
                var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
            }
            i++;
        }
        if(fin == false){
            printf("ERROR del_var_name2");
        }
    }
}

/*void del_var_pos(int dvar){
    if (dvar == var->size-1){
        free(var->tab[var->size - 1].varname);
        var->size--;
        var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    }else if(dvar < var->size-1){
        strcpy(var->tab[dvar].varname, var->tab[var->size - 1].varname);
        var->tab[dvar].depth = var->tab[var->size - 1].depth;
        var->tab[dvar].cst = var->tab[var->size - 1].cst;
        var->tab[dvar].init = var->tab[var->size - 1].init;
        free(var->tab[var->size - 1].varname);
        var->size--;
        var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    }else{
        printf("ERROR del_var_pos1");
    }
}*/

void incrementeDepth(int deb){
    for (int a = deb; a < var->size; a++){
        var->tab[a].depth++;
    }
}

void decrementeDepth(int deb){
    int del[var->size - deb];
    int index = 0;
    memset(del,-1,var->size - deb);
    for (int a = deb; a < var->size; a++){
        if(var->tab[a].depth == 0){
            del[index] = a;
            index++;
        }else{
            var->tab[a].depth--;
        }
    }
    int dels = 0;

    while(index){
        if(del[index-1] != var->size - 1 -  dels){
            strcpy(var->tab[del[index-1]].varname, var->tab[var->size - 1 - dels].varname);
            var->tab[del[index-1]].depth = var->tab[var->size - 1- dels].depth;
            var->tab[del[index-1]].cst = var->tab[var->size - 1- dels].cst;
            var->tab[del[index-1]].init = var->tab[var->size - 1- dels].init;
            var->tab[del[index-1]].address = var->tab[var->size - 1- dels].address;

        }
        dels++;
        index--;
    }
   
    if(dels){
        for (int z = 0; z < dels; z++){
            free(var->tab[var->size - 1 - z].varname);
        }
        var->size -= dels;
        var->nbvar -= dels;
        var->tab = (lvar*)realloc(var->tab,var->size * sizeof(lvar));
    }
}

void dellvar(Array* var){
    for (int a = 0; a < var->size; a++){
        free(var->tab[a].varname);
    }
    free(var->tab);
    free(var);
}
int getSize(){
    return var->nbvar;
}

int incrementOffset(int a){
    offset += a;
    return offset;
}
int decrementOffset(int a){
    offset -= a;
    return offset;
}



void printAll(void){
    printf("----------------------------------------------------------\n");
    printf("|     varname     |     address     | cst | init | depth |\n");
    for (int a = 0; a<var->size; a++){
        char str1[18] = {"                 "};
        strncpy(str1, var->tab[a].varname,strlen(var->tab[a].varname));
        printf("----------------------------------------------------------\n");
        printf("|%-17s|%-17d|%d    |%d     |%-7d|\n",str1,var->tab[a].address,var->tab[a].cst,var->tab[a].init,var->tab[a].depth);
        

    }
    printf("----------------------------------------------------------\n");
}