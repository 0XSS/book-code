// PlateIdentifyDoc.cpp : CPlateIdentifyDoc ���ʵ��
//

#include "stdafx.h"
#include "PlateIdentify.h"

#include "PlateIdentifyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int Units[NUM_LAYERS] = {N, 10, M};                            //��һά�����¼������Ԫ���� 
FILE* f;                                                       //�����ļ�ָ�� 

double Input[NUM_DATA][N];                                     //������¼ѧϰ��������ģʽ 
double Inputtest[NUM_DATA][N];                                 //������¼������������ģʽ 
char Pattern[NUM_DATA][Y][X];                                  //������¼ѵ����������
char testPattern[NUM_DATA][Y][X];                              //������¼���Լ�������
double Target[NUM_DATA][M] ;                                   //������¼Ŀ������������
// CPlateIdentifyDoc

IMPLEMENT_DYNCREATE(CPlateIdentifyDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlateIdentifyDoc, CDocument)
    ON_COMMAND(IDM_GRAY, &CPlateIdentifyDoc::OnGray)
    ON_COMMAND(IDM_GRAYSTRENTH, &CPlateIdentifyDoc::OnGraystrenth)
    ON_COMMAND(IDM_EDGEADD, &CPlateIdentifyDoc::OnEdgeadd)
    ON_COMMAND(IDM_BINARY, &CPlateIdentifyDoc::OnBinary)
    ON_COMMAND(IDM_POSITION, &CPlateIdentifyDoc::OnPosition)
    ON_COMMAND(IDM_STATE, &CPlateIdentifyDoc::OnState)
    ON_COMMAND(IDM_REVISE, &CPlateIdentifyDoc::OnRevise)
    ON_COMMAND(ID_SPLIT, &CPlateIdentifyDoc::OnSplit)
    ON_COMMAND(IDM_NETTRAIN, &CPlateIdentifyDoc::OnNettrain)
    ON_COMMAND(ID_GUIYI, &CPlateIdentifyDoc::OnGuiyi)
    //ON_COMMAND(ID_TEST, &CPlateIdentifyDoc::OnTest)
    ON_COMMAND(IDM_CHART_IDENTIFY, &CPlateIdentifyDoc::OnChartIdentify)
END_MESSAGE_MAP()


// CPlateIdentifyDoc ����/����

CPlateIdentifyDoc::CPlateIdentifyDoc()
{
	// TODO: �ڴ����һ���Թ������
    //���캯���б�����ʼ��
     m_image=NULL;                         
     gray=NULL;
     slipimage=NULL;

}

CPlateIdentifyDoc::~CPlateIdentifyDoc()
{
     //�ͷ���Դ
     cvReleaseImage(&m_image);
     cvReleaseImage(&gray);
     cvReleaseImage(&slipimage);
}

BOOL CPlateIdentifyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CPlateIdentifyDoc ���л�

void CPlateIdentifyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CPlateIdentifyDoc ���

#ifdef _DEBUG
void CPlateIdentifyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlateIdentifyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPlateIdentifyDoc ����

BOOL CPlateIdentifyDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  �ڴ������ר�õĴ�������
    //ʹ��opencv���ļ�
    if(0==(m_image=cvLoadImage(lpszPathName,CV_LOAD_IMAGE_ANYCOLOR)))
        return FALSE;
    else
    {
        m_Cimage.mSetImg(m_image);

        return TRUE;
    }
}

BOOL CPlateIdentifyDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: �ڴ����ר�ô����/����û���
        
    //ʹ��opencv�����ļ�   
    m_Cimage.Save(lpszPathName);
    return TRUE;
}
/********************************************************************************
   �ҶȻ�����
   ����opencv

*********************************************************************************/
void CPlateIdentifyDoc::OnGray()
{
    // TODO: �ڴ���������������
    gray = cvCreateImage( cvGetSize(m_image), 8, 1 );
    cvCvtColor(m_image,gray,CV_BGR2GRAY);
    m_Cimage.mSetImg(gray);
    //�ĵ����ݸı�֪ͨ��ͼ�ı䴰��
    UpdateAllViews(NULL);
}

