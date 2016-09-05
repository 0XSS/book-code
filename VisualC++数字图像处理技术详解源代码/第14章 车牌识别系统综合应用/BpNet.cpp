#include "StdAfx.h"
#include "BpNet.h"
 

/****************************************************************************** 
        ��������� 
******************************************************************************/ 

//����α��������� 
void BpNet:: InitializeRandoms()  
{ 
  srand(4711); 
} 
//����һ��LOW - TOP֮���α������� 
int BpNet::RandomEqualINT(int Low, int High)  
{ 
  return rand() % (High-Low+1) + Low; 
}      
//����һ��LOW - TOP֮���α��������� 
double BpNet::RandomEqualREAL(double Low, double High) 
{ 
  return ((double) rand() / RAND_MAX) * (High-Low) + Low; 
} 
/****************************************************************************** 
  //�ر��ļ�          
******************************************************************************/ 
void BpNet::FinalizeApplication(NET* Net) 
{ 
  fclose(f); 
}      
/****************************************************************************** 
//�����������Ȩ            
******************************************************************************/ 
void BpNet::RandomWeights(NET* Net)  
{ 
  int l,i,j; 
  
  for (l=1; l<NUM_LAYERS; l++) {  //ÿ�� 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->Weight[i][j] = RandomEqualREAL(-0.5, 0.5);//���ֵ 
      } 
    } 
  } 
} 
/****************************************************************************** 
//��������Ȩ����ֹ��ʧ���������Ȩ                                    
******************************************************************************/ 
void BpNet::SaveWeights(NET* Net) 
{ 
  int l,i,j; 

  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->WeightSave[i][j] = Net->Layer[l]->Weight[i][j]; 
      } 
    } 
  } 
} 
/****************************************************************************** 
//�ָ�����Ȩ���Ա���Ҫ��ʱ��������µ��ã���������              
******************************************************************************/ 
void BpNet::RestoreWeights(NET* Net)                
{ 
  int l,i,j; 

  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->Weight[i][j] = Net->Layer[l]->WeightSave[i][j];    
      } 
    } 
  } 
} 
/****************************************************************************** 
//�������磬Ϊ�������ռ�                        
******************************************************************************/ 
void BpNet::GenerateNetwork(NET* Net) 
{ 
  int l,i; 

  Net->Layer = (LAYER**) calloc(NUM_LAYERS, sizeof(LAYER*)); 
  
  for (l=0; l<NUM_LAYERS; l++) { 
    Net->Layer[l] = (LAYER*) malloc(sizeof(LAYER)); 
      
    Net->Layer[l]->Units      = Units[l]; 
    Net->Layer[l]->Output    = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Activation = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Error      = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Weight    = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->WeightSave = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->dWeight    = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->Output[0]  = BIAS; 
      
    if (l != 0) { 
      for (i=1; i<=Units[l]; i++) { 
        Net->Layer[l]->Weight[i]    = (double*) calloc(Units[l-1]+1, sizeof(double)); 
        Net->Layer[l]->WeightSave[i] = (double*) calloc(Units[l-1]+1, sizeof(double)); 
        Net->Layer[l]->dWeight[i]    = (double*) calloc(Units[l-1]+1, sizeof(double)); 
      } 
    } 
  } 
  Net->InputLayer  = Net->Layer[0];//Ϊ��������ָ�� 
  Net->OutputLayer = Net->Layer[NUM_LAYERS-1];//Ϊ��������ָ�� 
  Net->Alpha      = 0.8;//�������� 
  Net->Eta        = 0.5;//ѧϰ�� 
  Net->Epsilon    =0.005;//���ƾ��� 
} 
/****************************************************************************** 
����������ת����Ϊ����ģʽ��������һ���ļ��Ա�����ʾ��� 
******************************************************************************/ 
void BpNet::InitializeApplication(NET* Net)  
{ 
  int  n, i,j;      
  
  for (n=0; n<NUM_DATA; n++) { 
      for (i=0; i<Y; i++) { 
          for (j=0; j<X; j++) { 
              char c=Pattern[n][i][j];
              if ( Pattern[n][i][j] == 'O') 
                    Input_bp[n][i*X+j]  = HI ; 
                else Input_bp[n][i*X+j]  =LO ;  
              //NUM_DATA����ģʽ�������X*Y����Ԫ 
          } 
      } 
  } 
  f = fopen("result.txt", "w"); 
} 

