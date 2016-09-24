/*++
Copyright (C) Microsoft Corporation, 1991 - 1999

Module Name:

    diskperf.c

Abstract:

    This driver monitors disk accesses capturing performance data.

Environment:

    kernel mode only

Notes:

--*/


#define INITGUID

#include "ntddk.h"
#include "ntdddisk.h"
#include "stdarg.h"
#include "stdio.h"
#include <ntddvol.h>

#include <mountdev.h>
#include "wmistr.h"
#include "wmidata.h"
#include "wmiguid.h"
#include "wmilib.h"

#include "ntstrsafe.h"

#ifdef POOL_TAGGING
    #ifdef ExAllocatePool
        #undef ExAllocatePool
    #endif
    #define ExAllocatePool(a,b) ExAllocatePoolWithTag(a,b,'frPD')
#endif

#define DISKPERF_MAXSTR         64

#define DBG 1
//
// Device Extension
//

typedef struct _DEVICE_EXTENSION {

    //
    // Back pointer to device object
    //

    PDEVICE_OBJECT DeviceObject;

    //
    // Target Device Object
    //

    PDEVICE_OBJECT TargetDeviceObject;

    //
    // Physical device object
    //
    PDEVICE_OBJECT PhysicalDeviceObject;

    //
    // Disk number for reference in WMI
    //

    ULONG       DiskNumber;

    //
    // If device is enabled for counting always
    //

    LONG        EnabledAlways;

    //
    // Use to keep track of Volume info from ntddvol.h
    //

    WCHAR StorageManagerName[8];

    //
    // Disk performance counters
    // and locals used to compute counters
    //

    ULONG   Processors;
    PDISK_PERFORMANCE DiskCounters;    // per processor counters
    LARGE_INTEGER LastIdleClock;
    LONG QueueDepth;

    //
    // must synchronize paging path notifications
    //
    KEVENT PagingPathCountEvent;
    LONG  PagingPathCount;

    //
    // Physical Device name
    //

    UNICODE_STRING PhysicalDeviceName;
    WCHAR PhysicalDeviceNameBuffer[DISKPERF_MAXSTR];

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define DEVICE_EXTENSION_SIZE sizeof(DEVICE_EXTENSION)
#define PROCESSOR_COUNTERS_SIZE FIELD_OFFSET(DISK_PERFORMANCE, QueryTime)

/*
Layout of Per Processor Counters is a contiguous block of memory:
    Processor 1
+-----------------------+     +-----------------------+
|PROCESSOR_COUNTERS_SIZE| ... |PROCESSOR_COUNTERS_SIZE|
+-----------------------+     +-----------------------+
where PROCESSOR_COUNTERS_SIZE is less than sizeof(DISK_PERFORMANCE) since
we only put those we actually use for counting.
*/

UNICODE_STRING DiskPerfRegistryPath;


//
// Function declarations
//

DRIVER_INITIALIZE DriverEntry;

DRIVER_ADD_DEVICE DiskPerfAddDevice;

DRIVER_DISPATCH DiskPerfForwardIrpSynchronous;

__drv_dispatchType(IRP_MJ_PNP)
DRIVER_DISPATCH DiskPerfDispatchPnp;

__drv_dispatchType(IRP_MJ_POWER)
DRIVER_DISPATCH DiskPerfDispatchPower;

DRIVER_DISPATCH DiskPerfSendToNextDriver;

__drv_dispatchType(IRP_MJ_CREATE)
DRIVER_DISPATCH DiskPerfCreate;

__drv_dispatchType(IRP_MJ_READ)
__drv_dispatchType(IRP_MJ_WRITE)
DRIVER_DISPATCH DiskPerfReadWrite;

__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DiskPerfDeviceControl;

__drv_dispatchType(IRP_MJ_FLUSH_BUFFERS)
__drv_dispatchType(IRP_MJ_SHUTDOWN)
DRIVER_DISPATCH DiskPerfShutdownFlush;

DRIVER_DISPATCH DiskPerfStartDevice;
DRIVER_DISPATCH DiskPerfRemoveDevice;

IO_COMPLETION_ROUTINE DiskPerfIoCompletion;
IO_COMPLETION_ROUTINE DiskPerfIrpCompletion;

DRIVER_UNLOAD DiskPerfUnload;

VOID
DiskPerfLogError(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG UniqueId,
    IN NTSTATUS ErrorCode,
    IN NTSTATUS Status
);

NTSTATUS
DiskPerfRegisterDevice(
    IN PDEVICE_OBJECT DeviceObject
);

VOID
DiskPerfSyncFilterWithTarget(
    IN PDEVICE_OBJECT FilterDevice,
    IN PDEVICE_OBJECT TargetDevice
);


VOID
DiskPerfAddCounters(
    IN OUT PDISK_PERFORMANCE TotalCounters,
    IN PDISK_PERFORMANCE NewCounters,
    IN LARGE_INTEGER Frequency
);

#if DBG

ULONG DiskPerfDebug = 0;

VOID
DiskPerfDebugPrint(
    ULONG DebugPrintLevel,
    PCCHAR DebugMessage,
    ...
);

#define DebugPrint(x)   DiskPerfDebugPrint x

#else

#define DebugPrint(x)

#endif

//
// Define the sections that allow for discarding (i.e. paging) some of
// the code.
//

#ifdef ALLOC_PRAGMA
    #pragma alloc_text (INIT, DriverEntry)
    #pragma alloc_text (PAGE, DiskPerfCreate)
    #pragma alloc_text (PAGE, DiskPerfAddDevice)
    #pragma alloc_text (PAGE, DiskPerfDispatchPnp)
    #pragma alloc_text (PAGE, DiskPerfStartDevice)
    #pragma alloc_text (PAGE, DiskPerfRemoveDevice)
    #pragma alloc_text (PAGE, DiskPerfUnload)
    #pragma alloc_text (PAGE, DiskPerfRegisterDevice)
    #pragma alloc_text (PAGE, DiskPerfSyncFilterWithTarget)
#endif


#define USE_PERF_CTR

#ifdef USE_PERF_CTR
    #define DiskPerfGetClock(a, b) (a) = KeQueryPerformanceCounter((b))
#else
    #define DiskPerfGetClock(a, b) KeQuerySystemTime(&(a))
#endif


NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
)
{
    ULONG               ulIndex;
    PDRIVER_DISPATCH  * dispatch;
    //
    // ����ע�����Ϣ����RegistryPath��Ϣ��ȫ�ֱ���DiskPerfRegistryPath��
    //
    DiskPerfRegistryPath.MaximumLength = RegistryPath->Length
                                         + sizeof(UNICODE_NULL);
    DiskPerfRegistryPath.Buffer = ExAllocatePool(
                                      PagedPool,
                                      DiskPerfRegistryPath.MaximumLength);

    if (DiskPerfRegistryPath.Buffer != NULL) {
        RtlCopyUnicodeString(&DiskPerfRegistryPath, RegistryPath);
    } else {
        DiskPerfRegistryPath.Length = 0;
        DiskPerfRegistryPath.MaximumLength = 0;
    }

    KdPrint(("[DriverEntry] DiskPerfRegistryPath:%wZ\n", &DiskPerfRegistryPath));

    //
    // ��д�ַ�����
    //
    for (ulIndex = 0, dispatch = DriverObject->MajorFunction;
         ulIndex <= IRP_MJ_MAXIMUM_FUNCTION;
         ulIndex++, dispatch++) {
        *dispatch = DiskPerfSendToNextDriver;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE]          = DiskPerfCreate;
    DriverObject->MajorFunction[IRP_MJ_READ]            = DiskPerfReadWrite;
    DriverObject->MajorFunction[IRP_MJ_WRITE]           = DiskPerfReadWrite;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = DiskPerfDeviceControl;
    DriverObject->MajorFunction[IRP_MJ_SHUTDOWN]        = DiskPerfShutdownFlush;
    DriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS]   = DiskPerfShutdownFlush;
    DriverObject->MajorFunction[IRP_MJ_PNP]             = DiskPerfDispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_POWER]           = DiskPerfDispatchPower;
    DriverObject->DriverExtension->AddDevice            = DiskPerfAddDevice;
    DriverObject->DriverUnload                          = DiskPerfUnload;
    return (STATUS_SUCCESS);
}

