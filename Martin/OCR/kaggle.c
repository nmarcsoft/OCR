#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#define numberofpixel 785 //nb of rows to save
#define numberofoutput 10
#define numberoftrainingasset 72350
double data[numberoftrainingasset][numberofpixel];
double data_dev[numberofpixel][1000];
double data_train[numberofpixel][71350];
int Y_dev[1000];
double X_dev[784][1000];
int Y_train[71350];
double X_train[784][71350];
double W1[10][784];
double b1[10];
double W2[10][10];
double b2[10];
double Z1[10][71350];
double A1[10][71350];
double A1_t[71350][10];
double Z2[10][71350];
double A2[10][71350];
double one_hot_Y_tmp[71350][10];
double one_hot_Y[10][71350];
double dZ2[10][71350];
int m = 72350;
int n = 785;
long double dW2[10][10];
long double db2;
double Z2exp[10][71350];
double sum_Z2exp[71350];
double W2_t[10][10];
double W2_t_dot[10][71350];
double reluderiv[10][71350];
double dZ1[10][71350];
double dW1[10][784];
long double db1;
double X_T[71350][784];
int predictions[71350];


double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void row_to_line(char w){
    if(w == '0'){
        for(int i = 0; i < 1000; i++){
            for(int j = 0; j < numberofpixel; j++){
                if(j == 0){
                    data_dev[j][i] = data[i][j];
                }
                else{
                    data_dev[j][i] = 0;
                }
            }
        }
        int length = sizeof(data_dev[0])/sizeof(data_dev[0][0]);
        for (int i = 0; i < length; i++) {
        Y_dev[i] = data_dev[0][i];
        }
        for(int i = 1; i < 785; i++){
            for(int j = 0; j < 1000; j++){
                X_dev[i-1][j] = data_dev[i][j];
            }
        }
    }
    else{
        if(w == '1'){
            for(int i = 0; i < 72350; i++){
                for(int j = 0; j < numberofpixel; j++){
                        data_train[j][i] = data[i][j];
                }
            }
            for (int i = 1000; i < 71351; i++) {
            Y_train[i-1000] = data_train[0][i];
            }
            for(int i = 1; i < 785; i++){
                for(int j = 0; j < 71350; j++){
                    X_train[i-1][j] = data_train[i][j+1000];
                }
            }

        }
        else{
        printf("ERROR");
        exit(1);
        }
    }
}

void getInputdata(){ 
    FILE *fpt;
    char c,d;
    int i = -1;
    char str[9999];
    fpt=fopen("train.csv","r"); 
    if(fpt==NULL){
    printf("Can't open file ");
    }
    else {
        int label;
        int lines = -1;
        int rows = 0;
        while(fgets(str,9999,fpt)){ 
            for(int j = 0; j < strlen(str); j++){
                if(str[j] == ','){
                    continue;
                }
                else
                {
                    if(str[j] == 'a'){
                        lines++;
                        rows = 0;
                        j = strlen(str);
                    }
                    else{
                        if(lines != -1){
                            data[lines][rows] = (str[j] - '0');
                            }
                        rows++;
                    }
                }
                    
                }
            }
        }
    fclose(fpt); 
}

void init_params(int v){
    if(v == 0){
        for(int i = 0; i<10; i++){
            for(int j = 0; j < 784; j++){
                W1[i][j] = randfrom(-0.5,0.5);
            }
        }
    }
    if(v == 1){
        for(int i = 0; i < 10; i++){
            b1[i] = randfrom(-0.5,0.5);
        }
    }
    if(v == 2){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                W2[i][j] = randfrom(-0.5,0.5);
            }
        }
    }
    if(v == 3){
        for(int i = 0; i < 10; i++){
            b2[i] = randfrom(-0.5,0.5);
        }
    }
    
}


