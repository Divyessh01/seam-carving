#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;

short arr[3000][3000][3];
double energy[3000][3000];
float cost[3000][3000];
short seam[3000];

void createEnergyMatrix(int rows,int cols){

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            int xsquare=0, ysquare=0;
            int rgb;
            // energy calculation for non-border pixels
            if((i!=0) && (i!=rows-1) && (j!=0) && (j!=cols-1)){
                for(int k = 0; k < 3; k++){
                    rgb = arr[i][j+1][k] - arr[i][j-1][k];
                    xsquare += pow(rgb,2);
                }

                for(int k = 0; k < 3; k++){
                    rgb = arr[i+1][j][k] - arr[i-1][j][k];
                    ysquare += pow(rgb,2);
                }

                energy[i][j] = sqrt(xsquare  + ysquare);

            } else if((i == 0 || i == rows-1)){     // energy calculation for border pixels
                if(i == 0){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i+1][j][k] - arr[rows-1][j][k];
                        ysquare += pow(rgb,2);
                    }
                }
                else if(i == rows-1){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[0][j][k] - arr[i-1][j][k];
                        ysquare += pow(rgb,2);
                    }
                }
                if(j == 0){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i][j+1][k] - arr[i][cols-1][k];
                        xsquare += pow(rgb,2);
                    }
                } else if(j == cols-1){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i][0][k] - arr[i][j-1][k];
                        xsquare += pow(rgb,2);
                    }
                } else {
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i][j+1][k] - arr[i][j-1][k];
                        xsquare += pow(rgb,2);
                    }
                }
                energy[i][j] = sqrt(xsquare  + ysquare);
            } else{
                if(j == 0){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i][j+1][k] - arr[i][cols-1][k];
                        xsquare += pow(rgb,2);
                    }
                } else if(j == cols-1){
                    for(int k = 0; k < 3; k++){
                        rgb = arr[i][0][k] - arr[i][j-1][k];
                        xsquare += pow(rgb,2);
                    }
                }
                energy[i][j] = sqrt(xsquare  + ysquare);
            }
        }
    }
}

void createVerticalCostMatrix(int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i==0){
                cost[i][j] = energy[i][j];
            } else if(j==0){
                cost[i][j] = energy[i][j] + min(cost[i-1][j], cost[i-1][j+1]);
            } else if(j==cols-1){
                cost[i][j] = energy[i][j] + min(cost[i-1][j-1], cost[i-1][j]);
            } else{
                cost[i][j] = energy[i][j] + min(cost[i-1][j-1], min(cost[i-1][j], cost[i-1][j+1]));
            }
        }
    }
}

void findVerticalSeam(int rows, int cols){

    createVerticalCostMatrix(rows, cols);  

    float mini = INT_MAX;
    int k = 0;

    for(int j = 0; j < cols; j++){
        if(cost[rows-1][j] <= mini){
            k = j;
            seam[rows-1] = k;
            mini = cost[rows-1][j];
        }
    }

    for(int i = rows - 2; i >= 0; i--){
        for(int j = k-1; j <= k+1; j++){
            if((j>=0) && (j<cols)){
                if(cost[i][j] <= mini){
                    mini = cost[i][j];
                    seam[i] = j;
                }
            }
        }
        k = seam[i];
    }
}

void removeVerticalSeam(int rows, int cols){

    // swap till last
    float temp;
    for(int i = 0; i < rows; i++){
        for(int j = seam[i]; j < cols-1; j++){
            temp = cost[i][j];
            cost[i][j] = cost[i][j+1];
            cost[i][j+1] = temp;

            temp = energy[i][j];
            energy[i][j] = energy[i][j+1];
            energy[i][j+1] = temp;

            for(int k = 0; k < 3; k++){
                temp =  arr[i][j][k];
                arr[i][j][k] = arr[i][j+1][k];
                arr[i][j+1][k] = temp;
            }
        }
    }
}