#define FILTER_DEVICE_PROPOGATE_FLAGS            0
#define FILTER_DEVICE_PROPOGATE_CHARACTERISTICS (FILE_REMOVABLE_MEDIA |  \
        FILE_READ_ONLY_DEVICE | \
        FILE_FLOPPY_DISKETTE    \
                                                )

VOID
DiskPerfSyncFilterWithTarget(
    IN PDEVICE_OBJECT FilterDevice,
    IN PDEVICE_OBJECT TargetDevice
)
{
    ULONG                   propFlags;
    PAGED_CODE();
    //
    //  Ϊ�����豸�������Ŀ���豸���������õı�ǡ�����ع�������������Щ���
    //  �жϴ����豸�Ƿ��ǿ��ƶ���
    //
    propFlags = TargetDevice->Flags & FILTER_DEVICE_PROPOGATE_FLAGS;
    FilterDevice->Flags |= propFlags;
    propFlags = TargetDevice->Characteristics & FILTER_DEVICE_PROPOGATE_CHARACTERISTICS;
    FilterDevice->Characteristics |= propFlags;
}

NTSTATUS
DiskPerfAddDevice(
    IN PDRIVER_OBJECT DriverObject,
    IN PDEVICE_OBJECT PhysicalDeviceObject
)
{
    NTSTATUS                status;
    PDEVICE_OBJECT          filterDeviceObject;
    PDEVICE_EXTENSION       deviceExtension;
    PCHAR                   buffer;
    ULONG                   buffersize;
    PAGED_CODE();
    //
    // ����һ�������豸
    //
    DebugPrint((2, "DiskPerfAddDevice: Driver %X PhysicalDeviceObject %X\n",
                DriverObject, PhysicalDeviceObject));
    status = IoCreateDevice(DriverObject,
                            DEVICE_EXTENSION_SIZE,
                            NULL,
                            FILE_DEVICE_DISK,
                            FILE_DEVICE_SECURE_OPEN,
                            FALSE,
                            &filterDeviceObject);

    if (!NT_SUCCESS(status)) {
        DebugPrint((1, "DiskPerfAddDevice: Cannot create filterDeviceObject\n"));
        return status;
    }

    //��֤�ʹ��󶨵��豸��PartMgr�����д��ʽ��DIRECT_IO����д���һ��
    filterDeviceObject->Flags |= DO_DIRECT_IO;
    deviceExtension = (PDEVICE_EXTENSION) filterDeviceObject->DeviceExtension;
    RtlZeroMemory(deviceExtension, DEVICE_EXTENSION_SIZE);
    //��ȡ��ǰʱ��Ϊ��ʼ����ʱ�䣬��ʹ��������������豸����ʱ��
    DiskPerfGetClock(deviceExtension->LastIdleClock, NULL);
    DebugPrint((10, "DiskPerfAddDevice: LIC=%I64u\n",
                deviceExtension->LastIdleClock));
    //
    // Ϊÿ��CPU����һ���������ܼ������ṹ
    //
    // ע�⣺Ϊ�˽�ʡ�ڴ棬���÷�������DISK_PERFORMANCE��С���ڴ棬
    // ����QueryTime��ı����ò��������ԾͲ��÷�����
    //
    //ʹ����������ѡ���ȡ��ǰCPU������
#if (NTDDI_VERSION >= NTDDI_WIN7)
    deviceExtension->Processors = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
#elif (NTDDI_VERSION >= NTDDI_VISTA)
    deviceExtension->Processors = KeQueryActiveProcessorCount(NULL);
#else
    deviceExtension->Processors = KeNumberProcessors;
#endif
    buffersize = PROCESSOR_COUNTERS_SIZE * deviceExtension->Processors;
    buffer = (PCHAR) ExAllocatePool(NonPagedPool, buffersize);

    if (buffer != NULL) {
        RtlZeroMemory(buffer, buffersize);
        deviceExtension->DiskCounters = (PDISK_PERFORMANCE) buffer;
    } else {
        DiskPerfLogError(
            filterDeviceObject,
            513,
            STATUS_SUCCESS,
            IO_ERR_INSUFFICIENT_RESOURCES);
    }

    //
    // �����ɵĹ����豸�󶨵�PDO���豸ջ�У�һ���ǰ󶨵�PartMgr�����豸��
    //
    deviceExtension->PhysicalDeviceObject = PhysicalDeviceObject;
    deviceExtension->TargetDeviceObject =
        IoAttachDeviceToDeviceStack(filterDeviceObject, PhysicalDeviceObject);

    if (deviceExtension->TargetDeviceObject == NULL) {
        ExFreePool(deviceExtension->DiskCounters);
        deviceExtension->DiskCounters = NULL;
        IoDeleteDevice(filterDeviceObject);
        DebugPrint((1, "DiskPerfAddDevice: Unable to attach %X to target %X\n",
                    filterDeviceObject, PhysicalDeviceObject));
        return STATUS_NO_SUCH_DEVICE;
    }

    deviceExtension->DeviceObject = filterDeviceObject;
    deviceExtension->PhysicalDeviceName.Buffer
        = deviceExtension->PhysicalDeviceNameBuffer;
    //��ʼ���¼������¼�����PnP�ķ�ҳ��������
    KeInitializeEvent(&deviceExtension->PagingPathCountEvent,
                      NotificationEvent, TRUE);
    filterDeviceObject->Flags |=  DO_POWER_PAGABLE;
    filterDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    return STATUS_SUCCESS;
}


