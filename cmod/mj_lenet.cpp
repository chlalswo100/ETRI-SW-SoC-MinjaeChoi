#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace cv;
using namespace std;

double ***conv(int *input_size,int *out_size,int *filter_input,int *filter_channel,int *filter_size,int *stride,int *pad_size,double ***im,double ***o,double ****conv_fil)
{   
    int c=0;
    
    for(int k=0;k<*filter_input;k++)
    {
        for(int a=0;a<*out_size;a++)
        {
            for(int b=0;b<*out_size;b++)
            {  
                for(int l=0;l<*filter_channel;l++)
                {
                    for(int i=0;i<*filter_size;i++)
                    {        
                        for(int j=0;j<*filter_size;j++)
                        {
                            o[k][a][b] += im[l][i+a*c][j+b*c]*conv_fil[k][l][i][j];
                        }
                    }
                }
            c=*stride;
            }   
        } 
    }

}
double ***pool(int *input_size,int *out_size,int *filter_input,int *filter_channel,int *filter_size,int *stride,int *pad_size,double ***im,double ***o1,double ***o2)
{
    double temp;
    for(int k=0;k<*filter_input;k++)
    {
        for(int a=0;a<*out_size;a++)
        {
            for(int b=0;b<*out_size;b++)
            {  
                for(int i=0;i<*filter_size;i++)
                {        
                    for(int j=0;j<*filter_size;j++)
                    {   
                        double m = o1[k][i+a*2][j+b*2];
                        if(j==0 && i==0) temp = m;
                        else if(temp < m) temp = m;
                    }
                }
            o2[k][a][b]=temp;
            }   
        } 
    }
    
}

