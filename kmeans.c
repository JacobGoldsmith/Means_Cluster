#include <stdio.h>
#include <stdlib.h>

void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename);
float **collect(char *input_filename);
int find_cent_ind(float *v, float **centroids);
float *add_vecs(float *v1, float *v2);
int update_centroids(int k, int vec_size, float **centroids, float **sums, int *counts);
float delta_norm_pow2(float *v1, float *v2, int vec_size);
void write_out_file(char *output_filename, float **centroids);


// Main recieves args and runs k_means_cluster with max_it if provided or 200 default
int main(int argc, char *argv[]) {
    if(argc==4){
        k_means_cluster(atoi(argv[0]), atoi(argv[1]), argv[2], argv[3]);
    }
    else{
        k_means_cluster(atoi(argv[0]), 200, argv[1], argv[2]);
    }
    return 0;
}

// k_means_cluster 
void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename){
    int i, j, n, l, vec_size, cent_ind, smaller_than_e;

    float **vectors = collect(input_filename); //DONT FORGET MALLOC
    vec_size = sizeof(vectors[0]);
    n = sizeof(vectors)/vec_size;

    // Allocating memory for centroids and copying first k vectors from vectors in to centroids
    float **centroids = malloc((k)*sizeof(float*));
    assert(centroids!=NULL);

    for (i = 0; i<k; i++){
        centroids[i] = malloc(vec_size*sizeof(float));
        assert(centroids[i]!=NULL);
        for(j = 0; j < vec_size; j++){
            centroids[i][j] = vectors[i][j];
        }
    }


    // Initializing sums and counts and allocating memory with starting values 0
    int *counts = malloc(k*sizeof(int));
    assert(counts!=NULL);
    float **sums = malloc(k * sizeof (float *));
    assert(sums!=NULL);
    for(j = 0; j < k; j++){
        sums[j] = malloc(vec_size*sizeof(float));
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


        for(j = 0; j < n; j++){
            cent_ind = find_cent_ind(vectors[j], centroids);

            // Update sums and counts
            sums[cent_ind] = add_vecs(sums[cent_ind], vectors[j]);
            counts[cent_ind]++;
        }

        smaller_than_e = update_centroids(k, vec_size, centroids, sums, counts);

        if (smaller_than_e){
            break;
        }
        write_out_file(output_filename, centroids);

    }

    // Memory free
    free(counts);
    for(i = 0; i < k; i++){
        free(sums[i]);
    }
    free(sums);
}


// Read file and create array of vectors
float **collect(char *input_filename){
    FILE *ifp = NULL;
    ifp = fopen(input_filename, "r");
    assert(ifp!=NULL);




    fclose(ifp);
}