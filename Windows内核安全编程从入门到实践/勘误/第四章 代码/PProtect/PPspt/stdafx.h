// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#define Debug 1             //ע�͵�������ȥ��־��ӡ��Ϣ

// Windows ͷ�ļ�:
#include <windows.h>
#include "stdio.h"
#include "wchar.h"
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include "Mscat.h"
#include "Tlhelp32.h"

#include "ioctl.h"
#include "PPspt.h"


#pragma comment(lib,"Advapi32.lib")
#pragma comment (lib, "wintrust")

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

extern "C"
{
#include "miracl.h"
}

#pragma comment(lib,"ms32.lib")

#pragma comment(linker,"/NODEFAULTLIB:LIBCMT")
#pragma comment(linker,"/NODEFAULTLIB:LIBC")
