#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef double* vector;

struct cluster{
    int amount;
    vector sum;
};

typedef struct cluster cluster;

int get_vector_dim_from_file(){
    int dim = 0;
    double val;
    char c;
    while(scanf("%lf%c", &val, &c) == 2 && c != '\n'){
        dim++;
    }
    dim++;
    rewind(stdin);
    return dim;
}

int get_num_of_vectors_from_file(int dim){
    int vectors_num = 0;
    double val;
    char c;
    while(scanf("%lf%c", &val, &c) == 2){
        if(c == ',') {
            vectors_num++;
        }
    }
    vectors_num = vectors_num/(dim-1);
    rewind(stdin);
    return vectors_num;
}

vector* read_vectors_from_file(int dim, int vectors_num){
    double val;
    char c;
    int i, j;
    vector* vectors_pointer = (vector*) malloc(vectors_num * sizeof(vector));
    assert (vectors_pointer != NULL);
    for (i = 0; i < vectors_num; i++) {
        vectors_pointer[i] = (vector) malloc(dim * sizeof(double));
        assert (vectors_pointer[i] != NULL);
        j = 0;
        while(scanf("%lf%c", &val, &c) == 2 && c != '\n'){
            vectors_pointer[i][j] = val;
            j++;
        }
        vectors_pointer[i][j] = val;
    }
    return vectors_pointer;
}

vector* init_centroids(vector* vectors, int dim, int k){
    int i, j;
    vector* centroids_pointer = (vector*) malloc(k * sizeof(vector));
    assert (centroids_pointer != NULL);
    for (i = 0; i < k; i++) {
        centroids_pointer[i] = (vector) malloc(dim * sizeof(double));
        assert (centroids_pointer[i] != NULL);
        for (j = 0; j < dim; j++) {
            centroids_pointer[i][j] = vectors[i][j];
        }
    }
    return centroids_pointer;
}

vector* average_vector(vector* vectors, int dim, int n){
    int i, j, k;
    vector* centroids_pointer = init_centroids(vectors, dim, 1);
    for (i = 1; i < n; ++i) {
        for (j = 0; j < dim; ++j) {
            centroids_pointer[0][j] += vectors[i][j];
        }
    }
    for (k = 0; k < dim; ++k) {
        centroids_pointer[0][k] = centroids_pointer[0][k]/n;
    }
    return centroids_pointer;
}

double distance_sq(vector v1, vector v2, int dim){
    double distance = 0;
    int i;
    for (i = 0; i < dim; i++) {
        double diff = (v1[i]-v2[i])*(v1[i]-v2[i]);
        distance += diff;
    }
    return distance;
}

cluster* init_clusters(int dim, int k){
    cluster* clusters_pointer;
    int i, j;
    clusters_pointer = (cluster*) malloc(k * sizeof(cluster));
    assert (clusters_pointer != NULL);
    for (i = 0; i < k; i++) {
        cluster c;
        c.amount = 0;
        c.sum = (vector) malloc(dim * sizeof(double));
        assert (c.sum != NULL);
        for (j = 0; j < dim; j++) {
            c.sum[j] = 0;
        }
        clusters_pointer[i] = c;
    }
    return clusters_pointer;
}

void reset_clusters(struct cluster* clusters, int dim, int k){
    int i, j;
    for (i = 0; i < k; i++) {
        clusters[i].amount = 0;
        for (j = 0; j < dim; j++) {
            clusters[i].sum[j] = 0;
        }
    }
}

void print_vector(vector v, int dim){
    int i;
    for (i=0; i < dim - 1; i++){
        printf("%.4f,", v[i]);
    }
    printf("%.4f\n",v[dim-1]);
}

void calculate_clusters(vector* vectors, vector* centroids, struct cluster* clusters, int dim, int k, int vectors_num){
    int l, i, j;
    for (l = 0; l < vectors_num; l++) {
        double min_distance = distance_sq(vectors[l],centroids[0], dim);
        int closest_cluster = 0;
        for(i=1; i<k; i++) {
            double distance = distance_sq(vectors[l], centroids[i], dim);
            if (distance < min_distance) {
                min_distance = distance;
                closest_cluster = i;
            }
        }
        clusters[closest_cluster].amount += 1;
        for (j=0; j<dim; j++){
            clusters[closest_cluster].sum[j] += vectors[l][j];
        }
    }
}

int update_centroids(vector* centroids, cluster* clusters, int dim, int k){
    int i, j, changed = 0;
    double new_cor;
    struct cluster curr_cluster;
    for (i = 0; i < k; i++) {
        for (j = 0; j < dim; j++) {
            curr_cluster = clusters[i];
            new_cor = curr_cluster.sum[j] / curr_cluster.amount;
            if (centroids[i][j] != new_cor) {
                changed = 1;
                centroids[i][j] = new_cor;
            }
        }
    }
    return changed;
}

void free_vectors(vector* vectors, int vectors_num){
    int i;
    for(i=0; i<vectors_num; i++){
        free(vectors[i]);
    }
    free(vectors);
}

void free_clusters(cluster* clusters, int k){
    int i;
    for(i=0; i<k; i++) {
        free(clusters[i].sum);
    }
    free(clusters);
}

void free_all(vector* vectors, vector* centroids, cluster* clusters, int vectors_num, int k){
    free_vectors(vectors, vectors_num);
    free_vectors(centroids, k);
    if (k != 1){
        free_clusters(clusters, k);
    }
}

int main(int argc, char* argv[]) {
    int i, iter_num, centroids_changed, max_iter, k_val;
    int dimension = get_vector_dim_from_file();
    int v_num = get_num_of_vectors_from_file(dimension);
    double k_check, max_iter_check;
    cluster* clusters_pointer;
    vector* vectors_pointer;
    vector* centroids_pointer;
    assert("Invalid number of arguments" && argc > 1 && argc < 4);
    k_val = strtol(argv[1], NULL, 10);
    k_check = strtod(argv[1], NULL);
    assert("Invalid k" && (double)k_val-k_check == 0.0);
    max_iter = 200;
    if(argc == 3){
        max_iter = strtol(argv[2], NULL, 10);
        max_iter_check = strtod(argv[2], NULL);
        assert("Invalid number of iterations" && (double)max_iter-max_iter_check == 0.0);
    }
    assert("Invalid k" && k_val > 0 && k_val < v_num);
    assert("Invalid number of iterations" && max_iter > 0);
    vectors_pointer = read_vectors_from_file(dimension, v_num);
    if (k_val != 1){
        clusters_pointer = init_clusters(dimension, k_val);
        centroids_pointer = init_centroids(vectors_pointer,dimension,k_val);
        iter_num = 0;
        centroids_changed = 1;
        while((iter_num < max_iter) & centroids_changed){
            calculate_clusters(vectors_pointer, centroids_pointer, clusters_pointer, dimension, k_val, v_num);
            centroids_changed = update_centroids(centroids_pointer, clusters_pointer, dimension, k_val);
            reset_clusters(clusters_pointer, dimension, k_val);
            iter_num += 1;
        }
    }
    else{
        centroids_pointer = average_vector(vectors_pointer, dimension, v_num);
    }
    for(i=0; i<k_val; i++){
        print_vector(centroids_pointer[i], dimension);
    }
    free_all(vectors_pointer,centroids_pointer,clusters_pointer,v_num,k_val);
    return 0;
}