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
    for(int i = 0; i < ROW_SIZE; i++){
        parameter[i].starting_row = i;
        parameter[i].starting_col = 0;
        parameter[i].ending_col = 0;
        parameter[i].ending_row = i;
        pthread_create(&(tid[i]), &attr, valid_row(sudoku_board,parameter[i]), &(parameter[i]));
    }

    
    // replace this comment with your code
    for(int i = 0;i<NUM_OF_THREADS; i++) 
        if (worker_validation[i] != 1) return 0;
    return 1;
    
}

int valid_row(int** board, param_struct p){
    int ans[9];
    for(int i = p.starting_row; i < p.ending_row; i++)
        for(int z = p.starting_col; z < p.ending_row; z++){
            int current = board[i][z];
            ans[current - 1] = current - (current - 1);
        }

    for(int i = 0; i < ROW_SIZE; i++)
        if(ans[i] != 1) return 0;
    return 1;
}
