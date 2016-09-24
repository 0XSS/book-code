#include "stdafx.h"

//
//��kbdclass.sys�ļ����ݶ�ȡ���ڴ��У��Ժ󽫵���
//GetReadKbdClassDriverEntryAddrFromPE�Ⱥ�����ȡԭʼ��ַ
//
PVOID ReadKbdClassDriverFileData()
{
    NTSTATUS status;
    HANDLE hFile;
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING usKbdClass;
    IO_STATUS_BLOCK ioStatus;
    PFILE_STANDARD_INFORMATION pFileInfo;
    PVOID pFileData;
    RtlInitUnicodeString(&usKbdClass, L"\\SystemRoot\\System32\\Drivers\\kbdclass.sys");
    InitializeObjectAttributes(&oa, &usKbdClass, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = ZwCreateFile(
                 &hFile,
                 GENERIC_READ,
                 &oa,
                 &ioStatus,
                 NULL,
                 FILE_ATTRIBUTE_NORMAL,
                 FILE_SHARE_READ,
                 FILE_OPEN,
                 FILE_SYNCHRONOUS_IO_NONALERT,
                 NULL,
                 0
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwCreateFile Failure!err:0x%08x\n",
                 status));
        return NULL;
    }

    pFileInfo = (PFILE_STANDARD_INFORMATION)ExAllocatePoolWithTag(
                    NonPagedPool,
                    sizeof(FILE_STANDARD_INFORMATION),
                    'wjl'
                );

    if (NULL == pFileInfo) {
        ZwClose(hFile);
        return NULL;
    }

    //��ȡ�ļ��Ĵ�С
    status = ZwQueryInformationFile(
                 hFile,
                 &ioStatus,
                 pFileInfo,
                 sizeof(FILE_STANDARD_INFORMATION),
                 FileStandardInformation
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwQueryInformationFile Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        return NULL;
    }

    pFileData = ExAllocatePoolWithTag(
                    PagedPool,
                    pFileInfo->AllocationSize.u.LowPart,
                    'wjl'
                );

    if (NULL == pFileData) {
        KdPrint(("[ReadKbdClassDriverFileData] ExAllocatePoolWithTag Failure!\n"));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        return NULL;
    }

    //��ȡ����
    status = ZwReadFile(hFile, NULL, NULL, NULL, &ioStatus, pFileData,
                        pFileInfo->AllocationSize.u.LowPart, NULL, NULL);

    if (!NT_SUCCESS(status)) {
        KdPrint(("[ReadKbdClassDriverFileData] ZwReadFile Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        ExFreePoolWithTag(pFileInfo, 'wjl');
        ExFreePoolWithTag(pFileData, 'wjl');
        return NULL ;
    }

    ZwClose(hFile);
    ExFreePoolWithTag(pFileInfo, 'wjl');
    return pFileData;
}

//
//��PE�ļ��л�ȡDrierEntry��ԭʼ��ַ������ʵ��ַ��
//�����Ǵ�GsDriverEntry������JMP DriverEntry
//ע�⣬��������ǰ������������ڵ�GsDriverEntry��ַ��ʵ��Ч
//
boolean GetReadKbdClassDriverEntryAddrFromPE(PContextData Data)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    ULONG uGsDriverEntry = 0, uGsDriverEntryRawOffset = 0;
    ULONG uDelta = 0, uCodeLen = 0, uFnLen = 0, uOffset = 0;
    char *pCode;
    ULONG uDriverEntryAddr = 0;
    INSTRUCTION inst;
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //��ȡGsDriverEntry�ĵ�ַ���������ж�
    uGsDriverEntry = (ULONG)Data->KbdClassDrvObj->DriverInit;

    if (uGsDriverEntry < (ULONG)Data->KbdClassDrvObj->DriverStart
        || uGsDriverEntry > (ULONG)Data->KbdClassDrvObj->DriverStart + Data->KbdClassDrvObj->DriverSize) {
        KdPrint(("[GetReadKbdClassDriverEntryAddrFromPE] GsDriverEntry Hooked!\n"));
        return false;
    }

    //��ȡGsDriverEntry���ļ�ƫ��
    uGsDriverEntryRawOffset = uGsDriverEntry - (ULONG)Data->KbdClassDrvObj->DriverStart;
    uGsDriverEntryRawOffset = RVA2RAW(Data->KbdClassDriverFileData, uGsDriverEntryRawOffset);
    uGsDriverEntry = uGsDriverEntryRawOffset + (ULONG)Data->KbdClassDriverFileData;
    uCodeLen = GetFunctionLen(uGsDriverEntry, 0xe9);
    pCode = (char*)ExAllocatePoolWithTag(PagedPool, uCodeLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)uGsDriverEntry, uCodeLen);
    //ƫ�ƣ�����PE�ļ����ڴ��׵�ַ��PE��ѡ���ص�ַ֮��
    uOffset = (ULONG)Data->KbdClassDriverFileData - pNtHdr->OptionalHeader.ImageBase;
    //�ض�λ
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)((ULONG)Data->KbdClassDrvObj->DriverInit - (ULONG)Data->KbdClassDrvObj->DriverStart),
              (BYTE*)pCode, uCodeLen, uOffset);

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        //GsDriverEntry�����е�һ��0xe9����JMP DriverEntry
        if (inst.opcode == 0xe9) {
            //��ȡDriverEntry���ļ�ƫ��
            uDriverEntryAddr = (ULONG)Data->KbdClassDrvObj->DriverInit - (ULONG)Data->KbdClassDrvObj->DriverStart + uFnLen
                               + *(ULONG*)((ULONG)pCode + uFnLen + 1) + 5;
            uDriverEntryAddr = RVA2RAW(Data->KbdClassDriverFileData, uDriverEntryAddr);
            Data->KbdClassDriverEntryAddr = uDriverEntryAddr;
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (uDriverEntryAddr == 0) {
        return false;
    }

    return true;
}