NTSTATUS
DiskPerfDispatchPnp(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PIO_STACK_LOCATION  irpSp = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS            status;
    PDEVICE_EXTENSION   deviceExtension;
    PAGED_CODE();
    DebugPrint((2, "DiskPerfDispatchPnp: Device %X Irp %X\n",
                DeviceObject, Irp));

    switch (irpSp->MinorFunction) {
    case IRP_MN_START_DEVICE:
        //
        //�����豸��������
        //
        DebugPrint((3,
                    "DiskPerfDispatchPnp: Schedule completion for START_DEVICE"));
        status = DiskPerfStartDevice(DeviceObject, Irp);
        break;

    case IRP_MN_REMOVE_DEVICE: {
        //
        // �����豸�Ƴ�����
        //
        DebugPrint((3,
                    "DiskPerfDispatchPnp: Schedule completion for REMOVE_DEVICE"));
        status = DiskPerfRemoveDevice(DeviceObject, Irp);
        break;
    }

    case IRP_MN_DEVICE_USAGE_NOTIFICATION: {
        //
        //�������IRP����ȷ��Ŀ���豸�Ƿ�֧�������ļ�����ҳ�ļ���ת���ļ��������ļ�
        //
        PIO_STACK_LOCATION irpStack;
        BOOLEAN setPagable;
        DebugPrint((3,
                    "DiskPerfDispatchPnp: Processing DEVICE_USAGE_NOTIFICATION"));
        irpStack = IoGetCurrentIrpStackLocation(Irp);

        //ֻ�����ҳ�ļ����������·�
        if (irpStack->Parameters.UsageNotification.Type != DeviceUsageTypePaging) {
            status = DiskPerfSendToNextDriver(DeviceObject, Irp);
            break;
        }

        deviceExtension = DeviceObject->DeviceExtension;
        //
        // ����Ĵ����ǵ��͵�PnP��ҳ�ļ����������
        //
        status = KeWaitForSingleObject(&deviceExtension->PagingPathCountEvent,
                                       Executive, KernelMode,
                                       FALSE, NULL);
        setPagable = FALSE;

        if (!irpStack->Parameters.UsageNotification.InPath &&
            deviceExtension->PagingPathCount == 1) {
            if (DeviceObject->Flags & DO_POWER_INRUSH) {
                DebugPrint((3, "DiskPerfDispatchPnp: last paging file "
                            "removed but DO_POWER_INRUSH set, so not "
                            "setting PAGABLE bit "
                            "for DO %p\n", DeviceObject));
            } else {
                DebugPrint((2, "DiskPerfDispatchPnp: Setting  PAGABLE "
                            "bit for DO %p\n", DeviceObject));
                DeviceObject->Flags |= DO_POWER_PAGABLE;
                setPagable = TRUE;
            }
        }

        status = DiskPerfForwardIrpSynchronous(DeviceObject, Irp);

        if (NT_SUCCESS(status)) {
            IoAdjustPagingPathCount(
                &deviceExtension->PagingPathCount,
                irpStack->Parameters.UsageNotification.InPath);

            if (irpStack->Parameters.UsageNotification.InPath) {
                if (deviceExtension->PagingPathCount == 1) {
                    DebugPrint((3, "DiskPerfDispatchPnp: Clearing PAGABLE bit "
                                "for DO %p\n", DeviceObject));
                    DeviceObject->Flags &= ~DO_POWER_PAGABLE;
                }
            }
        } else {
            if (setPagable == TRUE) {
                DeviceObject->Flags &= ~DO_POWER_PAGABLE;
                setPagable = FALSE;
            }
        }

        KeSetEvent(&deviceExtension->PagingPathCountEvent,
                   IO_NO_INCREMENT, FALSE);
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
        break;
    }

    default:
        DebugPrint((3,
                    "DiskPerfDispatchPnp: Forwarding irp"));
        return DiskPerfSendToNextDriver(DeviceObject, Irp);
    }

    return status;
}


