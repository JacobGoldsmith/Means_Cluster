#include <stdio.h>
#include <stdlib.h>
#include <cassert>

void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename);
double **collect(char *input_filename, int sum_vectors, int vector_size);
int find_cent_ind(double *v, double **centroids, int k, int vec_size);
//double *add_vecs(double *v1, double *v2);
//int update_centroids(int k, int vec_size, double **centroids, double **sums, int *counts);
double delta_norm_pow2(double *v1, double *v2, int vec_size);
//void write_out_file(char *output_filename, double **centroids);
void find_lengths_and_amount(char *input_filename, int *size_vec_amount_vecs);

// Main recieves args and runs k_means_cluster with max_it if provided or 200 default
int main(int argc, char *argv[]) {
    if(argc==5){
        k_means_cluster(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
    }
    else{
        k_means_cluster(atoi(argv[1]), 200, argv[2], argv[3]);
    }
    return 0;
}


// k_means_cluster 
void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename){
    int i, j, l, vec_size, sum_vecs, cent_ind, smaller_than_e;
    int *size_vec_amount_vecs = (int*)malloc(2*sizeof(int));
    
    // Finding length of vectors and amount of vectors
    find_lengths_and_amount(input_filename, size_vec_amount_vecs);
    vec_size = size_vec_amount_vecs[0];
    sum_vecs =  size_vec_amount_vecs[1];

    printf("this is out file name %s \n", output_filename);
    printf("k = %d\nvecsize = %d\nsumvecs = %d\n", k, vec_size, sum_vecs);

    // Reading vectors from file
    double **vectors = collect(input_filename, vec_size, sum_vecs); 
    
    // Allocating memory for centroids and copying first k vectors from vectors in to centroids
    double **centroids = (double**)malloc((k)*sizeof(double*));
    assert(centroids!=NULL);

    for (i = 0; i<k; i++){
        centroids[i] = (double*)malloc(vec_size*sizeof(double));
        assert(centroids[i]!=NULL);
        for(j = 0; j < vec_size; j++){
            printf("%f ", vectors[i][j]);
            centroids[i][j] = vectors[i][j];
        }
        printf("\n");
    }


    // Initializing sums and counts and allocating memory with starting values 0
    int *counts = (int*)malloc(k*sizeof(int));
    assert(counts!=NULL);
    double **sums = (double**)malloc(k * sizeof (double *));
    assert(sums!=NULL);
    for(j = 0; j < k; j++){
        sums[j] = (double*)malloc(vec_size*sizeof(double));
        assert(sums[j]!=NULL);
    }
    // Cluster dividing
    for(i = 0; i < max_it; i++){

        // Initialize lists to calculate new centroids later
        for(j = 0; j < k; j++){
            counts[j] = 0;
            for(l = 0; l < vec_size; l++){
                sums[j][l] = 0;
            }
        }


        for(j = 0; j < sum_vecs; j++){
            cent_ind = find_cent_ind(vectors[j], centroids, k, vec_size);

            // Update sums and counts
            //sums[cent_ind] = add_vecs(sums[cent_ind], vectors[j]);
            counts[cent_ind]++;
        }

        smaller_than_e = 1;//update_centroids(k, vec_size, centroids, sums, counts);

        if (smaller_than_e){
           break;
        }
        //write_out_file(output_filename, centroids);

    }

    // Memory free
    free(counts);
    for(i = 0; i < k; i++){
        free(sums[i]);
    }
    free(sums);

    for(i = 0; i < sum_vecs; i++){
        free(vectors[i]);
    }
    free(vectors);
}


// Read file and create array of vectors
double **collect(char *input_filename, int sum_vectors, int vector_size){
    int i = 0, j = 0;
    float tmp;

    FILE *ifp = NULL;
    ifp = fopen(input_filename, "r");
    assert(ifp!=NULL);

    // Allocating memory for array of vectors
    double **vectors = (double**)malloc(sum_vectors*sizeof(double*));
    assert(vectors!=NULL);
    for (i = 0; i < sum_vectors; i++){
        vectors[i] = (double*)malloc(vector_size*sizeof(double));
        assert(vectors[i]!=NULL);
    }

    // Load file as doubles in to array vectors
    for (i = 0; i < sum_vectors; i++){
        for (j = 0; j < vector_size; j++){
            fscanf(ifp, "%f", &tmp);
            vectors[i][j] = (double)tmp;
            fgetc(ifp);
        }
    }

    fclose(ifp);

    return vectors;
}

void find_lengths_and_amount(char *input_filename, int *size_vec_amount_vecs){
    
    int sum_vectors = 0, sum_cords = 0;
    char c;
    
    FILE *ifp = NULL;
    ifp = fopen(input_filename, "r");
    assert(ifp!=NULL);

    // Finding size of vector and amount of vectors
    while ( ( c = fgetc( ifp ) ) != EOF ) {
        if ( c == '\n' ){
            sum_vectors++;
            sum_cords++;
        }
        else if (c == ',')
        {
            sum_cords++;
        }
    }
    fclose(ifp);

    size_vec_amount_vecs[0] = sum_cords/sum_vectors;
    size_vec_amount_vecs[1] = sum_vectors;
}

double delta_norm_pow2(double *v1, double *v2, int vec_size){
    double sum = 0;
    int i;
    for (i = 0; i < vec_size; i++){
        sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
    }
    return sum;
}


int find_cent_ind(double *v, double **centroids, int k, int vec_size){
    int min_ind = 0;
    int i;
    double delt;
    double min_delta = delta_norm_pow2(v, centroids[0], vec_size);
    for (i = 1; i < k; i++){
        delt = delta_norm_pow2(v, centroids[i], vec_size);
        if (delt < min_delta){
            min_delta = delt;
            min_ind = i;
        }
    }
    return min_ind;
}