//��PE�ļ��л�ȡKeyboardClassRead�ĵ�ַ
//����������DriverEntry�е�������mov dword [esi+0x44]
boolean GetRealKbdClassReadAddrFromPE(PContextData Data)
{
    INSTRUCTION inst;
    PVOID pCode;
    char cInstCode[50] = {0};
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0;
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);

    if (NULL == Data->KbdClassDriverEntryAddr) {
        return false;
    }

    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr), uFnLen);
    //ƫ�ƣ��ڴ���kbdclass������ʵ�ʼ��ص�ַ��PE�ļ�����ѡ���ص�ַ֮��
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    //�ض�λ
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //DriverEntry�г��ֵ�mov dword [esi+0x44]���������ļ��Ƕ���������ַ
        if (20 == RtlCompareMemory(cInstCode, "mov dword [esi+0x44]", 20)) {
            Data->KbdClassReadAddr = inst.op2.immediate;
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');
    return Data->KbdClassReadAddr ? true : false;
}

//
//��ȡKeyboardClassServiceCallback����ʵ��ַ
//�������ȴ�DriverEntry������KeyboardAddDeviceEx��ַ��
//�ٴ�KeyboardAddDeviceEx������KeyboardClassServiceCallback��ַ
//
boolean GetKbdServiceCallBackAddrFromPE(PContextData Data)
{
    ULONG uDriverEntryAddr = Data->KbdClassDriverEntryAddr;
    ULONG uKeyboardAddDeviceExAddr = 0, uKeyboardClassServiceCallback = 0;
    PVOID pCode = NULL;
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0, uPreviousLen = 0;
    PIMAGE_DOS_HEADER pDosHdr = NULL;
    PIMAGE_NT_HEADERS pNtHdr = NULL;
    INSTRUCTION inst;
    char cInstCode[50] = {0};
    int i = 0;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //
    //1.�ҵ�KeyboardAddDeviceEx��ַ
    //
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode,
                  (PVOID)((ULONG)Data->KbdClassDriverFileData + Data->KbdClassDriverEntryAddr), uFnLen);
    //ƫ�ƣ�������������KbdClass�������ڴ��ַ��PE��ѡ���ص�ַ֮��
    uOffset = (ULONG)Data->KbdClassDriverFileData - pNtHdr->OptionalHeader.ImageBase;
    RelocCode(Data->KbdClassDriverFileData,
              (PVOID)RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //���������"mov [eax+0x28]"��������һ��ָ���opcode��0xe8��
        //��ô�����ҵ���KeyboardAddDeviceEx
        if (14 == RtlCompareMemory(cInstCode, "mov [eax+0x28]", 14)) {
            get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen + uCodeLen), MODE_32);

            if (inst.opcode == 0xe8) {
                uKeyboardAddDeviceExAddr = RVA2RAW(
                                               Data->KbdClassDriverFileData,
                                               RAW2RVA(Data->KbdClassDriverFileData, Data->KbdClassDriverEntryAddr) +
                                               uFnLen + uCodeLen + 5 + inst.op1.immediate);
                //ShowDASM((ULONG)Data->KbdClassDriverFileData+uKeyboardAddDeviceExAddr);   �����������
                break;
            }
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (0 == uKeyboardAddDeviceExAddr) {
        return false;
    }

    //
    //2.�ҵ�KeyboardClassServiceCallback��ַ
    //
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + uKeyboardAddDeviceExAddr, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + uKeyboardAddDeviceExAddr), uFnLen);
    //ƫ�ƣ�kbdclass������ʵ�ʼ����׵�ַ��PE��ѡ���ػ���ַ֮��
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    RelocCode(Data->KbdClassDriverFileData, (PVOID)RAW2RVA(Data->KbdClassDriverFileData, uKeyboardAddDeviceExAddr),
              (BYTE*)pCode, uFnLen, uOffset);
    uFnLen = 0;
    i = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);

        //�����ǰָ����push esi����������ָ���opcode��0xe8����ô
        //����ָ���б�����KeyboardClassServiceCallback��ַ
        if (8 == RtlCompareMemory(cInstCode, "push esi", 8)) {
            get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen + uCodeLen), MODE_32);

            if (inst.opcode == 0xe8) {
                get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen - uPreviousLen), MODE_32);
                Data->KbdServiceCallbackAddr = inst.op1.immediate;
                break;
            }
        }

        uPreviousLen = uCodeLen;
        uFnLen += uCodeLen;
    } while (i++ < 1000);

    ExFreePoolWithTag(pCode, 'wjl');

    if (0 == Data->KbdServiceCallbackAddr) {
        return false;
    }

    return true;
}