int main(){

    FILE *ifp1;
    FILE *ifp2;
    FILE *ifp3;
    FILE *ifp4;

	ifp1 = fopen("Conv1WeightOrigin.txt","r");
    ifp2 = fopen("Conv2WeightOrigin.txt","r");
    ifp3 = fopen("Ip1WeightOrigin.txt","r");
    ifp4 = fopen("Ip2WeightOrigin.txt","r");
	
    if(ifp1==NULL){
		perror("file open for writing");
		exit(1);
	}
    if(ifp2==NULL){
		perror("file open for writing");
		exit(1);
	}
    if(ifp3==NULL){
		perror("file open for writing");
		exit(1);
	}
    if(ifp4==NULL){
		perror("file open for writing");
		exit(1);
	}
    
    Mat image;
    
    image = imread("8.jpg",IMREAD_COLOR);
    if(image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    

    double ***im;
    double ***output1;
    double ***output2;
    double ***output3;
    double ***output4;
    double ***output5;
    double ***output6;
    double ***output7;
    
    double ****conv1_fil;
    double ****conv2_fil;
    double ***ip1_fil;
    double ***ip2_fil;
    // double ****conv2_fil;
    // double ****ip1_fil;
    // double ****ip2_fil;

    int *input_size;      // 입력 벡터 사이즈
    int *out_size;        // 출력 벡터 사이즈
    int *filter_input;    // 필터 개수
    int *filter_channel;  // 필터 채널
    int *filter_size;     // 필터 사이즈
    int *stride;          // 스트라이드
    int *pad_size;        // 패딩

    int i_s;
    int o_s;
    int f_i;
    int f_c;
    int f_s;
    int s;
    int p_s;

    input_size = &i_s;
    out_size = &o_s;
    filter_input = &f_i;
    filter_channel = &f_c;
    filter_size = &f_s;
    stride = &s;
    pad_size = &p_s;

    im=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
        *(im+i)=(double **)malloc(image.rows*sizeof(double *));
        for(int j=0;j<image.rows;j++)
        {
            *(*(im+i)+j)=(double *)malloc(image.cols*sizeof(double));
        }
    }
 
    // 이미지값 쓰기
    for(int k=0;k<1;k++)
    {
        for(int i=0;i<image.rows;i++)
        {
            for(int j=0;j<image.cols;j++)
            {   // k=0 : r / k=1 : g / k=2 : b
                im[k][i][j]=image.at<uchar>(i,j);  
            }  
        }  
    }
    // conv1 layer
    i_s = image.rows;
    o_s = 24;
    f_i = 20;
    f_c = 1;
    f_s = 5;
    s   = 1;
    p_s   = 0;
    
    output1=(double ***)malloc(f_i*sizeof(double **));
    for(int i=0;i<f_i;i++)
    {
        *(output1+i)=(double **)malloc(o_s*sizeof(double *));
        for(int j=0;j<o_s;j++)
        {    
            *(*(output1+i)+j)=(double *)malloc(o_s*sizeof(double));
        }
    }
                           
    for(int k=0;k<f_i;k++)
    {
        for(int i=0;i<o_s;i++)
        {
            for(int j=0;j<o_s;j++)
            {  
                output1[k][i][j]=0;  
            }  
        }  
    }
    conv1_fil=(double ****)malloc(f_i*sizeof(double ***));
    for(int i=0;i<f_i;i++)
    {
       *(conv1_fil+i)=(double ***)malloc(f_c*sizeof(double **));
        for(int j=0;j<f_c;j++)
        {
            *(*(conv1_fil+i)+j)=(double **)malloc(f_s*sizeof(double*));
            for(int k=0;k<f_s;k++)
            {
                *(*(*(conv1_fil+i)+j)+k)=(double *)malloc(f_s*sizeof(double));
            }
        }
    }

    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
               for(int l=0;l<f_s;l++)
               {
               conv1_fil[i][j][k][l]  =0;
               }
            }
        }
    }

    // 필터 입력해주기
    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
               for(int l=0;l<f_s;l++)
               {
               fscanf(ifp1,"%lf", &conv1_fil[i][j][k][l] ); 
               }
            }
        }
    }
    
    int p=0;
    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
               for(int l=0;l<f_s;l++)
               {
               printf(" %lf ", conv1_fil[i][j][k][l] ); 
               printf("%d ",p);
               p++;
               }
            }
        }
    }
    
    conv(input_size,out_size,filter_input,filter_channel,filter_size,stride,pad_size,im,output1,conv1_fil);


    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
                free(*(*(*(conv1_fil+i)+j)+k));
            }
            free(*(*(conv1_fil+i)+j));
        }
        free(*(conv1_fil+i));
    }
    free(conv1_fil);

    
   
    
    // pooling 1
    i_s = 24;
    o_s = 12;
    f_i = 20;
    f_c = 1;
    f_s = 2;
    s   = 2;
    p_s  = 0;
    
    output2=(double ***)malloc(f_i*sizeof(double **));
    for(int i=0;i<f_i;i++)
    {
        *(output2+i)=(double **)malloc(o_s*sizeof(double *));
        for(int j=0;j<o_s;j++)
        {    
            *(*(output2+i)+j)=(double *)malloc(o_s*sizeof(double));
        }
    }
                           
    for(int k=0;k<f_i;k++)
    {
        for(int i=0;i<o_s;i++)
        {
            for(int j=0;j<o_s;j++)
            {  
                output2[k][i][j]=0;  
            }  
        }  
    }
    pool(input_size,out_size,filter_input,filter_channel,filter_size,stride,pad_size,im,output1,output2);
    
    // conv2 layer
    i_s = 12;
    o_s = 8;
    f_i = 50;
    f_c = 20;
    f_s = 5;
    s   = 1;
    p_s   = 0;
    
    output3=(double ***)malloc(f_i*sizeof(double **));
    for(int i=0;i<f_i;i++)
    {
        *(output3+i)=(double **)malloc(o_s*sizeof(double *));
        for(int j=0;j<o_s;j++)
        {    
            *(*(output3+i)+j)=(double *)malloc(o_s*sizeof(double));
        }
    }
                           
    for(int k=0;k<f_i;k++)
    {
        for(int i=0;i<o_s;i++)
        {
            for(int j=0;j<o_s;j++)
            {  
                output3[k][i][j]=0;  
            }  
        }  
    }
    conv2_fil=(double ****)malloc(f_i*sizeof(double ***));
    for(int i=0;i<f_i;i++)
    {
       *(conv2_fil+i)=(double ***)malloc(f_c*sizeof(double **));
        for(int j=0;j<f_c;j++)
        {
            *(*(conv2_fil+i)+j)=(double **)malloc(f_s*sizeof(double*));
            for(int k=0;k<f_s;k++)
            {
                *(*(*(conv2_fil+i)+j)+k)=(double *)malloc(f_s*sizeof(double));
            }
        }
    }
    // 필터 입력해주기
    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
               for(int l=0;l<f_s;l++)
               {
               fscanf(ifp2,"%lf", &conv2_fil[i][j][k][l] ); 
               }
            }
        }
    }

    conv(input_size,out_size,filter_input,filter_channel,filter_size,stride,pad_size,output2,output3,conv2_fil);

    for(int i=0;i<f_i;i++)
    {
        for(int j=0;j<f_c;j++)
        {
            for(int k=0;k<f_s;k++)
            {
                free(*(*(*(conv2_fil+i)+j)+k));
            }
            free(*(*(conv2_fil+i)+j));
        }
        free(*(conv2_fil+i));
    }
    free(conv2_fil);    
    

    // pooling 2
    i_s = 8;
    o_s = 4;
    f_i = 50;
    f_c = 1;
    f_s = 2;
    s   = 2;
    p_s  = 0;
    
    output4=(double ***)malloc(f_i*sizeof(double **));
    for(int i=0;i<f_i;i++)
    {
        *(output4+i)=(double **)malloc(o_s*sizeof(double *));
        for(int j=0;j<o_s;j++)
        {    
            *(*(output4+i)+j)=(double *)malloc(o_s*sizeof(double));
        }
    }
                           
    for(int k=0;k<f_i;k++)
    {
        for(int i=0;i<o_s;i++)
        {
            for(int j=0;j<o_s;j++)
            {  
                output4[k][i][j]=0;  
            }  
        }  
    }
    pool(input_size,out_size,filter_input,filter_channel,filter_size,stride,pad_size,im,output3,output4);

