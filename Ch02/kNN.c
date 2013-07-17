#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include "kNN.h"

#define MAX_LINE 1000
#define MAX_BUF  50

double groups[4][2] = {{1.0,1.1},{1.0,1.0},{0,0},{0,0.1}};
char labels[4] = {'A','A','B','B'};
Evaluate_Items evt_items[MAX_LINE];

evaluate_items norm_min={
    .flymiles = 0.0,
    .playvideogame = 0.0,
    .cameraconsumption = 0.0,
    .classlabel = -1
};

evaluate_items norm_max={
    .flymiles = 0.0,
    .playvideogame = 0.0,
    .cameraconsumption = 0.0,
    .classlabel = -1
};


//Find the index of samllest element in the array
char knn_find_class(double* array, int k){
    int i, j;
    double temp;
    char mlabels[4];
    char c;
    int classA=0;
    int classB=0;
    char ret = 'B';
    memcpy(mlabels, labels, 4);
    //sort distance array, then change the order of labels array accrodingly.
    for(i=0; i<4; i++){
        for(j=i+1; j < 4; j++){
        if( array[i] > array[j] ){
            temp = array[j];
            c    = mlabels[j];
            array[j]   = array[i];
            mlabels[j] = mlabels[i]; 
            array[i]   = temp;
            mlabels[i] = c;
            }
        }
    }
    //verify
    //for(i=0; i<4; i++)
    //    printf("array[%d] = %f\t mlabels[%d] = %c\n", i, array[i], i, mlabels[i]);
    //claculate the number of each class within range k, then found the class 
    for(i=0; i<k; i++)
    {
        if(mlabels[i] == 'A')
            classA++;
        else
            classB++;
    }
    //verify
    //printf("A=%d\tB=%d\n", classA, classB);
    if(classA > classB)
        ret = 'A';
    return ret;
}

void classify0(double* point, double** dataset, char* labs, int k){
    int i;
    double distance[4];
    memset(distance, 0, sizeof(double));
    if(point == NULL)
        return ;
    for(i=0; i<4; i++){
        distance[i] = sqrt(pow((groups[i][0] - point[0]), 2) + pow((groups[i][1]-point[1]),2));
    }
    //verify
    //for(i=0; i<4; i++)
    //    printf("distance[%d] = %f\n", i, distance[i]);
    printf("class = %c\n", knn_find_class(distance, k));
}

int knn_find_class1(double* dist, int* labs, int dist_len, int k){
    int i, j;
    double temp;
    int c;
    int class[3] = {0,0,0}; //class[0] - didntLike, [1] - smallDoses, [2] - largeDoses
    int ret = -1;
    //sort distance array, then change the order of labels array accrodingly.
    for(i=0; i<dist_len; i++){
        for(j=i+1; j < dist_len; j++){
        if( dist[i] > dist[j] ){
            temp = dist[j];
            c    = labs[j];
            dist[j]   = dist[i];
            labs[j] = labs[i]; 
            dist[i]   = temp;
            labs[i] = c;
            }
        }
    }
    //claculate the number of each class within range k, then found the class 
    for(i=0; i<k; i++)
    {
        if(labs[i] == 1)  //didntLike
            class[0]++;
        else if(labs[i] == 2)  //smallDoses
            class[1]++;
        else if(labs[i] == 3)  //largeDoses
            class[2]++;
        else
           printf("%s : Error count for class = labs[%d] = %d\n", __FUNCTION__, i, labs[i]);
    }
    //find the class with the maximum value
    int p = 0;
    for (i=1; i<3; i++){
        if (class[i] > class[p])
            p = i;
    }
    ret = p+1;
    return ret;
}

