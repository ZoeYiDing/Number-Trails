/**
 * Time complexity of task 1 is O(n^2)
 * using insertion sort to sort n numbers takes O(n^2)
 * nested for loop to check if there is an edge between n numbers takes O(n^2) because
 * the outer loop runs n times and the inner loop runs n times, and checking 2 numbers
 * is consecutive costs O(1). Therefore, the time complexity of task 1 is O(n^2).
 * 
 * Time complexity of task 2 is O(n^3)
 * floyd-warshall algorithm takes O(n^3) because there are 3 nested loops, and relax the edge
 * costs O(1).
 * After running floyd-warshall algorithm, we need to find the longest path, which takes O(n^2).
 * Outputting the longest path takes O(n) and there are n^2 paths in the worst case, which takes O(n^3).
 * Therefore, the time complexity of task 2 is O(n^3).
 * 
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *create_array(int size, int value);
int **create_2d_array(int rows, int cols, int value);
void insertion_sort(int *array, int size);
int has_edge(int i, int j);
void add_edges(int **graph, int *numbers, int n_numbers);
void output_edges(int **graph, int *numbers, int n_numbers);
void free_2d_array(int **array, int rows);
void free_array(int *array);
int **floyd_warshall(int **graph, int n_numbers, int **pre);
int cmp_path(int *path_a, int *path_b, int length);
void output_path(int *path, int *numbers, int length);
void longest_path(int **graph, int *numbers, int n_numbers, int **pre);

// create an array and initialize all elements
int *create_array(int size, int value) {
    int *array = malloc(size * sizeof(int));
    int i = 0;
    while (i < size) {
        array[i] = value;
        i++;
    }
    return array;
}

// create a 2d array and initialize all elements
int **create_2d_array(int rows, int cols, int value) {
    int **array = malloc(rows * sizeof(int *));
    int row = 0;
    while (row < rows) {
        array[row] = create_array(cols, value);
        row++;
    }
    return array;
}

// insertion sort to sort the integers
void insertion_sort(int *array, int size) {
    int j = 0;
    int i = 1;
    while (i < size) {
        int num = array[i];
        j = i - 1;
        // find the position to insert the number
        while (j >= 0 && array[j] > num) {
            array[j + 1] = array[j];
            j--;
        }
        // insert the number
        array[j + 1] = num;
        i++;
    }
}

// check whether two numbers have an edge
int has_edge(int i, int j) {
    // convert i and j to strings
    char string_i[20];
    sprintf(string_i, "%d", i);
    char string_j[20];
    sprintf(string_j, "%d", j);
    if (strlen(string_i) + 1 == strlen(string_j)) {
        // only one bit difference
        int num = 0;
        int offset = 0;
        for (int k = 0; k < strlen(string_i); k++) {
            if (k + offset >= strlen(string_j)) {
                // more than one bit difference
                return 0;
            }
            if (string_i[k] != string_j[k + offset]) {
                num++;
                k--;
                offset++;
            }
        }
        return num <= 1;
    } else if (strlen(string_i) == strlen(string_j)) {
        // same length
        int n = 0;
        for (int offset = 0; offset < strlen(string_i); offset++) {
            if (string_i[offset] == string_j[offset]) {
                continue;
            }
            n++;
            if (n > 1) {
                // more than one bit is different
                return 0;
            }
        }
        return n == 1;
    } else {
        return 0;
    }
}

// add edges to the graph
void add_edges(int **graph, int *numbers, int n_numbers) {
    for (int i = 0; i < n_numbers; i++) {
        for (int j = 0; j < n_numbers; j++) {
            if (i < j) {
                //  check if there is an edge between the numbers
                graph[i][j] = has_edge(numbers[i], numbers[j]);
            }
        }
    }
}

// output the edges of the graph
void output_edges(int **graph, int *numbers, int n_numbers) {
    for (int row = 0; row < n_numbers; row++) {
        // output the source number
        printf("%d:", numbers[row]);
        for (int col = row; col < n_numbers; col++) {
            if (graph[row][col]) {
                // output the destination number
                printf(" %d", numbers[col]);
            }
        }
        printf("\n");
    }
}

// free a 2d array
void free_2d_array(int **array, int rows) {
    for (int i = 0; i < rows; i++) {
        free_array(array[i]);
    }
    free(array);
}

// free an array
void free_array(int *array) {
    free(array);
}

// floyd-warshall algorithm
int **floyd_warshall(int **graph, int n_numbers, int **pre) {
    int **dis = create_2d_array(n_numbers, n_numbers, INT_MAX);
    // initialize dis and pre
    for (int row = 0; row < n_numbers; row++) {
        for (int col = 0; col < n_numbers; col++) {
            // is connected
            if (graph[row][col]) {
                dis[row][col] = -1;
                pre[row][col] = row;
            } else if (row == col) {
                // same number
                dis[row][col] = 0;
                pre[row][col] = row;
            }
        }
    }
    // floyd-warshall
    for (int k = 0; k < n_numbers; k++) {
        for (int i = 0; i < n_numbers; i++) {
            if (dis[i][k] == INT_MAX) {
                // no edge
                continue;
            }
            for (int j = 0; j < n_numbers; j++) {
                if (dis[k][j] == INT_MAX) {
                    // no edge
                    continue;
                }
                if (dis[i][j] > dis[i][k] + dis[k][j]) {
                    // relax the edge
                    dis[i][j] = dis[i][k] + dis[k][j];
                    pre[i][j] = pre[k][j];
                }
            }
        }
    }

    return dis;
}

// compare two paths
int cmp_path(int *path_a, int *path_b, int length) {
    int i = 0;
    while (i < length) {
        if (path_a[i] < path_b[i]) {
            // first path is smaller
            return -1;
        } else if (path_a[i] > path_b[i]) {
            // second path is smaller
            return 1;
        }
        i++;
    }
    return 0;
}

// output a path
void output_path(int *path, int *numbers, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d", numbers[path[i]]);
        if (i != length - 1) {
            // not the last number
            printf(" -> ");
        }
    }
    printf("\n");
}

// find the longest path
void longest_path(int **graph, int *numbers, int n_numbers, int **pre) {
    int **dis = floyd_warshall(graph, n_numbers, pre);
    int longest = 1;
    int n_paths = 0;
    int id_path = 0;
    int i, j;
    // find the longest path
    for (i = 0; i < n_numbers; i++) {
        for (j = 0; j < n_numbers; j++) {
            if (dis[i][j] < longest) {
                longest = dis[i][j];
            }
        }
    }
    // count the number of longest paths
    for (i = 0; i < n_numbers; i++) {
        for (j = 0; j < n_numbers; j++) {
            if (dis[i][j] == longest) {
                n_paths++;
            }
        }
    }
    int longest_path = longest;
    longest--;
    longest = -longest;
    printf("\nMaximum trail length: %d\nLongest trail(s):\n", longest);
    int **paths = create_2d_array(n_paths, longest, 0);

    // get all the longest paths
    for (i = 0; i < n_numbers; i++) {
        for (j = 0; j < n_numbers; j++) {
            if (dis[i][j] == longest_path) {
                int pre_node = j;
                int inner_id = longest - 1;
                paths[id_path][inner_id] = j;
                while (pre_node != i) {
                    inner_id--;
                    paths[id_path][inner_id] = pre[i][pre_node];
                    pre_node = pre[i][pre_node];
                }
                id_path++;
            }
        }
    }

    // sort the paths
    j = 0;
    i = 1;
    while (i < n_paths) {
        int *path = paths[i];
        j = i - 1;
        while (j >= 0 && cmp_path(paths[j], path, longest) > 0) {
            // swap the paths
            for (int k = 0; k < longest; k++) {
                paths[j + 1][k] = paths[j][k];
            }
            j--;
        }
        // insert the path
        for (int k = 0; k < longest; k++) {
            paths[j + 1][k] = path[k];
        }
        i++;
    }

    // output the paths
    for (i = 0; i < n_paths; i++) {
        output_path(paths[i], numbers, longest);
    }

    // free memory
    free_2d_array(dis, n_numbers);
    free_2d_array(paths, n_paths);
}

int main() {
    int n_numbers;
    printf("Enter a number: ");
    scanf("%d", &n_numbers);
    int *numbers = create_array(n_numbers, 0);
    for (int i = 0; i < n_numbers; i++) {
        printf("Enter a number: ");
        scanf("%d", &numbers[i]);
    }
    // sort the numbers
    insertion_sort(numbers, n_numbers);

    // create a graph
    int **graph = create_2d_array(n_numbers, n_numbers, 0);

    // add edges
    add_edges(graph, numbers, n_numbers);

    // output edges
    printf("\n");
    output_edges(graph, numbers, n_numbers);

    // floyd-warshall
    int **pre = create_2d_array(n_numbers, n_numbers, -1);
    longest_path(graph, numbers, n_numbers, pre);

    // free memory
    free_2d_array(graph, n_numbers);
    free_2d_array(pre, n_numbers);
    free_array(numbers);
}