void createHorizontalCostMatrix(int rows, int cols){
    for(int i = 0; i < cols; i++){
        for(int j = 0; j < rows; j++){
            if(i==0){
                cost[j][i] = energy[j][i];
            } else if(j==0){
                cost[j][i] = energy[j][i] + min(cost[j][i-1], cost[j+1][i-1]);
            } else if(j==rows-1){
                cost[j][i] = energy[j][i] + min(cost[j-1][i-1], cost[j][i-1]);
            } else{
                cost[j][i] = energy[j][i] + min(cost[j-1][i-1], min(cost[j][i-1], cost[j+1][i-1]));
            }
        }
    }
}

void findHorizontalSeam(int rows, int cols){
    
    createHorizontalCostMatrix(rows, cols);

    float mini = INT_MAX;
    int k = 0;

    for(int i = 0; i < rows; i++){
        if(cost[i][cols-1] <= mini){
            k = i;
            seam[cols-1] = k;
            mini = cost[i][cols-1];
        }
    }

    for(int i = cols - 2; i >= 0; i--){
        for(int j = k-1; j <= k+1; j++){
            if((j>=0) && (j<rows)){
                if(cost[j][i] <= mini){
                    mini = cost[j][i];
                    seam[i] = j;
                }
            }
        }
        k = seam[i];
    }

}

void removeHorizontalSeam(int rows, int cols){
    
    float temp;
    for(int i = 0; i < cols; i++){
        for(int j = seam[i]; j < rows-1; j++){
            temp = cost[j][i];
            cost[j][i] = cost[j+1][i];
            cost[j+1][i] = temp;

            temp = energy[j][i];
            energy[j][i] = energy[j+1][i];
            energy[j+1][i] = temp;

            for(int k = 0; k < 3; k++){
                temp =  arr[j][i][k];
                arr[j][i][k] = arr[j+1][i][k];
                arr[j+1][i][k] = temp;
            }
        }
    }
}

int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("Enter 3 argumets: [image path] [new height] [new width]\n");
        return -1;
    }
    
    int h = atoi(argv[2]);
    int w = atoi(argv[3]);

    Mat image;
    image = imread(argv[1], 1);
    if (!image.data) {
        printf("No image data \n");
        return -1;
    }

    int rows = image.rows;
    int cols = image.cols;

    // extract RGB values from mat & store in a 3d array
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            arr[i][j][0] = (int)image.at<Vec3b>(i,j)[0];
            arr[i][j][1] = (int)image.at<Vec3b>(i,j)[1];
            arr[i][j][2] = (int)image.at<Vec3b>(i,j)[2];
        }
    }

    int seamcount = 0;
    int totalseam = 0;
    if(h < rows)
        totalseam = rows - h - 1;
    if(w < cols)
        totalseam += cols - w - 1;
    

    while(w < cols){
        createEnergyMatrix(rows, cols);
        findVerticalSeam(rows, cols);
        removeVerticalSeam(rows, cols);
        cols--;
        seamcount++;
        cout << "Seams carved: " << seamcount << "/" << totalseam << "\r";
    }


    while(h < rows){
        createEnergyMatrix(rows, cols);
        findHorizontalSeam(rows, cols);
        removeHorizontalSeam(rows, cols);
        rows--;
        seamcount++;
        cout << "Seams carved: " << seamcount << "/" << totalseam << "\r";
    }
    
    cout << "Seams carved: " << seamcount << "/" << totalseam << "\r";

    // convert array to output image
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            image.at<Vec3b>(i,j)[0] = arr[i][j][0];
            image.at<Vec3b>(i,j)[1] = arr[i][j][1];
            image.at<Vec3b>(i,j)[2] = arr[i][j][2];
        }
    }

    image.rows = rows;
    image.cols = cols;

    cout << "----------------Done!----------------" << endl;


    cv::namedWindow("Display Image", WINDOW_AUTOSIZE);
    cv::imwrite("../sample_input/output.jpeg", image);

    return 0;
}