/****************************************************************************** 
                      ѵ������ 
//��ÿ������Ͷ������������Input��ת���������ģʽ��TargetΪ��ʦ�źţ�ͨ�������� 
//��Training��Ptotocolingֵ�����Ƿ�ѵ���ʹ�ӡ����/���ģʽ 
******************************************************************************/ 
void BpNet::SimulateNet(NET* Net, double* Input_bp, double* Target, BOOL Training,BOOL Protocoling) 
{ 
  double Output[M]; //������¼�������� 
  SetInput(Net, Input_bp,Protocoling);//��������㣬������������ 
  PropagateNet(Net);//���������������          
  GetOutput(Net, Output,Protocoling);//������������ 
  
  ComputeOutputError(Net, Target);//������������ 
  if (Training) { 
    BackpropagateNet(Net);//���򴫲� 
    AdjustWeights(Net);//��������Ȩ 
  } 
} 
/****************************************************************************** 
    ������������          
******************************************************************************/ 
void BpNet::SetInput(NET* Net, double* Input_bp,BOOL Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->InputLayer->Units; i++) {        
    Net->InputLayer->Output[i] = Input_bp[i-1];      //��������� 

  } 
  if (Protocoling) { 
    WriteInput(Net, Input_bp);//����Protocolingֵ�������ģʽ 
  } 
} 
/****************************************************************************** 
//���㵱ǰ������������upper Ϊ��ǰ�㣬LOWERΪǰһ��                    
******************************************************************************/ 
void BpNet::PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper)  
{ 
  int  i,j; 
  double Sum; 

  for (i=1; i<=Upper->Units; i++) { 
    Sum = 0; 
    for (j=0; j<=Lower->Units; j++) { 
      Sum += Upper->Weight[i][j] * Lower->Output[j];  //���㱾��ľ����� 
    }  
  Upper->Activation[i] = Sum;//��������ֵ 
  //���㱾������,�����������S�κ����������ſɵ�������BP���������ǰ�� 
  Upper->Output[i]=1/(1+exp(-Sum)); 
  } 
} 
/****************************************************************************** 
              //������������������� 
******************************************************************************/ 
void BpNet::PropagateNet(NET* Net) 
{ 
  int l; 
  
  for (l=0; l<NUM_LAYERS-1; l++) { 
    PropagateLayer(Net, Net->Layer[l], Net->Layer[l+1]); 
  } 
} 
/****************************************************************************** 
//������������              
******************************************************************************/ 
void BpNet::GetOutput(NET* Net, double* Output,BOOL Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->OutputLayer->Units; i++) {  
    Output[i-1] = Net->OutputLayer->Output[i];//�������� 
  } 
  if (Protocoling) { 
    WriteOutput(Net, Output);//����Protocolingֵ������ģʽ 
  } 
} 
/****************************************************************************** 
                  //�����������* Target�ǵ�ʦ�ź� 
******************************************************************************/ 
void BpNet::ComputeOutputError(NET* Net, double* Target)  
{ 
  int  i; 
  double  Err,Out;
  double temp;
  
  Net->Error = 0; 
  for (i=1; i<=Net->OutputLayer->Units; i++) { 
    Out = Net->OutputLayer->Output[i];//��������� 
    temp=Target[i-1];
    Err = temp-Out;//������ 
    Net->OutputLayer->Error[i] = Out * (1-Out) * Err; 
        //��delta�����������Ϊ���˿ɵ���s�κ��� 
    Net->Error += 0.5 * sqr(Err);//ƽ���ʽ 
  } 
} 
/****************************************************************************** 
  //���򴫲� Upper Ϊǰ�㣬LowerΪ��� ������ֵ���Ϊǰ��            
******************************************************************************/ 
void BpNet::BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower)  
{                                                              
 int  i,j;//ѭ������ 
  double Out, Err; 
  
  for (i=1; i<=Lower->Units; i++) { 
    Out = Lower->Output[i];//������� 
    Err = 0;//������¼��������������Ĺ���ֵ 
    for (j=1; j<=Upper->Units; j++) { 
      Err += Upper->Weight[j][i] * Upper->Error[j]; 
    //���ķ�����ͨ���Ѿ������ǰ���deltaֵ������Ȩȥ���ƣ������ۻ��� 
    } 
    Lower->Error[i] =Out * (1-Out) * Err;  //delta���� 
  } 
} 
/****************************************************************************** 
              //�����������ĺ� 
******************************************************************************/ 
void BpNet::BackpropagateNet(NET* Net) 
{ 
  int l;//ѭ������ 
  
  for (l=NUM_LAYERS-1; l>1; l--) { 
    BackpropagateLayer(Net, Net->Layer[l], Net->Layer[l-1]);//��ÿ�㴦�� 
  } 
} 
/****************************************************************************** 
              //��������ÿһ�������Ȩ 
******************************************************************************/ 
void BpNet::AdjustWeights(NET* Net)    
{ 
  int  l,i,j;//ѭ������ 
  double Out, Err, dWeight; 
  //��¼�����������ǰ����������ǰ��Ԫ����Ȩ�ϴεĵ����� 
  
  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Out = Net->Layer[l-1]->Output[j];//������� 
        Err = Net->Layer[l]->Error[i];//��ǰ��������� 
        dWeight = Net->Layer[l]->dWeight[i][j]; 
        //������Ԫ����Ȩ�ϴεĵ�����ȡ������ʼֵΪ0����ʼ������ʱ��ֵ�� 
        Net->Layer[l]->Weight[i][j] += Net->Eta * Err * Out + Net->Alpha * dWeight; 
        //AlphaΪ�����������ӿ�����������ٶ� 
        Net->Layer[l]->dWeight[i][j] = Net->Eta * Err * Out; 
        //��¼������Ԫ����Ȩ�ĵ����� 
      } 
    } 
  } 
} 
/****************************************************************************** 
//��ʾ����ģʽ              
******************************************************************************/ 
void BpNet::WriteInput(NET* Net, double* Input_bp)  
{ 
  int i; 
  
  for (i=0; i<N; i++) { 
    if (i%X == 0) { 
      fprintf(f, "\\n"); 
    } 
    fprintf(f, "%c", (Input_bp[i] == HI) ?'0' : ' '); 
  } 
  fprintf(f, " -> ");  
} 
/****************************************************************************** 
//��ʾ���ģʽ              
******************************************************************************/ 
void BpNet::WriteOutput(NET* Net, double* Output) 
{ 
  int  i;//ѭ������ 
  int  Index;//������¼������ֵ���±꣬Ҳ�������ʶ��Ľ�� 
  double  MaxOutput;//������¼�������ֵ 
    
  MaxOutput=0;//��ʼ�� 
  for (i=0; i<M; i++) 
  {    
      if(MaxOutput<Output[i]){ 
          MaxOutput=MAX(MaxOutput,Output[i]);//�������ֵ 
          Index=i; 
      } 
  } 
      
  fprintf(f, "%i\\n", Index);//д���ļ� 
  
} 
/****************************************************************************** 
              ��ʼ���������� 
******************************************************************************/ 
void BpNet::Initializetest() 
{ 
    int n,i,j;//ѭ������ 
    
    for (n=0; n<NUM_DATA; n++) { 
        for (i=0; i<Y; i++) { 
            for (j=0; j<X; j++)                                
                if (testPattern[n][i][j]=='O') 
                    Inputtest[n][i*X+j] = HI; 
                else Inputtest[n][i*X+j] =LO;  //NUM_DATA����ģʽ�������X*Y����Ԫ                                    
        } 
    } 
}

BpNet::~BpNet(void)
{
}