//ip1
    output5=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
        *(output5+i)=(double **)malloc(800*sizeof(double *));
        for(int j=0;j<800;j++)
        {    
            *(*(output5+i)+j)=(double *)malloc(1*sizeof(double));
        }
    }

    for(int k=0;k<1;k++)
    {
        for(int i=0;i<800;i++)
        {
            for(int j=0;j<1;j++)
            {  
                output5[k][i][j]=0;  
            }  
        }  
    }

    output6=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
        *(output6+i)=(double **)malloc(500*sizeof(double *));
        for(int j=0;j<500;j++)
        {    
            *(*(output6+i)+j)=(double *)malloc(1*sizeof(double));
        }
    }

    for(int k=0;k<1;k++)
    {
        for(int i=0;i<500;i++)
        {
            for(int j=0;j<1;j++)
            {  
                output6[k][i][j]=0;  
            }  
        }  
    }
    int a=0;
    for(int l=0;l<50;l++)
    {
        for(int i=0;i<4;i++)
        {        
            for(int j=0;j<4;j++)
            {
                output5[0][a][0] = output4[l][i][j];
                a++;
            }
        }
    }


    ip1_fil=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
       *(ip1_fil+i)=(double **)malloc(500*sizeof(double *));
        for(int j=0;j<500;j++)
        {
            *(*(ip1_fil+i)+j)=(double *)malloc(800*sizeof(double));
        }
    }
    // 필터 입력해주기
    
    for(int i=0;i<1;i++)
    {
        for(int j=0;j<500;j++)
        {
            for(int k=0;k<800;k++)
            {
               fscanf(ifp3,"%lf", &ip1_fil[i][j][k] ); 
             
             
            }
        }
    }
    
    
        double sum=0;
        for(int i=0;i<500;i++)
        {        sum=0;
            for(int j=0;j<800;j++)
            {
                sum+=ip1_fil[0][i][j]*output5[0][j][0];
            }
        output6[0][i][0]=sum;
        }
    
    
    // 500x800 free
    for(int i=0;i<1;i++)
    {
        for(int j=0;j<500;j++)
        {
            free(*(*(ip1_fil+i)+j));
        }
        free(*(ip1_fil+i));
    }
    free(ip1_fil);    


    for(int i=0;i<500;i++)
    {
        if(output6[0][i][0]>=0) output6[0][i][0]=output6[0][i][0];
        else if(output6[0][i][0]<0) output6[0][i][0]=0;
    }

