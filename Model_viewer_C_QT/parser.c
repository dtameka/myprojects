#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static char NUMBERS[] = "-1234567890.";
static char NUMBERS_F[] = "1234567890";
static char NUMBERS_SLASH[] = "1234567890/";

int initialize_file(const char *path, FILE* source, data_t* data) {
    int flag = 0;
        if ((source = fopen(path, "r")) == NULL) {
            flag = 1;
        } else {
            char ch[3];
            while (!feof(source)) {
                if (NULL != fgets(ch, 3, source)) {
                    if (ch[0] == 'v' && ch[1] == ' ') {
                        data->count_of_vertexes++;
                    } else if (ch[0] == 'f' && ch[1] == ' ') {
                        data->count_of_facets++;
                    }
                }
            }
        fclose(source);
        }
    return flag;
}

int parse_object_file(const char *path, double **vertices, int **indices,
                    int *v_count, int *i_count, int *vert_count, int* edge_count) {
    FILE* source = NULL;
    data_t data;
    int flag = 0;
    if ((source = fopen(path, "r")) == NULL) {
        flag = 1;
    } else {
        data_t data;
        initialize_struct(&data);
        initialize_file(path, source, &data);
        *vert_count = data.count_of_vertexes;
        *edge_count = data.count_of_facets;
        memory_allocation(&data);
        unsigned int j_point = 1;
        unsigned int i_point = 1;
        while (!feof(source)) {
            char str[256];
            char* string = str;
            char out_str[256];
            char* offset_pointer = out_str;
            memset(offset_pointer, 0, 256);
            if (NULL != fgets(string, 256, source)) {
                if (*string == 'v') {
                    string++;
                    if (*string == ' ') {
                        for (int i = 0, k = 0; *string != '\n' && *string != '\r'
                            && j_point < data.count_of_vertexes + 1; i++) {
                            int match = 0;
                            if (*string == ' ') {
                                string++;
                            } else {
                                match = strspn(string, NUMBERS);
                                strncpy(offset_pointer, string, match);
                                string += match;
                                data.matrix_3d.matrix[j_point][k] = atof(offset_pointer);
                                memset(offset_pointer, 0, 256);
                                k++;
                                if (k == 3) {
                                j_point++;
                                }
                            }
                        }
                    }
                } else if (*string == 'f') {
                    memset(offset_pointer, 0, 256);
                    string = string + 2;
                    data.polygons[i_point].numbers_of_vertexes_in_facets = count_the_vertexes(string);
                    data.polygons[i_point].vertexes =
                    (int*)calloc(data.polygons[i_point].numbers_of_vertexes_in_facets, sizeof(int*));

                    unsigned int i_point_f = 1;
                    while (*string != '\n' && *string != '\r' && *string != '\0') {
                        int match = 0;
                        int match_need = 0;
                        match = strspn(string, NUMBERS_SLASH);
                        if (*string == '/') {
                        string += match+1;
                        } else {
                            match_need = strspn(string, NUMBERS_F);
                            strncpy(offset_pointer, string, match_need);
                            data.polygons[i_point].vertexes[i_point_f] = atoi(out_str);
                            memset(offset_pointer, 0, 256);
                            i_point_f++;
                            if (*string != '\r' && *string != '\n' && *string != '\0') {
                                string += match+1;
                            } else {
                                break;
                            }
                        }
                    } i_point++;
                }
            }
        }
    fclose(source);
    // заполнение массивов для передачи в QT
    int count_edges = 0;
    int count_vertexes = 0;
    double* tmp_arr_vert = NULL;
    int* tmp_arr_ind = NULL;
    get_arrays(data, tmp_arr_vert, tmp_arr_ind, &count_vertexes, &count_edges);
    tmp_arr_vert = (double *)malloc(count_vertexes * sizeof(double));
    tmp_arr_ind = (int *)malloc(count_edges * sizeof(int));
    get_arrays(data, tmp_arr_vert, tmp_arr_ind, &count_vertexes, &count_edges);
    *vertices = tmp_arr_vert;
    *indices = tmp_arr_ind;
    *v_count = count_vertexes;
    *i_count = count_edges;
    tmp_arr_ind = NULL;
    tmp_arr_vert = NULL;
    free(tmp_arr_ind);
    free(tmp_arr_vert);
    memory_deallocation(&data);
        for (int i = 1; data.count_of_facets+1 > i; i++) {
            if (data.polygons[i].vertexes != NULL) {
                free(data.polygons[i].vertexes);
                data.polygons[i].vertexes = NULL;
            }
        }
    free(data.polygons);
    }
    return flag;
}

void s21_remove_matrix(matrix_t* A) {
    if (A->matrix != 0x0) {
        for (int i = 0; i < A->rows; i++) {
            if (A->matrix[i] != NULL) {
                free(A->matrix[i]);
                A->matrix[i] = NULL;
            }
        }
        free(A->matrix);
        A->matrix = NULL;
        A->rows = 0;
        A->columns = 0;
    }
}


