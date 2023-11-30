#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    sudoku_board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
	    sudoku_board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    }

    fp = fopen(filename,"r");
    for(int i = 0; i < ROW_SIZE; i++) {
        for(int j = 0; j < COL_SIZE; j++) {
            fscanf(fp, "%d%*c", &sudoku_board[i][j]);
        }
    }   

    for(int i = 0; i<ROW_SIZE; i++){
        for(int j = 0; j<COL_SIZE; j++)
        {
            printf("%d ", sudoku_board[i][j]);
        }
        printf("\n");
    }

    fclose(fp);
    return sudoku_board;
}

void* validateRow(void* arg){
    param_struct* param = (param_struct*) arg;
    int startrow = param->starting_row;
    int endrow = param->ending_row;
    int startcol = param->starting_col;
    int endcol = param->ending_col;
    int current;
    int arr[9] = {0};

    for(int i = 0; i < ROW_SIZE; i++){
            current = sudoku_board[startrow][i];
            if((current > 9 || current < 1) || arr[current - 1] == 1) {
                printf("Row: %d and Column: %d\n",startrow+1,i+1);
                pthread_exit(NULL);
            }
            else arr[current - 1] = 1;
        }
        worker_validation[startrow + 9] = 1;
}

void* validateCol(void* arg){
    param_struct* param = (param_struct*) arg;
    int startrow = param->starting_row;
    int startcol = param->starting_col;
    int current;
    int arr[9] = {0};


    for(int i = 0; i < ROW_SIZE; i++){
            current = sudoku_board[startrow][i];
            if((current > 9 || current < 1) || arr[current - 1] == 1){ 
                printf("Row: %d and Column: %d\n",i+1,startcol+1);
                pthread_exit(NULL);
            }
            else arr[current - 1] = 1;
        }
        worker_validation[startrow + 9] = 1;
        pthread_exit(NULL);
}

void* validate3x3(void* arg){
    param_struct* param = (param_struct*) arg;
    int startrow = param->starting_row;
    int endrow = param->ending_row;
    int startcol = param->starting_col;
    int endcol = param->ending_col;
    int arr[9] = {0};
    int current;

    
    for(int r = startrow; r < startrow + 3; r++){
        for(int c = startcol; c < startcol + 3; c++){
            current = sudoku_board[r][c];
            if((current > 9 || current < 1) || arr[current - 1] == 1) {
                printf("Row: %d and Column: %d\n",r+1,c+1);
                pthread_exit(NULL);
            }
            else arr[current - 1] = 1;
        }
    }
    worker_validation[startrow + startcol / 3] = 1;
    pthread_exit(NULL);
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    tid = (pthread_t*) malloc(sizeof(int*) * NUM_OF_THREADS);
    int index = 0;
    worker_validation = (int*) malloc(sizeof(int) * NUM_OF_THREADS);
    printf("Invalids:\n");
    for(int r = 0; r < ROW_SIZE; r++){
        for(int c = 0; c < COL_SIZE; c++){
            if(r % 3 == 0 && c % 3 == 0){
                parameter = (param_struct*) malloc(sizeof(param_struct*));
                parameter->starting_row = r;
                parameter->starting_col = c;
                parameter->ending_row = r+1;
                parameter->ending_col = c+1;
                pthread_create(&tid[index++], NULL, validate3x3, parameter);
            }
            if(r == 0){
                parameter = (param_struct*) malloc(sizeof(param_struct*));
                parameter->starting_row = r;
                parameter->starting_col = c;
                parameter->ending_row = r;
                parameter->ending_col = c+1;
                pthread_create(&tid[index++], NULL, validateCol, parameter);
            }
            if(c == 0){
                parameter = (param_struct*) malloc(sizeof(param_struct*));
                parameter->starting_row = r;
                parameter->starting_col = c;
                parameter->ending_row = c;
                parameter->ending_col = r+1;
                pthread_create(&tid[index++], NULL, validateRow, parameter);
            }
        }


    }

    for(int i = 0; i< NUM_OF_THREADS; i++){
        pthread_join(tid[i], NULL);
    }
    for (int i = 0; i<NUM_OF_THREADS; i++){
        if(worker_validation[i] == 0)return 0;
    }
    free(worker_validation);
    free(tid);
    return 1;
}
