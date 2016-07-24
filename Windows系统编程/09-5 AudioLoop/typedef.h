// Number of buffers
#define NUM_LEVEL_BUFFERS		10

//#define UPDATES_PER_SEC     20
#define BUFFER_SIZE_G7231		480   //for g.723.1
#define BUFFER_SIZE_G729		160   //for g.729

#define MSG_AUDIO_DATA			WM_USER+1

///////////////////////////////////////////////
//used in the file "MainFrm.cpp"

typedef struct _RecordPlayInfor 
{
	LPWAVEHDR	alpWave[NUM_LEVEL_BUFFERS];
    HWAVEIN		hWaveIn;
	HWAVEOUT	hWaveOut;
    int			buffersize;

	HANDLE		hThreadIn;
	DWORD		idThreadIn;
	
	HANDLE		hThreadOut;
	DWORD		idThreadOut;

//	short   record;
//	short   play;

} RECORDPLAYINFOR, FAR * PRECORDPLAYINFOR;

////////////////////////////////////////////////////////////////////////////////
//used in the file "test_audio_loopView.cpp"
//used for the record  loop
typedef struct  _AudioStreamData
{
	LPWAVEHDR HWave[10]; //���ӵ����������������ݿ�,����Ϊ2��
	PCMWAVEFORMAT  FAR *pwf;//������ʽָ��
	HWAVEIN hwavIn;//����������
	HWAVEOUT hwavOut; //��������ľ��
}  AUDIOSTREAMDATA, FAR * PAUDIOSTREAMDATA;

 