NTSTATUS
DiskPerfIrpCompletion(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp,
    IN PVOID Context
)
{
    PKEVENT Event = (PKEVENT) Context;
    UNREFERENCED_PARAMETER(DeviceObject);
    UNREFERENCED_PARAMETER(Irp);
    //���õȴ��¼�
    KeSetEvent(Event, IO_NO_INCREMENT, FALSE);
    //
    //����STATUS_MORE_PROCESSING_REQUIRED�����˴���ɲ�����ֹ��
    //Ҫ��������������IRP
    //
    return (STATUS_MORE_PROCESSING_REQUIRED);
}


NTSTATUS
DiskPerfStartDevice(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PDEVICE_EXTENSION   deviceExtension;
    NTSTATUS            status;
    PAGED_CODE();
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    //ͬ���·�����
    status = DiskPerfForwardIrpSynchronous(DeviceObject, Irp);
    DiskPerfSyncFilterWithTarget(DeviceObject,
                                 deviceExtension->TargetDeviceObject);
    //ע���豸�����򵥵Ļ�ȡһЩ����Ϣ��
    DiskPerfRegisterDevice(DeviceObject);
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}


NTSTATUS
DiskPerfRemoveDevice(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    NTSTATUS            status;
    PDEVICE_EXTENSION   deviceExtension;
    PAGED_CODE();
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    //ͬ���·�IRP
    status = DiskPerfForwardIrpSynchronous(DeviceObject, Irp);

    if (deviceExtension->DiskCounters) {
        ExFreePool(deviceExtension->DiskCounters);
    }

    //����󶨲�ɾ�������豸
    IoDetachDevice(deviceExtension->TargetDeviceObject);
    IoDeleteDevice(DeviceObject);
    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}


NTSTATUS
DiskPerfSendToNextDriver(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)

/*++

Routine Description:

    This routine sends the Irp to the next driver in line
    when the Irp is not processed by this driver.

Arguments:

    DeviceObject
    Irp

Return Value:

    NTSTATUS

--*/

{
    PDEVICE_EXTENSION   deviceExtension;
    IoSkipCurrentIrpStackLocation(Irp);
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    return IoCallDriver(deviceExtension->TargetDeviceObject, Irp);
} // end DiskPerfSendToNextDriver()

NTSTATUS
DiskPerfDispatchPower(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PDEVICE_EXTENSION deviceExtension;
#if (NTDDI_VERSION < NTDDI_VISTA)
    PoStartNextPowerIrp(Irp);
    IoSkipCurrentIrpStackLocation(Irp);
    deviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    return PoCallDriver(deviceExtension->TargetDeviceObject, Irp);
#else
    IoSkipCurrentIrpStackLocation(Irp);
    deviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    return IoCallDriver(deviceExtension->TargetDeviceObject, Irp);
#endif
} // end DiskPerfDispatchPower

NTSTATUS
DiskPerfForwardIrpSynchronous(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PDEVICE_EXTENSION   deviceExtension;
    KEVENT event;
    NTSTATUS status;
    KeInitializeEvent(&event, NotificationEvent, FALSE);
    deviceExtension = (PDEVICE_EXTENSION) DeviceObject->DeviceExtension;
    //
    // ����ջ��Ԫ
    //
    IoCopyCurrentIrpStackLocationToNext(Irp);
    //
    // ������ɺ���
    //
    IoSetCompletionRoutine(Irp, DiskPerfIrpCompletion,
                           &event, TRUE, TRUE, TRUE);
    //
    // �·�����
    //
    status = IoCallDriver(deviceExtension->TargetDeviceObject, Irp);

    //
    // �ȴ����
    //

    if (status == STATUS_PENDING) {
        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
        status = Irp->IoStatus.Status;
    }

    return status;
}


NTSTATUS
DiskPerfCreate(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PAGED_CODE();
    UNREFERENCED_PARAMETER(DeviceObject);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
} // end DiskPerfCreate()


