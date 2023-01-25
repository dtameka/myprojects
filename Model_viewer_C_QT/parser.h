#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    int parse_object_file(const char *path, double **vertices,
                          int **indices, int *v_count, int *i_count,
                          int *vert_count, int* edge_count);
    void min_max(double *vertices, double *min, double *max, int vertCount);
    int moveMatrix(double* arr_vert, int* count_vert, double d_X, double d_Y, double d_Z);
    int rotateMatrix(double* arr_vert, int* count_vert, double dFi_X, double dFi_Y, double dFi_Z);
    int scaleMatrix(double* arr_vert, int* count_vert,
                    double xFactor, double yFactor, double zFactor);
#ifdef __cplusplus
}
#endif

typedef struct matrix_t {
    double **matrix;
    int rows;
    int columns;
} matrix_t;

typedef struct facets {
    int *vertexes;
    int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct data_t {
    unsigned int count_of_vertexes;
    unsigned int count_of_facets;
    matrix_t matrix_3d;
    polygon_t* polygons;
} data_t;


int initialize_file(const char *path, FILE* source, data_t* data);
int moveMatrix(double* arr_vert, int* count_vert, double d_X, double d_Y, double d_Z);
void initialize_struct(data_t* data);
int s21_create_matrix(int rows, int columns, matrix_t *result);
void memory_allocation(data_t* data);
int parse_object_file(const char *path, double **vertices,
                      int **indices, int *v_count, int *i_count,
                      int *vert_count, int* edge_count);
void get_arrays(data_t data, double* arr_vertexes, int* arr_indexes, int* count_vert, int* count_edg);
int count_the_vertexes(char* string);
void min_max(double *vertices, double *min, double *max, int vertCount);
int rotateMatrix(double* arr_vert, int* count_vert, double dFi_X, double dFi_Y, double dFi_Z);
int scaleMatrix(double* arr_vert, int* count_vert,
                double xFactor, double yFactor, double zFactor);
void memory_deallocation(data_t* data);
void s21_remove_matrix(matrix_t* A);


#endif  //  PARSER_H_