/********************************************************************************
   �Ҷ����촦��
   ����opencv

*********************************************************************************/
void CPlateIdentifyDoc::OnGraystrenth()
{
    // TODO: �ڴ���������������
    ImageStretchByHistogram(gray,gray); 
    m_Cimage.mSetImg(gray);
    UpdateAllViews(NULL);

}
/********************************************************************************
   �Ҷ���ǿ����
   �ɻҶ�ͼ�����ֵ���쵽0-255����ֱ��ͼ����ͼ����ǿ
*********************************************************************************/
int ImageStretchByHistogram(IplImage *src,IplImage *dst)
{
    assert(src->width==dst->width);
    float p[256],p1[256],num[256];
    //�����������
    memset(p,0,sizeof(p));
    memset(p1,0,sizeof(p1));
    memset(num,0,sizeof(num));

    int height=src->height;
    int width=src->width;
    long wMulh = height * width;

    //����ͼ������Ҷȼ����ֵĴ���
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            num[v]++;
        }
    }

    //����ͼ������Ҷȼ��ĳ��ָ���
    for(int i=0;i<256;i++)
    {
        p[i]=num[i]/wMulh;
    }

    //���Ÿ����Ҷȼ�֮ǰ�ĸ��ʺ�
    for(int i=0;i<256;i++)
    {
        for(int k=0;k<=i;k++)
            p1[i]+=p[k];
    }

    //ֱ��ͼ�任
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            ((uchar*)(dst->imageData + dst->widthStep*y))[x]= p1[v]*255+0.5;           
        }
    }

    return 0;

} 
/********************************************************************************
   ͼ���Ե������
   ����opencv��Canny����
*********************************************************************************/
void CPlateIdentifyDoc::OnEdgeadd()
{
    // TODO: �ڴ���������������
    cvCanny(gray,gray,50,150,3);
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);

}
/********************************************************************************
   ͼ���ֵ������
   ����opencv
*********************************************************************************/
void CPlateIdentifyDoc::OnBinary()
{
    // TODO: �ڴ���������������
    cvThreshold( gray, gray, 0,255,CV_THRESH_OTSU );
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);
}
/********************************************************************************
   ���ƶ�λ
  
*********************************************************************************/
void CPlateIdentifyDoc::OnPosition()
{
    // TODO: �ڴ���������������
    //���ƶ�λ
  CvScalar s;                                       //���ڶ�ȡͼ��Ķ�ֵֵ
  int imgBottom,imgTop,imgLeft,imgRight;            //���ƵĶ������ײ�����ࡢ�Ҳ������
  int *imgResult;
  int imgHeight= gray->height;
  int imgWidth= gray->width;
  imgResult=new int[imgHeight];
  imgBottom=imgHeight;
  imgTop=0;
  imgLeft=0;
  imgRight=imgWidth;
  bool tag=FALSE;
  CString str;
  //��ͼ���1/2����4/5��������ˮƽͶӰ���г���12�������ʱ����Ϊ�ǳ���λ��ȷ��ˮƽλ��
   for(int ht=imgHeight/2;ht<imgHeight*0.8;ht+=3)
   {
       imgResult[ht]=0;
       for(int wt=0;wt<gray->width;wt++)
       {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                imgResult[ht]++;
            }
       }
       if((!tag)&&imgResult[ht]>12)
       {
           str.Format("%d",imgResult[ht]);
            AfxMessageBox(str);
           imgTop=ht;
           tag=TRUE;
       }
       if(tag&&(ht>imgTop+8)&&imgResult[ht]<12)  
       {
           imgBottom=ht;
           tag=FALSE;
           break;
       }
   }

  
   //��ͼ����ൽ�Ҳ���������ֱͶӰ���г���10�������ʱ����Ϊ�ǳ���λ��ȷ����ֱλ��
   CvRect selection;                                
   for(int wt_new=gray->width/3;wt_new<gray->width;wt_new++)
   {
       imgResult[wt_new]=0;
       for(int ht_new=imgTop;ht_new<imgBottom;ht_new++)
       {
            s=cvGet2D(gray,ht_new,wt_new);
            if(255==s.val[0])
            {
                imgResult[wt_new]++;
            }
       }
       if((!tag)&&((imgResult[wt_new]>10)||(imgResult[wt_new]>imgResult[wt_new-1]+5)))
       {
           imgLeft=wt_new;
           break;
       }
       
   }     
   
   //ȷ�����Ƶ�λ������
   selection.x=imgLeft+30;       
   selection.y=imgTop;
   selection.width=120;
   selection.height=imgBottom-imgTop-10;
   cvSetImageROI(m_image, selection);
   cvCopy(m_image,m_image);
   m_Cimage.mSetImg(m_image);
   UpdateAllViews(NULL);

}
/********************************************************************************
   ��̬ѧ����ʴ����
   ����opencv
*********************************************************************************/
void CPlateIdentifyDoc::OnState()
{
    // TODO: �ڴ�������������
    //��̬ѧ����

    IplConvKernel *element=cvCreateStructuringElementEx(3,1,0.5,0.5,CV_SHAPE_RECT,0);
    cvMorphologyEx( gray, gray, NULL,element, CV_MOP_CLOSE, 8);

    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);
}
/********************************************************************************
   ���Ƶ���бУ��
   ͨ������ͼ���ǰ�벿�ֵ�б�ʺͺ�벿�ֵ�б�ʽ��е���Ȼ��ͼ��ӳ�䵽�µ�λ����
*********************************************************************************/
void CPlateIdentifyDoc::OnRevise()
{
    // TODO: �ڴ���������������
    CString str;
    CvScalar s,s_new;
    double num=0;
    double leftaverage=0;
    double rightaverage=0;
    int iHeight=gray->height;
    int iWidth=gray->width;
    double slope=0;
    int pix_new;
    //����ǰ�벿��б��
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=0;wt<iWidth/2;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(0==s.val[0])
            {
                num+=iWidth/2-wt;
                leftaverage+=ht*(iWidth/2-wt);
            }
        }
    }
    leftaverage/=num;
    num=0;
    //�����벿��б��
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=iWidth/2;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(0==s.val[0])
            {
                num+=iWidth-wt;
                rightaverage+=ht*(iWidth-wt);
            }
        }
    }
    rightaverage/=num;
    slope=(rightaverage-leftaverage)/ (iWidth/2);
    str.Format("%f",slope);
    AfxMessageBox(str);
    //ͼ��ӳ��
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            pix_new=(ht-(wt-iWidth/2)*slope);
            s=cvGet2D(gray,ht,wt);
            
            if(pix_new<=0||iHeight<=pix_new)
            {
                
                s.val[0]=255;
                cvSet2D(gray,ht,wt,s);
            }
            else
            {
                s_new=cvGet2D(gray,pix_new,wt);
                s_new.val[0]=s.val[0];
                cvSet2D(gray,pix_new,wt,s_new);

            }
            
        }

    }
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);

}
/********************************************************************************
   ���Ƶ��ַ��ָ�
   ͨ������ͼ���ǰ�벿�ֵ�б�ʺͺ�벿�ֵ�б�ʽ��е���Ȼ��ͼ��ӳ�䵽�µ�λ����
*********************************************************************************/