//ip2
    output7=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
        *(output7+i)=(double **)malloc(10*sizeof(double *));
        for(int j=0;j<10;j++)
        {    
            *(*(output7+i)+j)=(double *)malloc(500*sizeof(double));
        }
    }

    for(int k=0;k<1;k++)
    {
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<1;j++)
            {  
                output7[k][i][j]=0;  
            }  
        }  
    }

    ip2_fil=(double ***)malloc(1*sizeof(double **));
    for(int i=0;i<1;i++)
    {
       *(ip2_fil+i)=(double **)malloc(10*sizeof(double *));
        for(int j=0;j<10;j++)
        {
            *(*(ip2_fil+i)+j)=(double *)malloc(500*sizeof(double));
        }
    }
    // 필터 입력해주기
    
    for(int i=0;i<1;i++)
    {
        for(int j=0;j<10;j++)
        {
            for(int k=0;k<500;k++)
            {
               fscanf(ifp4,"%lf", &ip2_fil[i][j][k] ); 
             
             
            }
        }
    }
    
    
    
        sum=0;
        for(int i=0;i<10;i++)
        {        sum=0;
            for(int j=0;j<500;j++)
            {
                sum+=ip2_fil[0][i][j]*output6[0][j][0];
            }
        output7[0][i][0]=sum;
        }
    
    
    // 500x800 free
    for(int i=0;i<1;i++)
    {
        for(int j=0;j<10;j++)
        {
            free(*(*(ip2_fil+i)+j));
        }
        free(*(ip2_fil+i));
    }
    free(ip2_fil);    

    for(int k=0;k<1;k++)
    {
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<1;j++)
            {  
                printf("%.10lf ",output7[k][i][j]);  
            }  
            printf("\n");
        } printf("\n");
        printf("\n");
    }
   
    
    for(int i=0;i<1;i++)
    {
        for(int j=0;j<image.rows;j++)
        {
            free(*(*(im+i)+j));
        }
        free(*(im+i));
    }
    free(im);

    
    fclose(ifp1);
    fclose(ifp2);
    fclose(ifp3);
    fclose(ifp4);

    for(int i=0;i<20;i++)
    {
        for(int j=0;j<24;j++)
        {
            free(*(*(output1+i)+j));
        }
        free(*(output1+i));
    }
    free(output1);

    for(int i=0;i<20;i++)
    {
        for(int j=0;j<12;j++)
        {
            free(*(*(output2+i)+j));
        }
        free(*(output2+i));
    }
    free(output2);

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<8;j++)
        {
            free(*(*(output3+i)+j));
        }
        free(*(output3+i));
    }
    free(output3);

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<4;j++)
        {
            free(*(*(output4+i)+j));
        }
        free(*(output4+i));
    }
    free(output4);

    for(int i=0;i<1;i++)
    {
        for(int j=0;j<800;j++)
        {
            free(*(*(output5+i)+j));
        }
        free(*(output5+i));
    }
    free(output5);

    for(int i=0;i<1;i++)
    {
        for(int j=0;j<500;j++)
        {
            free(*(*(output6+i)+j));
        }
        free(*(output6+i));
    }
    free(output6);

    for(int i=0;i<1;i++)
    {
        for(int j=0;j<10;j++)
        {
            free(*(*(output7+i)+j));
        }
        free(*(output7+i));
    }
    free(output7);
}