//
//����������ַת��Ϊ�ļ�ƫ��
//
ULONG RVA2RAW(PVOID Base, ULONG RVAaddr)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    PIMAGE_SECTION_HEADER pSectionHdr;
    int i;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        KdPrint(("[RVA2RAW] IMAGE_DOS_SIGNATURE X!\n"));
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((LONG)pDosHdr + pDosHdr->e_lfanew);
    pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);

    for (i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSectionHdr++) {
        if (RVAaddr >= pSectionHdr->VirtualAddress
            && RVAaddr < pSectionHdr->VirtualAddress + pSectionHdr->Misc.VirtualSize) {
            return RVAaddr + pSectionHdr->PointerToRawData - pSectionHdr->VirtualAddress;
        }
    }

    return 0;
}

//
//���ļ�ƫ��ת��Ϊ��������ַ
//
ULONG RAW2RVA(PVOID Base, ULONG RAWaddr)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    PIMAGE_SECTION_HEADER pSectionHdr;
    int i;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        KdPrint(("[RAW2RVA] IMAGE_DOS_SIGNATURE X!\n"));
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((LONG)pDosHdr + pDosHdr->e_lfanew);
    pSectionHdr = IMAGE_FIRST_SECTION(pNtHdr);

    for (i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSectionHdr++) {
        if (RAWaddr >= pSectionHdr->PointerToRawData
            && RAWaddr < pSectionHdr->PointerToRawData + pSectionHdr->SizeOfRawData) {
            return RAWaddr + pSectionHdr->VirtualAddress - pSectionHdr->PointerToRawData;
        }
    }

    return 0;
}

//
//���㺯���ĳ���
//EndOpcode����ָ���˺����Ľ���ָ���opcode(һ�㶼��0xc2,0xc3)
//
ULONG GetFunctionLen(ULONG CodeStartAddr, BYTE EndOpcode)
{
    INSTRUCTION inst;
    ULONG uCodeLen = 0, uFnLen = 0;
    int i = 0;

    while (i++ < 1000) {
        uCodeLen = get_instruction(&inst, (BYTE*)(CodeStartAddr + uFnLen), MODE_32);

        if (uCodeLen == 0) {
            break;
        }

        uFnLen += uCodeLen;

        if (inst.opcode == EndOpcode) {
            break;
        }
    }

    return uFnLen;
}