void CPlateIdentifyDoc::OnSplit()
{
    // TODO: �ڴ���������������
    FILE *fp;
    int iHeight=gray->height;
    int iWidth=gray->width;
    IplImage * graytemp=NULL;
    CvRect result_rect;
    graytemp=cvCreateImage ( cvGetSize(gray), 8, 1 );
    cvCopy(gray,graytemp);
    result=cvCreateImage( cvSize(g_width*7,g_height), 8, 1 );
    //graytemp=cvCreateImage( cvGetSize(gray), 8, 1 );
    CvRect selection;
    CvScalar s;
    CRect rect;
    rect.bottom=iHeight;
    rect.top=0;
    rect.left=0;
    rect.right=iWidth;
    CvSize dst_cvsize; 
    dst_cvsize.width=g_width;
    dst_cvsize.height=g_height;
    //���ļ������ָ���ַ������ļ���
    if((fp=fopen("test_set.txt","w"))==NULL)
   {
       AfxMessageBox("�����ļ�ʧ��");
       exit(0);

   }

    //������һ���ڵ�ȷ����������λ��
    for(int ht=0;ht<iHeight;ht++) 
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                //�ڵ�
                rect.top=ht;
                ht=iHeight;
                break;
            }
        }
    }
    //������һ���ڵ�ȷ���ײ�����λ��
    for(int ht=iHeight-1;ht>=0;ht--)
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                rect.bottom=ht;
                ht=-1;
                break;
            }
        }
    }

    bool lab=FALSE;                                  //�Ƿ����һ���ַ��ָ�״̬
    bool black=FALSE;                                //�Ƿ��ֺڵ�
    bool change=FALSE;
     int num=0;
    for(int wt=0;wt<iWidth;wt++) 
    {
       
        int cout=0;
        for(int ht=0;ht<iHeight;ht++)
        {
            s=cvGet2D(gray,ht,wt);
            if((255==s.val[0])&&(!change))
            {
                cout++;
                change=TRUE;
            }
            else if((0==s.val[0])&&(change)) 
            {
                cout++;
                change=FALSE;
            }
        }
        
        if(!lab&&(cout>5))
            {
                
                rect.left=wt-3;                      //�������߽�
                lab=TRUE;                            //�ַ��ָʼ
             }
        if(wt==iWidth-1)
            break;
        if(lab&&(cout<5)&&(wt>(rect.left+8))&&num<7)
        {
           
            rect.right=wt+2;
            lab=FALSE; 
            CvPoint  pt1,pt2;                                            //����ÿ���ַ�������
            pt1.x=rect.left;
            pt1.y=rect.top;
            pt2.x=rect.right;
            pt2.y=rect.bottom; 
            selection.x=pt1.x+1;       
            selection.y=pt1.y;
            selection.width=rect.right-rect.left+1;
            selection.height=rect.bottom-rect.top;
            cvSetImageROI(graytemp, selection);
            //slipimage=slipimage+num;
            slipimage=cvCreateImage( cvGetSize(graytemp), 8, 1 );
            cvCopy(graytemp,slipimage);                                  //���ƶ�������  
            cvResetImageROI(graytemp);
            IplImage *dst = cvCreateImage( dst_cvsize, 8, 1);           //��ŷָ����ַ�
            cvResize(slipimage, dst, CV_INTER_LINEAR);                  //����Դͼ��Ŀ��ͼ���һ����6*7
            CvScalar s_tem;
            char ch;
            //��ͼ���һ�������Ϣд���ļ�����Ϊ�ַ�ʶ��Ĳ��Լ�
            for(int ht=0;ht<g_height;ht++) 
           {
               
            for(int wt=0;wt<g_width;wt++)
               {
                    s_tem=cvGet2D(dst,ht,wt);
                    if(255==s_tem.val[0])
                    {
                        ch='0';
                    }
                    else 
                        ch=' ';
                    fputc(ch, fp);

               }
               fputc('\r', fp);
               fputc('\n', fp);
            }
            result_rect=cvRect(num*g_width,0,g_width,g_height);         
            cvSetImageROI(result,result_rect);
            cvCopy(dst,result);
            cvResetImageROI(result);
            cvReleaseImage(&dst);
            num++;
            cvRectangle(m_image,pt1,pt2,CV_RGB(255,0,0),1,8,0) ;        //��ÿ���ַ��ÿ��ʾ����
            m_Cimage.mSetImg(m_image);
	        UpdateAllViews(NULL);
        
        }
    }
    cvReleaseImage(&graytemp);
    fclose(fp);
   
}