int classify1(Evaluate_Items input, Evaluate_Items* dataset, int dataset_len, int k){
    int i, ret;
    double distances[dataset_len];
    int mlabels[dataset_len];
    if(input == NULL)
        return;
    for(i=0;i<dataset_len;i++){
        /*printf("input:\n");
        printf("[ %.8lf %.8lf  %.8lf]\n", input->flymiles,input->playvideogame,input->cameraconsumption);
        printf("dataset:\n");
        printf("[ %.8lf %.8lf  %.8lf]\n", dataset[i]->flymiles,dataset[i]->playvideogame,dataset[i]->cameraconsumption);
        printf("distance:\n");
        printf("[ %.8lf %.8lf  %.8lf]\n", input->flymiles - dataset[i]->flymiles,input->playvideogame - dataset[i]->playvideogame,input->cameraconsumption - dataset[i]->cameraconsumption);*/
        distances[i] = sqrt(pow(input->flymiles - dataset[i]->flymiles,2)  
                          + pow(input->playvideogame - dataset[i]->playvideogame,2) 
                          + pow(input->cameraconsumption - dataset[i]->cameraconsumption,2));        
        mlabels[i] = dataset[i]->classlabel;
    }
    ret = knn_find_class1(distances, mlabels, dataset_len, k);
    return ret;
}

int knn_find_class2(double* dist, int* labs, int dist_len, int k){
    int ret = -1;
    return ret;
}

void quick_sort(training_item_ptr* dataset, int low, int high){
    training_item_ptr temp;
    int pivot,j,i;
    if(low<high)
    {
        pivot = low;
        i = low;
        j = high;

        while(i<j){
            while((dataset[i]->distance_from_test<=dataset[pivot]->distance_from_test)&&(i<high)){
                i++;
            }

            while(dataset[j]->distance_from_test>dataset[pivot]->distance_from_test){
                j--;
            }

            if(i<j){
                temp=dataset[i];
                dataset[i]=dataset[j];
                dataset[j]=temp;
            }
        }
        temp=dataset[pivot];
        dataset[pivot]=dataset[j];
        dataset[j]=temp;
        quick_sort(dataset,low,j-1);
        quick_sort(dataset,j+1,high);
    }

}