//
//�ض�λ����
//����PE�ļ����ض�λ������Ϣ�����ض�λ
//
boolean RelocCode(PVOID Base, PVOID CodeRVA, BYTE* Code, ULONG CodeLen, ULONG Offset)
{
    PIMAGE_BASE_RELOCATION pReloc;
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNTHdr;
    ULONG uNumItem = 0, i = 0, uRelocEntry = 0;
    PUSHORT pItem;
    pDosHdr = (PIMAGE_DOS_HEADER)Base;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNTHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);

    if (IMAGE_NT_SIGNATURE != pNTHdr->Signature) {
        return false;
    }

    //��λ�ض�λ���ڷ�����ڴ��еĵ�ַ
    pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pDosHdr +
                                      RVA2RAW(
                                          Base,
                                          pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress)
                                     );

    if ((ULONG)pReloc == (ULONG)pDosHdr) {
        return false;
    }

    //���������ض�λ��
    //ע�⣬ÿ���ض�λ���С���ܲ�һ������������1k��С�����ַ��Χ���ض�λ������
    //�����ض�λ�鰴�ո���Ĺ�����Χ����ַ��С��������
    while (pReloc->SizeOfBlock) {
        if (pReloc->VirtualAddress < (ULONG)CodeRVA) {
            //������ض�λ�鸺����ض�λ��Χ��ȫ�ڴ��ض�λ��ַ��Χ���£���ô�ж���һ���ض�λ��
            if (pReloc->VirtualAddress + 0x1000 < (ULONG)CodeRVA) {
                pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pReloc + pReloc->SizeOfBlock);
                continue;
            }
        } else {
            //������ض�λ�鸺����ض�λ��Χ��ȫ�ڴ��ض�λ��ַ��Χ���ϣ�˵����Ҳ�������ض�λ��Ḻ����ض�λ
            //��ַ��Χ�ڵ��ض�λ�����ˣ�������
            if (pReloc->VirtualAddress > (ULONG)CodeRVA + CodeLen) {
                break;
            }
        }

        //
        //�ҵ���Ҫ�ض�λ�ĵ�ַ��ʹ��Offset�ض�λ
        //ÿ���ض�λ����2���ֽڣ���12λ���Ǵ��ض�λ�ĵ�ַ����Ҫ�����ض�λ��Ļ���ַ��
        uNumItem = (pReloc->SizeOfBlock - 8) / 2;
        pItem = (PUSHORT)((ULONG)pReloc + 8);

        for (i = 0; i < uNumItem; ++i) {
            if ((pItem[i] & 0xf000) == 0x3000) {
                if ((pItem[i] & 0x0fff) + pReloc->VirtualAddress >= (ULONG)CodeRVA
                    && (pItem[i] & 0x0fff) + pReloc->VirtualAddress <= (ULONG)CodeRVA + CodeLen) {
                    uRelocEntry = (ULONG)Code + (pItem[i] & 0x0fff) + pReloc->VirtualAddress - (ULONG)CodeRVA;
                    *(ULONG*)uRelocEntry += Offset;
                }
            }
        }

        pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pReloc + pReloc->SizeOfBlock);
    }

    return true;
}

//
//����������Ϣ
//
VOID ShowDASM(ULONG CodeAddr)
{
    int i = 0;
    INSTRUCTION inst;
    ULONG uFnLen = 0, uCodeLen = 0;
    char Code[50] = {0};

    while (++i <= 1000) {
        uCodeLen = get_instruction(&inst, (BYTE*)(CodeAddr + uFnLen), MODE_32);

        if (0 == uCodeLen)
            break;

        get_instruction_string(&inst, FORMAT_INTEL, 0, Code, 50);
        KdPrint(("%s\n", Code));

        if (inst.opcode == 0xc2 || inst.opcode == 0xc3)
            break;

        uFnLen += uCodeLen;
    }

    return ;
}