void get_arrays(data_t data, double* arr_vert, int* arr_ind, int* count_vert, int* count_edg) {
    // Заполнение массива индексов
    if (*count_vert == 0) {
        *count_vert = data.count_of_vertexes * 3;
        for ( unsigned int i = 1; data.count_of_facets >= i; i++ ) {
            *count_edg += data.polygons[i].numbers_of_vertexes_in_facets * 2;
        }
    } else {
        int count_sides = 0;
        int count_vertexes = data.count_of_vertexes * 3;
        for ( unsigned int i = 1; data.count_of_facets >= i; i++ ) {
            count_sides += data.polygons[i].numbers_of_vertexes_in_facets * 2;
        }
        unsigned int i_point = 0;
        unsigned int j_point = 0;
        unsigned int tmp_j = 0;




        for ( unsigned int i = 1; data.count_of_facets >= i; i++ ) {
            for (int j = 0, k = 1; data.polygons[i].numbers_of_vertexes_in_facets > j; j++, k++) {
                if (tmp_j == 0) {
                    arr_ind[j_point] = data.polygons[i].vertexes[k] - 1;
                    j_point++;
                    tmp_j++;
                } else {
                    arr_ind[j_point] = data.polygons[i].vertexes[k] - 1;
                    arr_ind[j_point+1] = data.polygons[i].vertexes[k] - 1;
                    j_point += 2;
                    if (data.polygons[i].numbers_of_vertexes_in_facets <= j+1) {
                        arr_ind[j_point] = data.polygons[i].vertexes[1] - 1;
                        j_point++;
                        tmp_j = 0;
                    }
                }
            }
        }
        i_point = 0;
        j_point = 1;
        for (unsigned int i = 0, k = 0; count_vertexes > i; i++, i_point++) {
            *arr_vert = data.matrix_3d.matrix[j_point][k];
            arr_vert++;
            i_point++;
            k++;
            if (k > 2) {
                k = 0;
                j_point++;
            }
        }
    }
}

void initialize_struct(data_t* data) {
    data->count_of_facets = 0;
    data->count_of_vertexes = 0;
    data->matrix_3d.columns = 0;
    data->matrix_3d.rows = 0;
    data->matrix_3d.matrix = NULL;
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
    int res = 0;
    if (rows < 1 || columns < 1) {
        res = 1;
        result->matrix = NULL;
    } else {
        result->columns = columns;
        result->rows = rows;
        result->matrix = malloc(sizeof(*result->matrix) * rows);
        for (int i = 0; i < rows; i++) {
            result->matrix[i] = malloc(sizeof(**result->matrix) * columns);
        }
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++) result->matrix[i][j] = 0;
    }
    return res;
}

void memory_allocation(data_t* data) {
    s21_create_matrix(data->count_of_vertexes+1, 3, &data->matrix_3d);
    data->polygons = calloc(data->count_of_facets+1, sizeof(polygon_t));
}

void memory_deallocation(data_t* data) {
    s21_remove_matrix(&data->matrix_3d);
}

int count_the_vertexes(char* string) {
    char *string_tmp = string;
    int result_counter = 0;
    strncpy(string_tmp, string, (int)strlen(string));
    while ( *string_tmp != '\n' && *string_tmp != '\r' && *string_tmp != '\0' ) {
        if (*string_tmp == ' ') {
            string_tmp++;
        } else {
            int match = 0;
            match = strspn(string_tmp, NUMBERS_SLASH);
            string_tmp += match;
            result_counter++;
        }
    }
    return result_counter;
}

void min_max(double *vertices, double *min, double *max, int vertCount) {
    *max = vertices[0];
    *min = vertices[0];
    for ( int i = 0; i < vertCount; i++ ) {
        if (vertices[i] < (*min))
            *min = vertices[i];
        if (vertices[i] > (*max))
            *max = vertices[i];
    }
}

int moveMatrix(double* arr_vert, int* count_vert,
               double d_X, double d_Y, double d_Z) {
    int result = 0;
    for (double *pr = arr_vert; pr - arr_vert < *count_vert;) {
        *pr += d_X;
        *(pr + 1) += d_Y;
        *(pr + 2) += d_Z;
        pr += 3;
    }
    return result;
}

int rotateMatrix(double* arr_vert, int* count_vert,
                 double dFi_X, double dFi_Y, double dFi_Z) {
    int result = 0;
    if (dFi_X != 0) {
       for (double *pr = arr_vert; pr - arr_vert < *count_vert;) {
           double bufY = *(pr + 1);
           double bufZ = *(pr + 2);
               *(pr + 1) = bufY * cos(dFi_X) + bufZ * sin(dFi_X);
               *(pr + 2) = -1 * bufY * sin(dFi_X) + bufZ * cos(dFi_X);
               pr += 3;
            }
        }

    if (dFi_Y != 0) {
        for (double *pr = arr_vert; pr - arr_vert < *count_vert;) {
            double bufX = *pr;
            double bufZ = *(pr + 2);
            *pr = bufX * cos(dFi_Y) - bufZ * sin(dFi_Y);
            *(pr + 2) = bufX * sin(dFi_Y) + bufZ * cos(dFi_Y);
            pr += 3;
        }
    }

    if (dFi_Z != 0) {
        for (double *pr = arr_vert; pr - arr_vert < *count_vert;) {
            double bufX = *pr;
            double bufY = *(pr + 1);
            *pr = bufX * cos(dFi_Z) - bufY * sin(dFi_Z);
            *(pr + 1) = bufX * sin(dFi_Z) + bufY * cos(dFi_Z);
            pr += 3;
        }
    }

    return result;
}

int scaleMatrix(double* arr_vert, int* count_vert,
                double xFactor, double yFactor, double zFactor) {
    int result = 0;
    for (double *pr = arr_vert; pr - arr_vert < *count_vert;) {
        if (xFactor != 0) *pr *= xFactor;
        if (yFactor != 0) *(pr + 1) *= yFactor;
        if (zFactor != 0) *(pr + 2) *= zFactor;
        pr += 3;
    }
    return result;
}