int NumberOfSetBits(unsigned int i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

double count_distance(unsigned int* test, unsigned int* data, int len){
    int sum = 0, i;
    unsigned int diff;
    for(i=0;i<len;i++){
        diff = test[i]^data[i];
        sum += NumberOfSetBits(diff);
    }
    return sqrt(sum);
}

int classify2(training_item_ptr test_item, training_item_ptr* dataset, const int ds_len, int k){
    int class[10];
    int i, ret = -1;
    int e_height = sizeof(test_item->data)/sizeof(unsigned int);
    memset( class, 0, 10*sizeof(int) );
    for(i=0;i<ds_len;i++){
       dataset[i]->distance_from_test = count_distance(test_item->data, dataset[i]->data, e_height);
    }
    quick_sort(dataset,0, ds_len-1);
    for(i=0; i<k; i++){
       class[dataset[i]->hwLabel] += 1; 
    }
    //find the class with the maximum value
    int p = 0;
    for (i=1; i<10; i++){
        if (class[i] > class[p])
            p = i;
    }
    ret = p;
    return ret;
}

void file2matrix_helper(char** parsed_data, int current_line){
    const char* eval_result[3] = { "didntLike", "smallDoses", "largeDoses" };
    Evaluate_Items eval_item = (Evaluate_Items)malloc(sizeof(struct ev_items));
    eval_item->flymiles = atof(parsed_data[0]);
    eval_item->playvideogame = atof(parsed_data[1]);
    eval_item->cameraconsumption = atof(parsed_data[2]);
    int i=0;
    if(0==isdigit(parsed_data[3][0])){
        for(i=0; i<3; i++){
            if(0==strcmp(parsed_data[3], eval_result[i]))
                eval_item->classlabel = (i+1);
        }
    }else{
        eval_item->classlabel = atoi(parsed_data[3]);
    }
    evt_items[current_line-1]=eval_item;
}

void file2matrix(char* filename){
    FILE *fp;
    char buf[MAX_BUF]; 
    char *token;
    char* field[4];  //four fields of each line
    int current_line = 0;

    fp=fopen(filename,"r");
    if(fp!=NULL){
        while(fgets(buf,sizeof(buf),fp)){
            int token_idx = 0;
            strtok(buf, "\n");
            token = strtok(buf, "\t");
            while(token != NULL){
                field[token_idx] = token;
                token_idx++;
                token = strtok(NULL, "\t");   //next token
            }
            current_line++;
            file2matrix_helper(field, current_line);
        }
    }else{
        printf("Cannot access the file!");
    }
    fclose(fp);
}


int autoNorm_helper(const double* array, Prefer p){
    double min, max, range;
    double newValue;
    int i, ret = 0;
    min = max = array[0];
    for(i=0; i<MAX_LINE; i++){
        if(max < array[i])    max = array[i];
        if(min > array[i])    min = array[i];     
   } 
    range = max-min;
    for(i=0; i<MAX_LINE; i++){
        newValue = (array[i]-min)/range;
        switch(p){
        case FLYMILES:
            norm_min.flymiles = min;
            norm_max.flymiles = max;
            evt_items[i]->flymiles = newValue;
        break;
        case VIDEOGAME:
            norm_min.playvideogame = min;
            norm_max.playvideogame = max;
            evt_items[i]->playvideogame = newValue;
        break;
        case ICECREAM:
            norm_min.cameraconsumption = min;
            norm_max.cameraconsumption = max;
            evt_items[i]->cameraconsumption = newValue;
        break;
        default:
            printf(" %s : fail out!\n", __FUNCTION__);
            ret = -1;
        }
    }
    return ret;    
}

void autoNorm(void){
    double fly_miles[MAX_LINE];
    double play_videogame[MAX_LINE];
    double cream_consumption[MAX_LINE];
    int i;
    for(i=0;i<MAX_LINE;i++){
        fly_miles[i] = evt_items[i]->flymiles;
        play_videogame[i] = evt_items[i]->playvideogame;
        cream_consumption[i] = evt_items[i]->cameraconsumption;
    }
    autoNorm_helper(fly_miles, FLYMILES);
    autoNorm_helper(play_videogame, VIDEOGAME);
    autoNorm_helper(cream_consumption, ICECREAM);
}

int create_dataset(Evaluate_Items* dataset, int start, int end){
    int ret = -1;
    int i,newstart = start;
    for(i=0; i<start; i++){
        newstart = start + i;
        dataset[i] = (Evaluate_Items)malloc(sizeof(struct ev_items));
        memcpy(dataset[i], evt_items[newstart], sizeof(struct ev_items));
    }
    if(dataset != NULL)
        ret = 0;
    return ret; 
}

int datingClassTest(void){
    const double hoRatio=0.50;    //hold out 10%
    int m = MAX_LINE; 
    int numTestVecs = m * hoRatio; 
    int classifierResult;
    double errorCount = 0.0;
    file2matrix("datingTestSet2.txt");
    autoNorm();
    Evaluate_Items dataset[m-numTestVecs];
    int check = create_dataset(dataset, numTestVecs, m);
    int i;
    for(i=0;i<numTestVecs;i++){
        classifierResult = classify1(evt_items[i], dataset, m-numTestVecs, 3);
        printf("the classifier came back with: %d, the real answer is: %d\n",classifierResult, evt_items[i]->classlabel);
        if(classifierResult!=evt_items[i]->classlabel) errorCount += 1.0;
    }
    printf("the total error rate is: %f\n" , errorCount/(float)numTestVecs);
    printf("%f\n" , errorCount);
}

int classifyPerson(){
    const char* resultList[3] = {"not at all", "in small doses", "in larget doses"}; 
    Evaluate_Items  classifyperson = (Evaluate_Items)malloc(sizeof(struct ev_items)); 
    int classifyResult;
    double percenTats, ffMiles, iceCream;
    printf("percentage of time spent playing video games?");
    scanf("%lf", &percenTats);
    printf("frequent flier miles earned per year?");
    scanf("%lf", &ffMiles);
    printf("liters of ice cream consumed per year?");
    scanf("%lf", &iceCream);
    file2matrix("datingTestSet.txt");
    autoNorm();
    classifyperson->flymiles          = (percenTats-norm_min.flymiles)/(norm_max.flymiles-norm_min.flymiles);
    classifyperson->playvideogame     = (ffMiles-norm_min.playvideogame)/(norm_max.flymiles-norm_min.flymiles);
    classifyperson->cameraconsumption = (iceCream-norm_min.cameraconsumption)/(norm_max.flymiles-norm_min.flymiles);
    classifyResult = classify1(classifyperson, evt_items, MAX_LINE, 3);
    printf("You will probably like this person: %s\n", resultList[classifyResult - 1]);
    free(classifyperson);
}

int img2vector(char* filename, unsigned int* returnVect){
    int ret = -1;
    unsigned char  line[34];
    FILE *fp;
    int i,j = 0;
    int conv_int;
    fp=fopen(filename,"r");
    if(fp!=NULL){
        while(fgets(line,sizeof(line),fp)){
            if(strcmp(line,"\n")!=0){   //avoid "enter signal"
            returnVect[j] = 0;
            i=0;
            while(i<32){
               conv_int = line[i] - 48;
               returnVect[j] |= conv_int << (31-i); //shift start from 1, not zero
               i++;
            }
            j++;}
        }
        ret = 0;
    }else{
         printf("Cannot access the file!");
    }
    fclose(fp);

    return ret;
}

void prepare_img2vector(const char* filename, training_item_ptr* dataset, const int ds_len){
    int  file_number = ds_len+2;      //including ./ and ../
    char fileNames[file_number][12];
    DIR *dp;
    struct dirent *ep;
    char* c;
    char file_path[32];
    //read the file matrix from path
    int i=0, j=0;
    dp = opendir (filename);
    if (dp != NULL)
    {
        while (ep = readdir(dp)){
            sprintf(fileNames[i], "%s", ep->d_name);
            i++;
        }
        (void) closedir (dp);
    }else
         perror ("Couldn't open the directory");
    //Process data into structure
    for(i=0; i<file_number;i++){
        if(i>1)        //bypass ./ and ../
        {  
            snprintf(file_path, sizeof(file_path), "%s/%s", filename, fileNames[i]);
            c = strtok(fileNames[i], "_");
            dataset[j]->hwLabel = atoi(c); 
            img2vector(file_path, dataset[j]->data);
            //capture training 3_162.txt and test 9_67.txt
            //if(strcmp(file_path, "./trainingDigits/3_162.txt") == 0)
            //    printf("dataset[%d] = 3_162.txt\n", j);
            //if(strcmp(file_path, "./testDigits/9_67.txt") == 0)
            //    printf("testset[%d] = 9_67.txt\n", j);
            j++;
        }
    }
}

void handwritingClassTest(void){
    const int ds_len = 1934;  //including ./ and ../, need to bypass
    const int ts_len = 946;   //including ./ and ../, need to bypass
    training_item_ptr training_dataset[ds_len];
    training_item_ptr vectorUnderTest[ts_len];
    int training_data[32];
    int classifierResult;
    double errorCount;

    int i;
    for(i=0;i<ds_len;i++){
       training_dataset[i] = (training_item_ptr)malloc(sizeof(struct training_item)); 
       memset(training_dataset[i], 0, sizeof(struct training_item));
    }
    prepare_img2vector("./trainingDigits", training_dataset, ds_len);
    
    for(i=0;i<ts_len;i++){
       vectorUnderTest[i] = (training_item_ptr)malloc(sizeof(struct training_item)); 
       memset(vectorUnderTest[i], 0, sizeof(struct training_item));
    }
    // Post condition : the elements of vectorUnderTest contain the correct answer extract from the file names
    prepare_img2vector("./testDigits", vectorUnderTest, ts_len);
 
    int mTest = ts_len;
    for(i=0;i<mTest;i++){
        classifierResult = classify2(vectorUnderTest[i], training_dataset, ds_len, 3);
        printf("the classifier came back with: %d, the real answer is: %d\n",classifierResult, vectorUnderTest[i]->hwLabel);
        if(classifierResult!=vectorUnderTest[i]->hwLabel) errorCount += 1.0;
    }
    printf("\nthe total number of errors is: %f\n", errorCount);
    printf("\nthe total error rate is: %f\n", errorCount/(double)mTest);
 
}

void verify(void){
    int i=0;
    for(i=0; i<MAX_LINE; i++){
        printf("item[%d]\tflymiles = %.8lf\tplayvideogame = %.8lf\tcameraconsumption = %.8lf\n", i, evt_items[i]->flymiles, evt_items[i]->playvideogame, evt_items[i]->cameraconsumption);
        printf("class label[%d] = %d\n", i, evt_items[i]->classlabel);
    }
    printf("norm_min.flymiles = %.8lf\t norm_min.playvideogame = %.8lf\tnorm_min.cameraconsumption = %.8lf\n", norm_min.flymiles, norm_min.playvideogame, norm_min.cameraconsumption);
    printf("norm_max.flymiles = %.8lf\t norm_max.playvideogame = %.8lf\tnorm_max.cameraconsumption = %.8lf\n", norm_max.flymiles, norm_max.playvideogame, norm_max.cameraconsumption);
}

void debug_verify1(training_item_ptr* dataset, int len){
    int i,j;
    for(i=0;i<len;i++){
        printf("data : ");
        for(j=0; j<32; j++) 
            printf("0x%x\n", dataset[i]->data[j]);
        j=0;
        printf("hwLabel = %d\n", dataset[i]->hwLabel);
        printf("distance_from_test = %f\n", dataset[i]->distance_from_test);
    }

}

void debug_verify(Evaluate_Items* dataset, int size){
    int i=0;
    if(dataset == NULL)
        return;
    for(i=0; i<size; i++){
        printf("%s\n", __FUNCTION__);
        printf("item[%d]\tflymiles = %.8lf\tplayvideogame = %.8lf\tcameraconsumption = %.8lf\n", i, dataset[i]->flymiles, dataset[i]->playvideogame, dataset[i]->cameraconsumption);
        printf("class label[%d] = %d\n", i, dataset[i]->classlabel);
    }
}

void verify_plot(void){
    int i=0;
    for(i=0; i<MAX_LINE; i++){
        printf("{%lf,%lf},\n", evt_items[i]->playvideogame, evt_items[i]->cameraconsumption);
    }
}

void output_plot(double* predict, char* result){
    int i,j=0;
    for(i=0; i<4; i++)
        printf("%lf,%lf,%d\n", groups[i][j], groups[i][j+1], 0);
    printf("%lf,%lf,%d", predict[0], predict[1],0);
}
int main(int argc, char** argv)
{
    //int i,j;
    //for ( i = 0; i< 4; i++)
    //    for( j = 0; j < 2; j++)
    //        printf("[%d][%d] = %f\n", i, j, groups[i][j]);
    //for ( i = 0; i < 4; i++)
    //    printf("[%d] = %c\n", i, labels[i]);
    //double point[2] ={0.5,0.9};
    //classify0(point, groups, labels, 3);
    //output_plot(point, NULL);   
    //file2matrix("datingTestSet.txt");
    //autoNorm();
    //verify();
    //datingClassTest();
    //verify_plot();
    //classifyPerson();
    handwritingClassTest();
    return 1;
}