NTSTATUS
DiskPerfReadWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)
{
    PDEVICE_EXTENSION  deviceExtension   = DeviceObject->DeviceExtension;
    PIO_STACK_LOCATION currentIrpStack   = IoGetCurrentIrpStackLocation(Irp);
#if (NTDDI_VERSION >= NTDDI_WIN7)
    ULONG              processor         = KeGetCurrentProcessorNumberEx(NULL);
#else
    ULONG              processor         = KeGetCurrentProcessorNumber();
#endif
    PDISK_PERFORMANCE  partitionCounters = NULL;
    LONG               queueLen;
    PLARGE_INTEGER     timeStamp;

    //
    // ���ڴ������������ܶ�̬���ӣ�
    // ������Ҫ��֤��ǰ�������Ƿ񱻳�ʼ�����豸���ܼ�����
    //

    if (deviceExtension->DiskCounters != NULL &&
        processor < deviceExtension->Processors) {
        partitionCounters = (PDISK_PERFORMANCE)
                            ((PCHAR) deviceExtension->DiskCounters
                             + (processor * PROCESSOR_COUNTERS_SIZE));
    }

    //
    // �豸��û�г�ʼ����ϣ�ֱ���·���IRP
    //
    if (deviceExtension->PhysicalDeviceNameBuffer[0] == 0 ||
        partitionCounters == NULL) {
        KdPrint(("[DiskPerfReadWrite] DiskPerfSendToNextDriver\n"));
        return DiskPerfSendToNextDriver(DeviceObject, Irp);
    }

    //
    // ����������еĳ���
    //
    queueLen = InterlockedIncrement(&deviceExtension->QueueDepth);
    //
    // ������ǰI/Oջ��Ԫ���²�ջ��Ԫ
    //
    IoCopyCurrentIrpStackLocationToNext(Irp);
    //
    // ��¼��ǰ��ʱ���
    //
    timeStamp = (PLARGE_INTEGER) &currentIrpStack->Parameters.Read;
    DiskPerfGetClock(*timeStamp, NULL);
    DebugPrint((10, "DiskPerfReadWrite: TS=%I64u\n", *timeStamp));

    //
    //���������г���Ϊ1��˵����������ǰ�豸һֱ���ڿ���״̬��
    //��¼�¿���ʱ��
    //
    if (queueLen == 1) {
        partitionCounters->IdleTime.QuadPart
        += timeStamp->QuadPart -
           deviceExtension->LastIdleClock.QuadPart;
        deviceExtension->LastIdleClock.QuadPart = timeStamp->QuadPart;
    }

    //
    // ������ɺ���������IRP���ձ���ɵ�ʱ�򽫻���ø���ɺ�����
    //
    IoSetCompletionRoutine(Irp,
                           DiskPerfIoCompletion,
                           DeviceObject,
                           TRUE,
                           TRUE,
                           TRUE);
    //
    // �·���IRP
    //
    return IoCallDriver(deviceExtension->TargetDeviceObject,
                        Irp);
}

NTSTATUS
DiskPerfIoCompletion(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp,
    IN PVOID          Context
)
{
    PDEVICE_EXTENSION  deviceExtension   = DeviceObject->DeviceExtension;
    PIO_STACK_LOCATION irpStack          = IoGetCurrentIrpStackLocation(Irp);
#if (NTDDI_VERSION >= NTDDI_WIN7)
    ULONG              processor         = KeGetCurrentProcessorNumberEx(NULL);
#else
    ULONG              processor         = KeGetCurrentProcessorNumber();
#endif
    PDISK_PERFORMANCE  partitionCounters = NULL;
    LARGE_INTEGER      timeStampComplete;
    PLARGE_INTEGER     difference;
    LONG               queueLen;
    UNREFERENCED_PARAMETER(Context);

    if (Irp->PendingReturned) {
        IoMarkIrpPending(Irp);
    }

    //
    // ��ȡ��ǰʱ����������㵱ǰʱ�䣨��IRP���ʱ�䣩�͸�IRP����ʱ���ʱ���
    //
    difference = (PLARGE_INTEGER) &irpStack->Parameters.Read;
    DiskPerfGetClock(timeStampComplete, NULL);
    difference->QuadPart = timeStampComplete.QuadPart - difference->QuadPart;
    DebugPrint((10, "DiskPerfIoCompletion: TS=%I64u diff %I64u\n",
                timeStampComplete, difference->QuadPart));
    //
    // ��С��д������г���
    //
    queueLen = InterlockedDecrement(&deviceExtension->QueueDepth);

    if (queueLen < 0) {
        queueLen = InterlockedIncrement(&deviceExtension->QueueDepth);
    }

    if (queueLen == 0) {
        deviceExtension->LastIdleClock = timeStampComplete;
    }

    //
    //
    // ����CPU��صĴ�����������
    //

    if (deviceExtension->DiskCounters != NULL &&
        processor < deviceExtension->Processors) {
        partitionCounters = (PDISK_PERFORMANCE)
                            ((PCHAR) deviceExtension->DiskCounters
                             + (processor * PROCESSOR_COUNTERS_SIZE));
    }

    if (partitionCounters == NULL) {
        return STATUS_SUCCESS;
    };

    if (irpStack->MajorFunction == IRP_MJ_READ) {
        //
        // �����ζ�����ʵ����ɵ��ֽ����ӵ��ܵļ�¼�ֽ�����
        //
        partitionCounters->BytesRead.QuadPart += Irp->IoStatus.Information;
        //
        // ����һ�ζ�������
        //
        partitionCounters->ReadCount++;
        //
        // �����ζ����󻨷ѵ�ʱ��ӵ��ܵĶ����󻨷�ʱ����
        //
        partitionCounters->ReadTime.QuadPart += difference->QuadPart;
        DebugPrint((11, "Added RT delta %I64u total %I64u qlen=%d\n",
                    difference->QuadPart, partitionCounters->ReadTime.QuadPart,
                    queueLen));
    } else {
        //
        // ������ʵ��д����ֽ������ӵ��ܵļ�¼����
        //
        partitionCounters->BytesWritten.QuadPart += Irp->IoStatus.Information;
        //
        // ����һ��д������
        //
        partitionCounters->WriteCount++;
        //
        // ������д���󻨷ѵ�ʱ��ӵ��ܵ�д���󻨷ѵ�ʱ����
        //
        partitionCounters->WriteTime.QuadPart += difference->QuadPart;
        DebugPrint((11, "Added WT delta %I64u total %I64u qlen=%d\n",
                    difference->QuadPart, partitionCounters->WriteTime.QuadPart,
                    queueLen));
    }

    if (Irp->Flags & IRP_ASSOCIATED_IRP) {
        partitionCounters->SplitCount++;
    }

    return STATUS_SUCCESS;
}