//���º���Ϊʵ��������
 /****************************************************************************** 
        ��������� 
******************************************************************************/ 

//����α��������� 
void InitializeRandoms()  
{ 
  srand(4711); 
} 
//����һ��LOW - TOP֮���α������� 
int RandomEqualINT(int Low, int High)  
{ 
  return rand() % (High-Low+1) + Low; 
}      
//����һ��LOW - TOP֮���α��������� 
double RandomEqualREAL(double Low, double High) 
{ 
  return ((double) rand() / RAND_MAX) * (High-Low) + Low; 
} 
/****************************************************************************** 
  //�ر��ļ�          
******************************************************************************/ 
void FinalizeApplication(NET* Net) 
{ 
  fclose(f); 
}      
/****************************************************************************** 
//�����������Ȩ            
******************************************************************************/ 
void RandomWeights(NET* Net)  
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
void SaveWeights(NET* Net) 
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
void RestoreWeights(NET* Net)                
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
void GenerateNetwork(NET* Net) 
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
void InitializeApplication(NET* Net)  
{ 
  int  n, i,j;      
  
  for (n=0; n<NUM_DATA; n++) { 
      for (i=0; i<Y; i++) { 
          for (j=0; j<X; j++) { 
              if ( Pattern[n][i][j] == '0' ) 
                    Input[n][i*X+j]  = HI ; 
                else Input[n][i*X+j]  =LO ;  
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
void SimulateNet(NET* Net, double* Input, double* Target, bool Training,bool Protocoling) 
{ 
  double Output[M]; //������¼�������� 
  SetInput(Net, Input,Protocoling);//��������㣬������������ 
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
void SetInput(NET* Net, double* Input,bool Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->InputLayer->Units; i++) {        
    Net->InputLayer->Output[i] = Input[i-1];      //��������� 
  } 
  if (Protocoling) { 
    WriteInput(Net, Input);//����Protocolingֵ�������ģʽ 
  } 
} 
/****************************************************************************** 
//���㵱ǰ������������upper Ϊ��ǰ�㣬LOWERΪǰһ��                    
******************************************************************************/ 
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper)  
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
void PropagateNet(NET* Net) 
{ 
  int l; 
  
  for (l=0; l<NUM_LAYERS-1; l++) { 
    PropagateLayer(Net, Net->Layer[l], Net->Layer[l+1]); 
  } 
} 
/****************************************************************************** 
//������������              
******************************************************************************/ 
void GetOutput(NET* Net, double* Output,bool Protocoling) 
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
void ComputeOutputError(NET* Net, double* Target)  
{ 
  int  i; 
  double  Err,Out; 
  
  Net->Error = 0; 
  for (i=1; i<=Net->OutputLayer->Units; i++) { 
    Out = Net->OutputLayer->Output[i];//��������� 
    Err = Target[i-1]-Out;//������ 
	
    Net->OutputLayer->Error[i] = Out * (1-Out) * Err; 
        //��delta�����������Ϊ���˿ɵ���s�κ��� 
    Net->Error += 0.5 * sqr(Err);//ƽ���ʽ 
  } 
} 
/****************************************************************************** 
  //���򴫲� Upper Ϊǰ�㣬LowerΪ��� ������ֵ���Ϊǰ��            
******************************************************************************/ 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower)  
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
void BackpropagateNet(NET* Net) 
{ 
  int l;//ѭ������ 
  
  for (l=NUM_LAYERS-1; l>1; l--) { 
    BackpropagateLayer(Net, Net->Layer[l], Net->Layer[l-1]);//��ÿ�㴦�� 
  } 
} 
/****************************************************************************** 
              //��������ÿһ�������Ȩ 
******************************************************************************/ 
void AdjustWeights(NET* Net)    
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
void WriteInput(NET* Net, double* Input)  
{ 
    /*
  int i; 
  
  for (i=0; i<N; i++) { 
    if (i%X == 0) { 
      fprintf(f, "\\n"); 
    } 
    fprintf(f, "%c", (Input[i] == HI) ?'0' : ' '); 
  } 
  fprintf(f, " -> "); 
  */
} 
/****************************************************************************** 
//��ʾ���ģʽ              
******************************************************************************/ 
void WriteOutput(NET* Net, double* Output) 
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
  if(Index==0)    
     fputs("��",f);//д���ļ� 
  if(Index==1)    
     fputs("A",f);//д���ļ�  
  if(Index==2)    
     fputs("U",f);//д���ļ�   
  if(Index==3)    
     fputs("E",f);//д���ļ�
   if(Index==4)    
     fputs("8",f);//д���ļ� 
   if(Index==5)    
     fputs("8",f);//д���ļ� 
   if(Index==6)    
     fputs("6",f);//д���ļ� 
} 
/****************************************************************************** 
              ��ʼ���������� 
******************************************************************************/ 
void Initializetest() 
{ 
    int n,i,j;//ѭ������ 
    
    for (n=0; n<NUM_DATA; n++) { 
        for (i=0; i<Y; i++) { 
            for (j=0; j<X; j++) {                               
                if (testPattern[n][i][j]=='0')
                    Inputtest[n][i*X+j] = HI; 
                else Inputtest[n][i*X+j] =LO;  //NUM_DATA����ģʽ�������X*Y����Ԫ                                    
            } 
        }
     }
}
/****************************************************************************** 
             ���ļ�����Ŀ������ 
******************************************************************************/ 
void TegetData(char taget[NUM_DATA][M],double tagetdouble[NUM_DATA][M])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("teget_num.txt","r"))==NULL)
   {
       AfxMessageBox("��Ŀ���ļ�ʧ��");
        exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<M;j++)
         {
             ch=fgetc(fp);
             while(ch=='\n' || ch=='\r'||ch==' ')
             {
                  ch=fgetc(fp);
              }
             taget[i][j]=ch;
         }
     }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<M;j++)
         {
             if(taget[i][j]=='H')
             {
                 tagetdouble[i][j]=HI;
             }
             else
                 tagetdouble[i][j]=LO;
         }
     }
    fclose(fp);

}
/****************************************************************************** 
              ���ļ��ж���ѵ�������� 
******************************************************************************/ 
void GetSwatch(char swatch[NUM_DATA][Y][X])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("train_num.txt","r"))==NULL)
   {
        AfxMessageBox("��ѵ�����ļ�ʧ��");
         exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<Y;j++)
         {                
             for(int k=0;k<X;k++)
             {
                ch=fgetc(fp);
                while(ch=='\n' || ch=='\r')
                    {
                        ch=fgetc(fp);
                    }
                 swatch[i][j][k]=ch;
              }
           }
     }
     fclose(fp);

}
/****************************************************************************** 
              ���ļ��ж������������� 
******************************************************************************/ 
void GetTest(char swatch11[NUM_DATA][Y][X])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("test_set.txt","r"))==NULL)
   {
        AfxMessageBox("�򿪲��Լ��ļ�ʧ��");
       exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<Y;j++)
         {                
             for(int k=0;k<X;k++)
             {
                ch=fgetc(fp);
                while(ch=='\n' || ch=='\r')
                    {
                        ch=fgetc(fp);
                    }
                 swatch11[i][j][k]=ch;
              }
           }
     }
      fclose(fp);
}
/****************************************************************************** 
              ѵ��BP������ 
******************************************************************************/ 
void CPlateIdentifyDoc::OnNettrain()
{
    NET Net;
    char   Target1[NUM_DATA][M];
    int  m,n,count;                                 //ѭ������ 
    bool Stop;                                      //ѧϰ�Ƿ�����Ŀ��Ʊ��� 
  double Error;                                     //��¼��ǰ����������������
  GetSwatch(Pattern);                               //���ļ��ж�ȡѵ������
  GetTest(testPattern);                             //���ı��ж����Լ�
  TegetData(Target1,Target);                        //���ļ��ж�ȡĿ��
  InitializeRandoms();                              //��������� 
  GenerateNetwork(&Net);                            //�������粢��ʼ�����磬����ռ� 
  RandomWeights(&Net);                              //��ʼ����������Ȩ 
  InitializeApplication(&Net);                      //��ʼ������㣬��ѧϰ����ת��������ģʽ 
  count=0;                                          //��ʾѧϰ���ȵĿ��Ʊ��� 
  do { 
    Error = 0;                                      //��� 
    Stop = true;                                    //��ʼ�� 
    for (n=0; n<NUM_DATA; n++) {                    //��ÿ��ģʽ����ģ����������� 
      SimulateNet(&Net, Input[n],Target[n], false, false);  //����ģ�����������
      Error = MAX(Error, Net.Error);                //��ȡ�ṹ��ֵ,��ȡ������ֵ 
      Stop = Stop && (Net.Error < Net.Epsilon); 
      count++; 
    } 
    Error = MAX(Error, Net.Epsilon);                //����:��ֹ���,��֤��100%��ʱ��ֹͣѵ������ȡ������ֵ 
    if (count%1000==0){ 
        break;
    }
    if (! Stop) { 
        for (m=0; m<10*NUM_DATA; m++) {             //�Ը�ģʽ����ѵ�� 
        n = RandomEqualINT(0,NUM_DATA-1);           //���ѡ��ѵ��ģʽ    
        SimulateNet(&Net, Input[n],Target[n], true,false ); 
      } 
    }    
  } while (! Stop);  
  AfxMessageBox("BP������ѵ������\n"); 
  SaveWeights(&Net);                                //ѧϰ�����󱣴汦�������Ȩ  
  Initializetest();                                 //��ʼ����������������ת��������ģʽ
  for (int n=0; n<NUM_DATA; n++) { 
    SimulateNet(&Net, Inputtest[n],Target[n], false, true); //�����д���ļ�   
  } 
  FinalizeApplication(&Net);                        //�ر��ļ�   
}
/****************************************************************************** 
             ��һ����Ӧ������������ڷָ���ֻ����ʾ���� 
******************************************************************************/ 

void CPlateIdentifyDoc::OnGuiyi()
{
    // TODO: �ڴ�������������
       m_Cimage.mSetImg(result);
	   UpdateAllViews(NULL);
}
/****************************************************************************** 
             �ַ��ָ�����ѵ����������ʱд�뱣�������ļ������ļ��ж�ȡ��� 
******************************************************************************/ 

void CPlateIdentifyDoc::OnChartIdentify()
{
    // TODO: �ڴ���������������
  
  AfxMessageBox("����Ѵ��뵱ǰĿ¼��result.txt�д��ļ��������");
  FILE *fp;
    char ch ;
     if((fp=fopen("result.txt","r"))==NULL)
   {
        AfxMessageBox("��ѵ�����ļ�ʧ��");
         exit(0);

   }
    char buff[9];
    fgets(buff,sizeof(buff),fp); 
    AfxMessageBox(buff);
}