void dot(int R1, int C2, int R2, int v){
    double hlo;
    double ow;
    if(v == 0){
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                Z1[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = W1[x][k];//M1
                    ow = X_train[k][j];//M2
                    Z1[x][j] += hlo * ow;
                }
            }
        }
    }
    if(v == 1){
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                Z2[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = W2[x][k];
                    ow = A1[k][j];
                    Z2[x][j] += hlo * ow;
                }
            }
        }

    }
    if(v == 2){
        double tmp;
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                dW2[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = dZ2[x][k];
                    ow = A1_t[k][j];
                    tmp = hlo * ow;
                    if(isnan(tmp) || isinf(tmp)){
                        tmp = 0;
                    }
                    dW2[x][j] += tmp;
                }
            }
        }

    }
    if(v == 3){
        double tmp;
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                W2_t_dot[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = W2_t[x][k];
                    ow = dZ2[k][j];
                    tmp = hlo * ow;
                    if(isnan(tmp) || isinf(tmp)){
                        tmp = 0;
                    }
                    W2_t_dot[x][j] += tmp;
                }
            }
        }

    }
    if(v == 4){
        double tmp;
        for(int x = 0; x < R1; x++){
            for(int j = 0; j < C2; j++){
                dW1[x][j] = 0;
                for(int k = 0; k < R2; k++){
                    hlo = dZ1[x][k];
                    ow = X_T[k][j];
                    tmp = hlo * ow;
                    if(isnan(tmp) || isinf(tmp)){
                        tmp = 0;
                    }
                    dW1[x][j] += tmp;
                }
            }
        }

    }


}

void add(int v){
    if(v == 0){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 71350; j++){
                Z1[i][j] += b1[i];
            }
        }
    }
    if(v == 1){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 71350; j++){
                Z2[i][j] += b2[i];
            }
        }
    }

}

void RelU(){
    for(int i=0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            if(Z1[i][j] > 0){
                A1[i][j] = Z1[i][j];
            }
            else{
                A1[i][j] = 0;
            }
        }
    }
}

double sum(int i, int j, int v){
    if(v == 0){
        double sum = 0;
        for (int y = 0; y < 71350; i++) {
            sum += Z2[i][y];
        }
        return sum;
        
    }
    if(v == 1){
        db2 = 0;
        for(int x = 0; x < 10; x++){
            for(int y = 0; y < 71350; y++){
                db2 += dZ2[x][y];
            }
        }
        return 0;
    }
    if(v == 2){
        db1 = 0;
        for(int x = 0; x < 10; x++){
            for(int y = 0; y < 71350; y++){
                db1 += dZ1[x][y];
            }
        }
        return 0;
    }
    if(v == 3){
        double sum;
        for(int i = 0; i < 71350; i++){
            if(predictions[i] == Y_train[i]){
                sum++;
            }
        }
        return sum;
    }

}

void softmax(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            Z2exp[i][j] = exp(Z2[i][j]);
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            sum_Z2exp[j] = 0;
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            sum_Z2exp[j] += Z2exp[i][j];
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            A2[i][j] = exp(Z2[i][j]) / sum_Z2exp[j];
        }
    }
}
void forward_prop(){
    dot(10, 71350, 784, 0); //Z1 = W1.dot(X)
    add(0);
    RelU();
    dot(10, 71350, 10, 1);
    add(1);
    softmax();
}

void one_hot(){
    for(int i = 0; i < 71350; i++){
        for(int j = 0; j < 10; j++){
            one_hot_Y_tmp[i][j] = 0;
        }
    }
    int toreplace;
    for(int i = 0; i < 71350; i++){
       toreplace = Y_train[i];
       one_hot_Y_tmp[i][toreplace] = 1;
    }
    for(int i = 0; i < 71350; i++){
        for(int j = 0; j < 10; j++){
            one_hot_Y[j][i] = one_hot_Y_tmp[i][j];
        }
    }
}

void divide(double k, int v){
    if(v == 0){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                dW2[i][j] /= k;
            } 
        }
    }
    if(v == 1){
        db2 /= k;
    }
    if(v == 2){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 784; j++){
                dW1[i][j] /= k;
            }
        }
    }
    if(v == 3){
        db1 /=k;
    }
}

void transform_A1(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            A1_t[j][i] = A1[i][j];
        }
    }
}

void transform_W2(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            W2_t[j][i] = W2[i][j];
        }
    }
}

void ReLU_deriv(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            if(Z1[i][j] > 0){
                reluderiv[i][j] = 1;
            }
            else{
                reluderiv[i][j] = 0;
            }
        }
    }
}

void transform_X(){
    for(int i = 0; i < 784; i++){
        for(int j = 0; j < 71350; j++){
            X_T[j][i] = X_train[i][j];
        } 
    }
}

