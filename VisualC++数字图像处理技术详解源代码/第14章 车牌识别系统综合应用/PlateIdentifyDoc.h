// PlateIdentifyDoc.h : CPlateIdentifyDoc ��Ľӿ�
//


#pragma once
#include "cv.h"
#include "highgui.h"
#include "myimg.h"


#define MIN(x,y)      ((x)<(y) ? (x) : (y)) 
#define MAX(x,y)      ((x)>(y) ? (x) : (y))
#define sqr(x)        ((x)*(x)) 

const double LO=0.1; 
const double HI=0.9; 
const double BIAS=0.5; 

const int NUM_LAYERS=3;                                               //������� 
const int NUM_DATA=7;                                                 //������ 
const int X=6;                                                        //ÿ������������ 
const int Y=7;                                                        //ÿ������������ 
const int N=X * Y;                                                    //�������Ԫ���� 
const int M=7;                                                        //�������Ԫ���� 
typedef struct {                    
        int            Units;                                         //����Ԫ���� 
        double*        Output;                                        //����� ���������ʸ��Ԫ�ظ����� 
        double*        Activation;                                    //����ֵ 
        double*        Error;                                         //�������            
        double**       Weight;                                        //����Ȩ 
        double**       WeightSave;                                    //����ѵ�������������Ȩ 
        double**       dWeight;                                       //������ 
} LAYER;                                                              //�������ṹ 

typedef struct {                    
        LAYER**       Layer;                                          //���������ָ�� 
        LAYER*        InputLayer;                                     //����� 
        LAYER*        OutputLayer;                                    //����� 
        double        Alpha;                                          //�������� 
        double        Eta;                                            //ѧϰ��        
        double        Error;                                          //����� 
        double        Epsilon;                                        //���ƾ��� 
} NET;                                                                //������



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
void SimulateNet(NET* Net, double* Input, double* Target, bool Training,bool Protocoling);//��ÿ����������Ͷ����������
void SetInput(NET* Net, double* Input,bool Protocoling);// ������������      
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper);//���㵱ǰ������������upper Ϊ��ǰ�㣬LOWERΪǰһ�� 
void PropagateNet(NET* Net);//������������������� 
void GetOutput(NET* Net, double* Output,bool Protocoling);//������������ 
void ComputeOutputError(NET* Net, double* Target);//�������������������� 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower);//��ǰ�����򴫲� 
void BackpropagateNet(NET* Net);////�����������ĺ� 
void AdjustWeights(NET* Net); //���������������Ȩ����ȡ�������� 
void WriteInput(NET* Net, double* Input);//��ʾ����ģʽ    
void WriteOutput(NET* Net, double* Output);//��ʾ���ģʽ  
void Initializetest();//����������ת����Ϊ����ģʽ 
void TegetData(char taget[NUM_DATA][M],double tagetdouble[NUM_DATA][M]);//���ļ��ж���Ŀ������
void GetSwatch(char swatch[NUM_DATA][Y][X]);   //���ļ��ж���ѵ����
void GetTest(char swatch11[NUM_DATA][Y][X]);   //���ļ��������Լ�
int ImageStretchByHistogram(IplImage *src,IplImage *dst); 

const int g_width=6;                      //���ù�һ���Ŀ�� 
const int g_height=7;                     //���ù�һ���ĸ߶� 

class CPlateIdentifyDoc : public CDocument
{
protected: // �������л�����
	CPlateIdentifyDoc();
	DECLARE_DYNCREATE(CPlateIdentifyDoc)


// ����
public:
    //����Open cv�ı����Ķ���
    IplImage * m_image,*gray;             //����ԭͼ�񡢻Ҷ�ͼ��
    IplImage *slipimage;                  //����ָ��Цͼ��   
    IplImage * result;                    //����ָ�����ͼ��
    myimg m_Cimage;                       //��Ҫ������ʾͼ�����̳�
    NET Net;                              //������
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CPlateIdentifyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public:
    afx_msg void OnGray();
public:
    afx_msg void OnGraystrenth();
public:
    afx_msg void OnEdgeadd();
public:
    afx_msg void OnBinary();
public:
    afx_msg void OnPosition();
public:
    afx_msg void OnState();
public:
    afx_msg void OnRevise();
public:
    afx_msg void OnSplit();
public:
    afx_msg void OnNettrain();

    int CPlateIdentifyDoc::map(char *character);
public:
    afx_msg void OnGuiyi();
public:
    
public:
    
public:
    afx_msg void OnTest();
public:
    afx_msg void OnChartIdentify();
};


