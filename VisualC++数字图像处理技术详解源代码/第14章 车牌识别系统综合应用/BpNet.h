#pragma once

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <conio.h> 


#define MAX(x,y)      ((x)>(y) ? (x) : (y)) 
#define sqr(x)        ((x)*(x)) 

const double LO=0.1; 
const double HI=0.9; 
const double BIAS=0.5; 

const int NUM_LAYERS=3;      //������� 
const int NUM_DATA=10;      //������ 
const int X=6;      //ÿ������������ 
const int Y=7;      //ÿ������������ 

const int N=X * Y; //�������Ԫ���� 
const int M=10;      //�������Ԫ���� 

const int Units[NUM_LAYERS] ={N, 10, M}; //��һά�����¼������Ԫ����







typedef struct {                    
            int          Units;        //����Ԫ���� 
            double*        Output;        //����� ���������ʸ��Ԫ�ظ����� 
            double*        Activation;    //����ֵ 
            double*        Error;        //�������            
            double**        Weight;        //����Ȩ 
            double**        WeightSave;    //����ѵ�������������Ȩ 
            double**        dWeight;      //������ 
    } LAYER;    //�������ṹ 

    typedef struct {                    
            LAYER**      Layer;        //���������ָ�� 
            LAYER*        InputLayer;    //����� 
            LAYER*        OutputLayer;  //����� 
            double          Alpha;        //�������� 
            double          Eta;          //ѧϰ��        
            double          Error;        //����� 
            double          Epsilon;      //���ƾ��� 
    } NET; //  ������


class BpNet
{
    public:
        
     
    static double Input_bp[NUM_DATA][N];
    FILE* f;//�����ļ�ָ�� 

    
    
    BpNet(void);

    /****************************************************************************** 
                  ���������� 
******************************************************************************/ 
void InitializeRandoms(); //����α��������� 
int  RandomEqualINT(int Low, int High);//����һ��LOW - TOP֮���α������� 
double RandomEqualREAL(double Low, double High);//����һ��LOW - TOP֮���α��������� 
void FinalizeApplication(NET* Net);//�ر��ļ� 
void RandomWeights(NET* Net) ;//������������������Ȩ 
void SaveWeights(NET* Net);//���������������Ȩ���Է���ʧ���������Ȩ 
void RestoreWeights(NET* Net);//�ָ������������Ȩ���Ա��ؽ����� 
void GenerateNetwork(NET* Net);//�������磬Ϊ�������ռ� 
void InitializeApplication(NET* Net);//��ѧϰ����ת����Ϊ����ģʽ��������һ���ļ��Ա�����ʾ��� 
void SimulateNet(NET* Net, double* Input, double* Target, int Training,int Protocoling);//��ÿ������Ͷ���������� 
void SetInput(NET* Net, double* Input,int Protocoling);// ������������      
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper);//���㵱ǰ������������upper Ϊ��ǰ�㣬LOWERΪǰһ�� 
void PropagateNet(NET* Net);//������������������� 
void GetOutput(NET* Net, double* Output,int Protocoling);//������������ 
void ComputeOutputError(NET* Net, double* Target);//�������������������� 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower);//��ǰ�����򴫲� 
void BackpropagateNet(NET* Net);////�����������ĺ� 
void AdjustWeights(NET* Net); //���������������Ȩ����ȡ�������� 
void WriteInput(NET* Net, double* Input);//��ʾ����ģʽ    
void WriteOutput(NET* Net, double* Output);//��ʾ���ģʽ  
void Initializetest();//����������ת����Ϊ����ģʽ

public:
    ~BpNet(void);
};
