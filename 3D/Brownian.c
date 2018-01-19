// Coded by ScratchyCode
// Compile with -lm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// radiant degree
#define BROWNIAN_THETA (2*M_PI)

double realRand(double min, double max);
int randSign();
double average(double array[], long long int N);
double sigma(double array[], long long int N, double average);
int control(void);

int main(){
    long long int i, N;
    double deltaSMax;
    double origin[]={0,0,0};
    
    int gnupl = control();
    if(gnupl == 1){
        printf("\nYou need gnuplot to graph the results.");
        printf("\nInstall it with: sudo apt-get install gnuplot\n\n");
        exit(2);
    }
    
    srand48(time(NULL));
    
    printf("\nEnter the max length of path's moves (meters): ");
    scanf("%lf",&deltaSMax);
    if(deltaSMax <= 0){
        printf("\nThe maximum shift must be greater than zero!");
        exit(1);
    }
    
    printf("Enter the max number of interactions: ");
    scanf("%lld",&N);
    if(N <= 0){
        printf("\nThe number of interactions must be greater than zero!");
        exit(1);
    }
    
    printf("Computing...\n");
    
    // arrays that will contain random moving and angles
    double *deltaS = calloc(N,sizeof(double));
    double *deltaH = calloc(N,sizeof(double));
    double *theta = calloc(N,sizeof(double));
    if(deltaS == NULL || deltaH == NULL || theta == NULL){
        perror("\nError");
        exit(1);
    }
    
    // arrays that will contain the coordinates after each move
    double *vectX = calloc(N,sizeof(double));
    double *vectY = calloc(N,sizeof(double));
    double *vectZ = calloc(N,sizeof(double));
    if(vectX == NULL || vectY == NULL || vectZ == NULL){
        perror("\nError");
        exit(1);
    }
    
    // generating the moving between 0 and the maximum deltaS
    for(i=0; i<N; i++){
        deltaS[i] = realRand(0,deltaSMax);
        deltaH[i] = realRand(0,deltaSMax);
    }
    
    // generating the angles between 0 and 360°
    for(i=0; i<N; i++){
        theta[i] = realRand(0,BROWNIAN_THETA);
    }
    
    // generating coordinates after each move
    for(i=0; i<N; i++){
        vectX[i] = deltaS[i] * cos(theta[i]) + origin[0];
        vectY[i] = deltaS[i] * sin(theta[i]) + origin[1];
        vectZ[i] = (deltaH[i] * randSign()) + origin[2];
        // the new versor of rotation is the previous vector
        origin[0] = vectX[i];
        origin[1] = vectY[i];
        origin[2] = vectZ[i];
    }
    
    // writing results
    FILE *write = fopen("Path.dat","w");
    if(write == NULL){
        perror("\nError");
        exit(1);
    }
    
    for(i=0; i<N; i++){
        fprintf(write,"%lf %lf %lf\n",vectX[i],vectY[i],vectZ[i]);
    }
    
    // calculating traveled distance
    double traveled=0;
    for(i=0; i<N; i++){
        traveled += deltaS[i];
    }
    
    // calculating the shift by taking the last vector coordinates
    double shift = sqrt(pow(vectX[N-1],2) + pow(vectY[N-1],2));
    
    // average shift
    double averageShift = average(deltaS,N);
    double error = sigma(deltaS,N,averageShift);
    
    printf("\n\t***Statistics***\n");
    printf("Traveled space:\t\t%lf m\nShift:\t\t\t%lf m\n",traveled,shift);
    printf("Average shift:\t\t%lf m\nStandard deviation:\t%lf m\n",averageShift,error);
    
    // close the file and deallocate used memory
    fclose(write);
    
    free(deltaS);
    free(deltaH);
    free(theta);
    free(vectX);
    free(vectY);
    free(vectZ);
    
    printf("\n\t***Plotting***\n");
    
    return 0;
}

double realRand(double min, double max){
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (lrand48() / div);
}

int randSign(){
    if(lrand48() % 2 == 0){
        return 1;
    }else{
        return -1;
    }
}

double average(double array[], long long int N){
    long long int i;
    double sum=0;
    
    for(i=0; i<N; i++){
        sum += array[i];
    }
    
    sum = (double)sum/N;
    
    return sum;
}

double sigma(double array[], long long int N, double average){
    long long int i;
    double sigma, sum=0;
    
    for(i=0; i<N; i++){
        sum += pow((array[i] - average),2);
    }
    
    sigma = sqrt(sum/(N-1));
    
    return sigma;
}

int control(void){
    char path[] = "/usr/bin/gnuplot";
    
    FILE *pf = fopen(path,"r");
    if(pf == NULL){
        fclose(pf);
        return 1;
    }else{
        fclose(pf);
        return 0;
    }
}

