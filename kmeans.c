#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename);
double **collect(char *input_filename, int sum_vectors, int vec_size);
int find_cent_ind(double *v, double **centroids, int k, int vec_size);
double *add_vecs(double *v1, double *v2, int vec_size);
int update_centroids(int k, int vec_size, double **centroids, double **sums, int *counts);
double delta_norm_pow2(double *v1, double *v2, int vec_size); /* vec_size has been added to arguments */
void write_out_file(char *output_filename, double **centroids, int k, int vec_size); /* k and vec_size have been added to arguments*/
void find_lengths_and_amount(char *input_filename, int *size_vec_amount_vecs);
int is_number(char *c);

int main(int argc, char *argv[]) {
    if(argc==5){
        if(!is_number(argv[1])||!is_number(argv[2])){
            printf("Invalid Input!");
            exit(1);
        }
        k_means_cluster(atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
        exit(0);
    }
    else if(argc==4){
        if(!is_number(argv[1])){
            printf("Invalid Input!");
            exit(1);
        }
        k_means_cluster(atoi(argv[1]), 200, argv[2], argv[3]);
        exit(0);
    }
    else{
        printf("Invalid Input!");
        exit(1);
    }
}

/* k_means_cluster */
void k_means_cluster(int k, int max_it, char *input_filename, char *output_filename){
    int i, j, l, vec_size, sum_vecs, cent_ind, smaller_than_e;
    int *size_vec_amount_vecs = (int*)malloc(2*sizeof(int));
    double **sums;
    double **vectors;
    double **centroids;
    int *counts;
    
    /* Finding length of vectors and amount of vectors*/
    find_lengths_and_amount(input_filename, size_vec_amount_vecs);
    vec_size = size_vec_amount_vecs[0];
    sum_vecs =  size_vec_amount_vecs[1];
    
    /*check if not enough vectors for clusters*/
    if(sum_vecs < k){
        printf("An Error Has Occurred");
        exit(1);
        }

    /* Reading vectors from file*/
    vectors = collect(input_filename, sum_vecs, vec_size); 

    /* Allocating memory for centroids and copying first k vectors from vectors in to centroids*/
    centroids = (double**)malloc((k)*sizeof(double*));
    if(centroids==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }

    for (i = 0; i < k; i++){
        centroids[i] = (double*)malloc(vec_size*sizeof(double));
        if(centroids[i]==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
        for(j = 0; j < vec_size; j++){
            centroids[i][j] = vectors[i][j];
        }
    }
    
    

    /* Initializing sums and counts and allocating memory with starting values 0*/
    counts = (int*)malloc(k*sizeof(int));
    if(counts==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
    sums = (double**)malloc(k * sizeof (double *));
    if(sums==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
    for(j = 0; j < k; j++){
        sums[j] = (double*)malloc(vec_size*sizeof(double));
        if(sums[j]==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
    }


    /* K-means Cluster Dividing*/
    for(i = 0; i < max_it; i++){
        /* Initialize lists to calculate new centroids later*/
        for(j = 0; j < k; j++){
            counts[j] = 0;
            for(l = 0; l < vec_size; l++){
                sums[j][l] = 0;
            }
        }
        
        for(j = 0; j < sum_vecs; j++){
            cent_ind = find_cent_ind(vectors[j], centroids, k, vec_size);

            /* Update sums and counts*/
            sums[cent_ind] = add_vecs(sums[cent_ind], vectors[j], vec_size);
            counts[cent_ind]++;
        }

        smaller_than_e = update_centroids(k, vec_size, centroids, sums, counts);
        if (smaller_than_e){
           break;
        } 
    }

    write_out_file(output_filename, centroids, k, vec_size);
    
    /* Memory free*/
    for(i = 0; i < k; i++){
        free(centroids[i]);
    }
    free(centroids);

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

void find_lengths_and_amount(char *input_filename, int *size_vec_amount_vecs){

    int sum_vectors = 0, sum_cords = 0;
    char c;

    FILE *ifp = NULL;
    ifp = fopen(input_filename, "r");
    if(ifp==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }

    /* Finding size of vector and amount of vectors*/
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

/* Read file and create array of vectors*/
double **collect(char *input_filename, int sum_vectors, int vec_size){
    int i = 0, j = 0;
    double tmp;
    double **vectors;

    FILE *ifp = NULL;
    ifp = fopen(input_filename, "r");
    if(ifp==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }

    /* Allocating memory for array of vectors*/
    vectors = (double**)malloc(sum_vectors*sizeof(double*));
    if(vectors==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
    for (i = 0; i < sum_vectors; i++){
        vectors[i] = (double*)malloc(vec_size*sizeof(double));
        if(vectors[i]==NULL){
        printf("An Error Has Occurred");
        exit(1);
        }
    }

    /* Load file as doubles in to array vectors*/
    for (i = 0; i < sum_vectors; i++){
        for (j = 0; j < vec_size; j++){
            fscanf(ifp, "%lf", &tmp);
            vectors[i][j] = tmp;
            fgetc(ifp);
        }
    }

    fclose(ifp);

    return vectors;
}

double delta_norm_pow2(double *v1, double *v2, int vec_size){
    double sum = 0;
    int p;
    for (p = 0; p < vec_size; p++){
        sum += (v1[p]-v2[p])*(v1[p]-v2[p]);
    }
    return sum;
}


int find_cent_ind(double *v, double **centroids, int k, int vec_size){
    int min_ind = 0;
    int ind;
    double delt;
    double min_delta = delta_norm_pow2(v, centroids[0], vec_size);
    for (ind = 0; ind < k; ind++){
        delt = delta_norm_pow2(v, centroids[ind], vec_size);
        if (delt < min_delta){
            min_delta = delt;
            min_ind = ind;
        }
    }
    return min_ind;
}

int update_centroids(int k, int vec_size, double **centroids, double **sums, int *counts) {
    int smaller_than_e = 1; /*true = 1, false = 0*/
    int i,j,index;
    double *prev_cent;
    double delta_norm;
    prev_cent = (double*)malloc(vec_size*sizeof(double)); /* Allocating space for 1d array of a vector*/

    /*update all k centroids*/
    for (i = 0; i < k; i++) {
        for (index = 0; index < vec_size; index++){/* want to make copy*/
            prev_cent[index] = centroids[i][index];
        } 
        for (j = 0; j < vec_size; j++) {
            centroids[i][j] = sums[i][j] / counts[i];
        }

        /*update smaller_than_e*/
        delta_norm = pow(delta_norm_pow2(prev_cent, centroids[i], vec_size), 0.5);

        if (delta_norm >= 0.001) { /***********create constant "epsilon"!!!!!!****************/
            smaller_than_e = 0;
        }
    }
    free(prev_cent);
    return smaller_than_e;
}

double *add_vecs(double *v1, double *v2, int vec_size) {
    int i;
    double *res = (double*)malloc(vec_size*sizeof(double ));

    for (i = 0; i < vec_size; i++) {
        res[i] = v1[i] + v2[i];
    }

    return res;
}

void write_out_file(char *output_filename, double **centroids, int k, int vec_size) {
    FILE *outfile = NULL;
    int i, j;
    /* create file to write result to */
    outfile = fopen(output_filename, "w");

    /*print all k centroids*/
    for (i = 0; i < k; i++) { /**** why was the default "++i"??? ********/
        /* print centroid[i]*/
        for (j = 0; j < vec_size; j++) {
            fprintf(outfile, "%.4f", centroids[i][j]);

            if (j < vec_size - 1) { /* separate with a comma */
                fprintf(outfile, "%s", ",");
            }
        }
        fprintf(outfile, "\n"); /* separate centroids with new line*/
    }

    /* close file */
    fclose(outfile);
}

/* Checks if string c is only digits*/
int is_number(char *c){
    int index = 0;
    while(c[index] != 0){
        if(c[index] < 48 || c[index] > 57){
            return 0;
        }
        index++;
    }
    return 1;
}
