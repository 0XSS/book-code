#include "StdAfx.h"
#include "myimg.h"

myimg::myimg()
{
}

void myimg::mSetImg(IplImage* pImg)
{
 m_img=cvCloneImage(pImg);  //���ܹؼ� ,����Ҳ������CImage���е�CopeOf����,������������ת������
}
myimg::~myimg(void)
{
}