NTSTATUS
DiskPerfDeviceControl(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
)

{
    PDEVICE_EXTENSION  deviceExtension = DeviceObject->DeviceExtension;
    PIO_STACK_LOCATION currentIrpStack = IoGetCurrentIrpStackLocation(Irp);
    DebugPrint((2, "DiskPerfDeviceControl: DeviceObject %X Irp %X\n",
                DeviceObject, Irp));

    if (currentIrpStack->Parameters.DeviceIoControl.IoControlCode ==
        IOCTL_DISK_PERFORMANCE) {
        NTSTATUS        status;

        //
        // ȷ���ṩ�Ļ������㹻��
        //

        if (currentIrpStack->Parameters.DeviceIoControl.OutputBufferLength <
            sizeof(DISK_PERFORMANCE)) {
            status = STATUS_BUFFER_TOO_SMALL;
            Irp->IoStatus.Information = 0;
        } else {
            ULONG i;
            PDISK_PERFORMANCE totalCounters;
            PDISK_PERFORMANCE diskCounters = deviceExtension->DiskCounters;
            LARGE_INTEGER frequency, perfctr;

            if (diskCounters == NULL) {
                Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
                IoCompleteRequest(Irp, IO_NO_INCREMENT);
                return STATUS_UNSUCCESSFUL;
            }

            totalCounters = (PDISK_PERFORMANCE) Irp->AssociatedIrp.SystemBuffer;
            RtlZeroMemory(totalCounters, sizeof(DISK_PERFORMANCE));
#ifdef USE_PERF_CTR
            perfctr = KeQueryPerformanceCounter(&frequency);
#endif
            KeQuerySystemTime(&totalCounters->QueryTime);

            //
            //��ÿ��CPU��صĴ������������ۼƸ�Ӧ�ó����ṩ�Ļ�����
            //
            for (i = 0; i < deviceExtension->Processors; i++) {
                DiskPerfAddCounters(totalCounters, diskCounters, frequency);
                diskCounters = (PDISK_PERFORMANCE)
                               ((PCHAR) diskCounters + PROCESSOR_COUNTERS_SIZE);
            }

            totalCounters->QueueDepth = deviceExtension->QueueDepth;

            //
            //���QueueDepthΪ0,�������豸����ʱ��
            //
            if (totalCounters->QueueDepth == 0) {
                LARGE_INTEGER difference;
                difference.QuadPart =
#ifdef USE_PERF_CTR
                    perfctr.QuadPart
#else
                    totalCounters->QueryTime.QuadPart
#endif
                    - deviceExtension->LastIdleClock.QuadPart;

                if (difference.QuadPart > 0) {
                    totalCounters->IdleTime.QuadPart +=
#ifdef USE_PERF_CTR
                        10000000 * difference.QuadPart / frequency.QuadPart;
#else
                        difference.QuadPart;
#endif
                }
            }

            totalCounters->StorageDeviceNumber
                = deviceExtension->DiskNumber;
            RtlCopyMemory(
                &totalCounters->StorageManagerName[0],
                &deviceExtension->StorageManagerName[0],
                8 * sizeof(WCHAR));
            status = STATUS_SUCCESS;
            Irp->IoStatus.Information = sizeof(DISK_PERFORMANCE);
        }

        //
        // Complete request.
        //
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    } else {
        //
        //  ����������ȫ���·�����������д���ʵ���Ͼ���IoSkipCurrentIrpStackLocation
        //
        Irp->CurrentLocation++,
            Irp->Tail.Overlay.CurrentStackLocation++;
        return IoCallDriver(deviceExtension->TargetDeviceObject, Irp);
    }
}


NTSTATUS
DiskPerfShutdownFlush(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
)

/*++

Routine Description:

    This routine is called for a shutdown and flush IRPs.  These are sent by the
    system before it actually shuts down or when the file system does a flush.

Arguments:

    DriverObject - Pointer to device object to being shutdown by system.
    Irp          - IRP involved.

Return Value:

    NT Status

--*/

{
    PDEVICE_EXTENSION  deviceExtension = DeviceObject->DeviceExtension;
    DebugPrint((2, "DiskPerfShutdownFlush: DeviceObject %X Irp %X\n",
                DeviceObject, Irp));
    IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(deviceExtension->TargetDeviceObject, Irp);
} // end DiskPerfShutdownFlush()


VOID
DiskPerfUnload(
    IN PDRIVER_OBJECT DriverObject
)

/*++

Routine Description:

    Free all the allocated resources, etc.

Arguments:

    DriverObject - pointer to a driver object.

Return Value:

    VOID.

--*/
{
    PAGED_CODE();
    UNREFERENCED_PARAMETER(DriverObject);
    return;
}


