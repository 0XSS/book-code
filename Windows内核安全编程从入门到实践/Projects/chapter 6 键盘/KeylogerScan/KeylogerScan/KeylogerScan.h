#ifndef _KeylogerScan_H_
#define _KeylogerScan_H_

#include "ntddkbd.h"
#include "wmilib.h"

extern POBJECT_TYPE *IoDeviceObjectType;

typedef unsigned char boolean;

typedef struct _CONTEXT_DATA_ {
    PDRIVER_OBJECT KbdClassDrvObj;      //KbdClass����������
    PDEVICE_OBJECT KbdClass0DevObj;     //KeyboardClass0�豸����

    PVOID KbdClassDriverFileData;       //����Ļ����������ڱ�������PE�ļ�����kbdclass.sys

    ULONG KbdClassDriverEntryAddr;      //DriverEntry���ļ�ƫ��
    ULONG KbdClassReadAddr;             //KeyboardClassRead����ʵ��ַ
    ULONG KbdServiceCallbackAddr;       //KeyboardClassServiceCallback����ʵ��ַ

} ContextData, *PContextData;


NTKERNELAPI
NTSTATUS
ObReferenceObjectByName(
    IN PUNICODE_STRING ObjectName,
    IN ULONG           Attributes,
    IN PACCESS_STATE   PassedAccessState OPTIONAL,
    IN ACCESS_MASK     DesiredAccess OPTIONAL,
    IN POBJECT_TYPE    ObjectType OPTIONAL,
    IN KPROCESSOR_MODE AccessMode,
    IN OUT PVOID       ParseContext OPTIONAL,
    OUT    PVOID       *Object
);

void KeylogerScanUnload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS ScanKeyloger();

PContextData InitContextData();

VOID DeInitContextData(PContextData Data);

PDEVICE_OBJECT GetKbdClass0DeviceObject();

NTSTATUS ScanKeyboardFilterDriver(PContextData Data);

NTSTATUS ScanDispatchReadHook(PContextData Data);

NTSTATUS ScanServiceCallbackHook(PContextData Data);

boolean ScanKbdServiceCallbackEntryHook(PContextData Data);

boolean ScanInlineHook(PContextData Data, PVOID FnRAW);

boolean RecoverKnrlMemory(PVOID Dest, PVOID Source, ULONG Offset);

#endif