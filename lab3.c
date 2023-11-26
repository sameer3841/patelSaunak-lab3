#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){

    FILE *fp = fopen(filename,"r");
    int** board = (int**)malloc(ROW_SIZE * sizeof(int*));

    for(int i = 0; i < COL_SIZE; i++)
        board[i] = (int*) malloc(COL_SIZE * sizeof(int));
    if(fp == NULL) return;

    for(int r = 0; r < ROW_SIZE; r++)
        for(int c = 0; c < COL_SIZE; c++)
            fscanf(fp,"%d",&board[r][c]);
    fclose(fp);
    return board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);
    parameter = (param_struct*) malloc(sizeof(param_struct) * NUM_OF_THREADS);
    parameter[0].starting_row = 0;
    parameter[0].starting_col = 0;
    parameter[0].ending_col = COL_SIZE - 1;
    parameter[0].ending_row = 0;

    
    // replace this comment with your code

    
}