void backward_prop(){
    one_hot();
    //Dz2
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            dZ2[i][j] = A2[i][j] - one_hot_Y[i][j];
        }
    }
    transform_A1();
    dot(10, 10, 71350, 2);
    divide(m, 0);
    sum(0,0,1);
    divide(m,1);
    db2 = db2 -1.468116633553845e-15;
    transform_W2();
    dot(10, 71350, 10, 3);
    ReLU_deriv();
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 71350; j++){
            dZ1[i][j] = W2_t_dot[i][j] * reluderiv[i][j];
        }
    }
    transform_X();
    dot(10, 784, 71350, 4);
    divide(m,2);
    sum(0,0,2);
    divide(m, 3);
}


void update_w1(double learning_rate){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 784; j++){
            W1[i][j] = W1[i][j] - (learning_rate* dW1[i][j]);
        }
    }
}

void update_b1(double learning_rate){
    for(int i = 0; i < 10; i++){
        b1[i] = b1[i] - (learning_rate * db1);
    }
}

void update_w2(double learning_rate){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            W2[i][j] = W2[i][j] - (learning_rate* dW2[i][j]);
        }
    }
}


void update_b2(double learning_rate){
    for(int i = 0; i < 10; i++){
        b2[i] = b2[i] - (learning_rate * db2);
    }
}

void update_params(double learning_rate){
    update_w1(learning_rate);
    update_b1(learning_rate);
    update_w2(learning_rate);
    update_b2(learning_rate);
}


void get_prediction(){
    double maxi = 0;
    int max_indice = -1;
    for(int j =0; j < 71350; j++){
        for(int i = 0; i < 10; i++){
            if(maxi < A2[i][j]){
                maxi = A2[i][j];
                max_indice = i;
            }
        }
        maxi = 0;
        predictions[j] = max_indice;
    }    
}

double get_accuracy(){
    get_prediction();
    double accu;
    accu = sum(0,0,3);
    return accu / 71350;
}

void init_W1(){
    size_t len = 0;
    FILE *fp;
    char *line;
    ssize_t read;
    fp = fopen("W1.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        for(int j = 0; j < 784; j++){
            read = getline(&line, &len, fp);
            W1[i][j] = strtod(line, NULL);
        }
    }
    fclose(fp);
}

void init_W2(){
    size_t len = 0;
    FILE *fp;
    char *line;
    ssize_t read;
    fp = fopen("W2.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        for(int j = 0; j < 10; j++){
            read = getline(&line, &len, fp);
            W2[i][j] = strtod(line, NULL);
        }
    }
    fclose(fp);
}

void init_b1_b2(){
    size_t len = 0;
    FILE *fp;
    char *line;
    ssize_t read;
    fp = fopen("b1.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        read = getline(&line, &len, fp);
        b1[i] = strtod(line, NULL);
    }
    fclose(fp);
    fp = fopen("b2.txt", "r");
    if(fp == NULL){
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <10; i++){
        read = getline(&line, &len, fp);
        b2[i] = strtod(line, NULL);
    }
    fclose(fp);

}

void gradient_descent(double learning_rate, int epoch){
    init_params(0);//Init W1
    init_params(1);//init b1
    init_params(2);//init W2
    init_params(3);//init b2
    //init_W1();
    //init_W2();
    //init_b1_b2();
    double accu;
    for(int i = 0; i < epoch; i++){
        forward_prop();
        backward_prop();
        update_params(learning_rate);
        printf("EPOCH : %i, ACCURACY : %lf \n", i, get_accuracy());
    }
}

void save(){
    FILE *f = fopen("save.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    for(int i =0; i < 10; i++){
        for(int j = 0; j < 784; j++){
            fprintf(f, "%lf\n", W1[i][j]);
        }
    }
    for(int i = 0; i < 10; i++){
        fprintf(f, "%lf\n", b1[i]);
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            fprintf(f, "%lf\n", W2[i][j]);
        }
    }
    for(int i = 0; i < 10; i++){
        fprintf(f, "%lf\n", b2[i]);
    }
    fclose(f); 
 

}

int main(){
    getInputdata();
    row_to_line('0');
    row_to_line('1');
    gradient_descent(0.1, 500);
    save();
    return 0;
}