NTSTATUS
DiskPerfRegisterDevice(
    IN PDEVICE_OBJECT DeviceObject
)
{
    NTSTATUS                status;
    IO_STATUS_BLOCK         ioStatus;
    KEVENT                  event;
    PDEVICE_EXTENSION       deviceExtension;
    PIRP                    irp;
    STORAGE_DEVICE_NUMBER   number;
    ULONG                   registrationFlag = 0;
    PAGED_CODE();
    DebugPrint((2, "DiskPerfRegisterDevice: DeviceObject %X\n",
                DeviceObject));
    deviceExtension = DeviceObject->DeviceExtension;
    KeInitializeEvent(&event, NotificationEvent, FALSE);
    //
    // ��ȡ�豸��,����һ�����ݽṹ��STORAGE_DEVICE_NUMBER����
    // �������豸���͡��豸�š�������
    //
    //����һ����ѯIRP
    irp = IoBuildDeviceIoControlRequest(
              IOCTL_STORAGE_GET_DEVICE_NUMBER,
              deviceExtension->TargetDeviceObject,
              NULL,
              0,
              &number,
              sizeof(number),
              FALSE,
              &event,
              &ioStatus);

    if (!irp) {
        DiskPerfLogError(
            DeviceObject,
            256,
            STATUS_SUCCESS,
            IO_ERR_INSUFFICIENT_RESOURCES);
        DebugPrint((3, "DiskPerfRegisterDevice: Fail to build irp\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = IoCallDriver(deviceExtension->TargetDeviceObject, irp);

    //ͬ����ɸ�IRP
    if (status == STATUS_PENDING) {
        KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
        status = ioStatus.Status;
    }

    if (NT_SUCCESS(status)) {
        //
        // ��������豸��
        //
        deviceExtension->DiskNumber = number.DeviceNumber;
        //
        // ΪĿ��PDO�����豸������\Device\Harddisk0\\Partition0��\Device\Harddisk1\\Partition0
        //
        KdPrint(("DiskPerfRegisterDevice: Create device name for each partition\n"));
        RtlStringCbPrintfW(
            deviceExtension->PhysicalDeviceNameBuffer,
            sizeof(deviceExtension->PhysicalDeviceNameBuffer),
            L"\\Device\\Harddisk%d\\Partition%d",
            number.DeviceNumber, number.PartitionNumber);
        RtlInitUnicodeString(&deviceExtension->PhysicalDeviceName, &deviceExtension->PhysicalDeviceNameBuffer[0]);
        //
        // Ϊ�����������Ĭ����
        //
        RtlCopyMemory(
            &(deviceExtension->StorageManagerName[0]),
            L"PhysDisk",
            8 * sizeof(WCHAR));
        KdPrint(("DiskPerfRegisterDevice: Device name %ws\n",
                 deviceExtension->PhysicalDeviceNameBuffer));
    } else {
        //
        // �����ȡ�����豸��Ϣʧ�ܣ����Ի�ȡ���豸��Ϣ
        //
        ULONG           outputSize = sizeof(MOUNTDEV_NAME);
        PMOUNTDEV_NAME  output;
        VOLUME_NUMBER   volumeNumber;
        ULONG           nameSize;
        output = ExAllocatePool(PagedPool, outputSize);

        if (!output) {
            DiskPerfLogError(
                DeviceObject,
                257,
                STATUS_SUCCESS,
                IO_ERR_INSUFFICIENT_RESOURCES);
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        KeInitializeEvent(&event, NotificationEvent, FALSE);
        //����һ�������ܺ�ΪIOCTL_MOUNTDEV_QUERY_DEVICE_NAME�Ĳ�ѯIRP
        irp = IoBuildDeviceIoControlRequest(
                  IOCTL_MOUNTDEV_QUERY_DEVICE_NAME,
                  deviceExtension->TargetDeviceObject, NULL, 0,
                  output, outputSize, FALSE, &event, &ioStatus);

        if (!irp) {
            ExFreePool(output);
            DiskPerfLogError(
                DeviceObject,
                258,
                STATUS_SUCCESS,
                IO_ERR_INSUFFICIENT_RESOURCES);
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        status = IoCallDriver(deviceExtension->TargetDeviceObject, irp);

        //ͬ����ɸ�IRP
        if (status == STATUS_PENDING) {
            KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
            status = ioStatus.Status;
        }

        if (status == STATUS_BUFFER_OVERFLOW) {
            //
            //������ڴ治�������·��䲢��ѯ
            //
            outputSize = sizeof(MOUNTDEV_NAME) + output->NameLength;
            ExFreePool(output);
            output = ExAllocatePool(PagedPool, outputSize);

            if (!output) {
                DiskPerfLogError(
                    DeviceObject,
                    258,
                    STATUS_SUCCESS,
                    IO_ERR_INSUFFICIENT_RESOURCES);
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            KeInitializeEvent(&event, NotificationEvent, FALSE);
            irp = IoBuildDeviceIoControlRequest(
                      IOCTL_MOUNTDEV_QUERY_DEVICE_NAME,
                      deviceExtension->TargetDeviceObject, NULL, 0,
                      output, outputSize, FALSE, &event, &ioStatus);

            if (!irp) {
                ExFreePool(output);
                DiskPerfLogError(
                    DeviceObject, 259,
                    STATUS_SUCCESS,
                    IO_ERR_INSUFFICIENT_RESOURCES);
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            status = IoCallDriver(deviceExtension->TargetDeviceObject, irp);

            if (status == STATUS_PENDING) {
                KeWaitForSingleObject(
                    &event,
                    Executive,
                    KernelMode,
                    FALSE,
                    NULL
                );
                status = ioStatus.Status;
            }
        }

        if (!NT_SUCCESS(status)) {
            ExFreePool(output);
            DiskPerfLogError(
                DeviceObject,
                260,
                STATUS_SUCCESS,
                IO_ERR_CONFIGURATION_ERROR);
            return status;
        }

        //
        // ���ڻ�ȡ���Ǿ�����û�еõ�DiskNumber������ֱ������-1
        //
        deviceExtension->DiskNumber = (ULONG) - 1;
        nameSize = min(output->NameLength, sizeof(deviceExtension->PhysicalDeviceNameBuffer) - sizeof(WCHAR));
        RtlStringCbCopyW(deviceExtension->PhysicalDeviceNameBuffer, nameSize, output->Name);
        RtlInitUnicodeString(&deviceExtension->PhysicalDeviceName, &deviceExtension->PhysicalDeviceNameBuffer[0]);
        ExFreePool(output);
        //
        // ��ȡVOLUME_NUMBER�ṹ��Ϣ��������ź;����������
        //
        outputSize = sizeof(VOLUME_NUMBER);
        RtlZeroMemory(&volumeNumber, sizeof(VOLUME_NUMBER));
        KeInitializeEvent(&event, NotificationEvent, FALSE);
        irp = IoBuildDeviceIoControlRequest(
                  IOCTL_VOLUME_QUERY_VOLUME_NUMBER,
                  deviceExtension->TargetDeviceObject, NULL, 0,
                  &volumeNumber,
                  sizeof(VOLUME_NUMBER),
                  FALSE, &event, &ioStatus);

        if (!irp) {
            DiskPerfLogError(
                DeviceObject,
                265,
                STATUS_SUCCESS,
                IO_ERR_INSUFFICIENT_RESOURCES);
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        status = IoCallDriver(deviceExtension->TargetDeviceObject, irp);

        if (status == STATUS_PENDING) {
            KeWaitForSingleObject(&event, Executive,
                                  KernelMode, FALSE, NULL);
            status = ioStatus.Status;
        }

        //��ȡʧ�ܾ�����Ĭ�ϵĴ洢����������ΪLogiDisk��
        //��������Ϊ��ȡ�ĵ����������
        if (!NT_SUCCESS(status) ||
            volumeNumber.VolumeManagerName[0] == (WCHAR) UNICODE_NULL) {
            RtlCopyMemory(
                &deviceExtension->StorageManagerName[0],
                L"LogiDisk",
                8 * sizeof(WCHAR));

            if (NT_SUCCESS(status))
                deviceExtension->DiskNumber = volumeNumber.VolumeNumber;
        } else {
            RtlCopyMemory(
                &deviceExtension->StorageManagerName[0],
                &volumeNumber.VolumeManagerName[0],
                8 * sizeof(WCHAR));
            deviceExtension->DiskNumber = volumeNumber.VolumeNumber;
        }

        DebugPrint((3, "DiskPerfRegisterDevice: Device name %ws\n",
                    deviceExtension->PhysicalDeviceNameBuffer));
    }

    return status;
}


VOID
DiskPerfLogError(
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG UniqueId,
    IN NTSTATUS ErrorCode,
    IN NTSTATUS Status
)

/*++

Routine Description:

    Routine to log an error with the Error Logger

Arguments:

    DeviceObject - the device object responsible for the error
    UniqueId     - an id for the error
    Status       - the status of the error

Return Value:

    None

--*/

{
    PIO_ERROR_LOG_PACKET errorLogEntry;
    errorLogEntry = (PIO_ERROR_LOG_PACKET)
                    IoAllocateErrorLogEntry(
                        DeviceObject,
                        (UCHAR)(sizeof(IO_ERROR_LOG_PACKET) + sizeof(DEVICE_OBJECT))
                    );

    if (errorLogEntry != NULL) {
        errorLogEntry->ErrorCode = ErrorCode;
        errorLogEntry->UniqueErrorValue = UniqueId;
        errorLogEntry->FinalStatus = Status;
        //
        // The following is necessary because DumpData is of type ULONG
        // and DeviceObject can be more than that
        //
        RtlCopyMemory(
            &errorLogEntry->DumpData[0],
            DeviceObject,
            sizeof(DEVICE_OBJECT));
        errorLogEntry->DumpDataSize = sizeof(DEVICE_OBJECT);
        IoWriteErrorLogEntry(errorLogEntry);
    }
}

VOID
DiskPerfAddCounters(
    IN OUT PDISK_PERFORMANCE TotalCounters,
    IN PDISK_PERFORMANCE NewCounters,
    IN LARGE_INTEGER Frequency
)
{
    TotalCounters->BytesRead.QuadPart   += NewCounters->BytesRead.QuadPart;
    TotalCounters->BytesWritten.QuadPart += NewCounters->BytesWritten.QuadPart;
    TotalCounters->ReadCount            += NewCounters->ReadCount;
    TotalCounters->WriteCount           += NewCounters->WriteCount;
    TotalCounters->SplitCount           += NewCounters->SplitCount;
#ifdef USE_PERF_CTR

    if (Frequency.QuadPart > 0) {
        TotalCounters->ReadTime.QuadPart    +=
            NewCounters->ReadTime.QuadPart * 10000000 / Frequency.QuadPart;
        TotalCounters->WriteTime.QuadPart   +=
            NewCounters->WriteTime.QuadPart * 10000000 / Frequency.QuadPart;
        TotalCounters->IdleTime.QuadPart    +=
            NewCounters->IdleTime.QuadPart * 10000000 / Frequency.QuadPart;
    } else
#endif
    {
        TotalCounters->ReadTime.QuadPart    += NewCounters->ReadTime.QuadPart;
        TotalCounters->WriteTime.QuadPart   += NewCounters->WriteTime.QuadPart;
        TotalCounters->IdleTime.QuadPart    += NewCounters->IdleTime.QuadPart;
    }
}

#if DBG

VOID
DiskPerfDebugPrint(
    ULONG DebugPrintLevel,
    PCCHAR DebugMessage,
    ...
)

/*++

Routine Description:

    Debug print for all DiskPerf

Arguments:

    Debug print level between 0 and 3, with 3 being the most verbose.

Return Value:

    None

--*/

{
    va_list ap;
    va_start(ap, DebugMessage);

    if ((DebugPrintLevel <= (DiskPerfDebug & 0x0000ffff)) ||
        ((1 << (DebugPrintLevel + 15)) & DiskPerfDebug)) {
        DbgPrint(DebugMessage, ap);
    }

    DbgPrint(DebugMessage, ap);
    va_end(ap);
}
#endif
