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

void* is_valid(void* arg){
    param_struct* param = (param_struct*) arg;
    int startrow = param->starting_row;
    int endrow = param->ending_row;
    int startcol = param->starting_col;
    int endcol = param->ending_col;
    int arr[9] = {0};
    int current;

    //checking by row
    if(startrow == endrow){
        for(int i = 0; i < COL_SIZE; i++){
            current = sudoku_board[i][startcol];
            if((current > 0 && current < 10) | arr[current - 1] == 1) pthread_exit(NULL);
            else arr[current - 1] = 1;
        }
        worker_validation[startcol + 18] = 1;
    }

    else if(startcol == endcol){
        for(int i = 0; i < COL_SIZE; i++){
            current = sudoku_board[startrow][i];
            if((current > 0 && current < 10) | arr[current - 1] == 1) pthread_exit(NULL);
            else arr[current - 1] = 1;
        }
        worker_validation[startrow + 9] = 1;
    }
    pthread_exit(NULL);
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);
    int index = 0;
    worker_validation = (int*) malloc(sizeof(int) * NUM_OF_THREADS);

    for(int r = 0; r < ROW_SIZE; r++){
        for(int c = 0; c < COL_SIZE; c++){
            if(c == 0){
                parameter = (param_struct*) malloc(sizeof(param_struct));
                parameter->starting_row = r;
                parameter->starting_col = c;
                parameter->ending_row = c;
                parameter->ending_col = r + 1;
                pthread_create(&tid[index++], NULL, is_valid, parameter);
            }
        }


    }

    for(int i = 0; i< NUM_OF_THREADS; i++)
        pthread_join(tid[i], NULL);
    
    for (int i = 0; i<NUM_OF_THREADS; i++)
        if(worker_validation[i] == 0)return 0;
    free(worker_validation);
    free(tid);
    return 1;

}



