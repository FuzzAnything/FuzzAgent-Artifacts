// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Forward declarations for static helper functions
static cmsUInt32Number FuzzIO_Read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count);
static cmsBool FuzzIO_Seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset);
static cmsBool FuzzIO_Close(struct _cms_io_handler* iohandler);
static cmsUInt32Number FuzzIO_Tell(struct _cms_io_handler* iohandler);
static cmsBool FuzzIO_Write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer);

// Helper to create a context
static cmsContext CreateContext(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    (void)Data;
    (void)Size;
    return ctx;
}

// Helper to destroy a context
static void DestroyContext(cmsContext ctx) {
    if (ctx) cmsDeleteContext(ctx);
}

// Implementation of IO handler callbacks
static cmsUInt32Number FuzzIO_Read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !iohandler->stream || !Buffer) return 0;
    
    const uint8_t* src = (const uint8_t*)iohandler->stream;
    cmsUInt32Number total = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    cmsUInt32Number to_read = (total < available) ? total : available;
    
    if (to_read > 0) {
        memcpy(Buffer, src + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    
    return (to_read == total) ? count : 0;
}

static cmsBool FuzzIO_Write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    (void)iohandler;
    (void)size;
    (void)Buffer;
    // For fuzzing, we simulate a successful write without actually writing to memory
    return TRUE;
}

static cmsBool FuzzIO_Seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (!iohandler) return FALSE;
    if (offset <= iohandler->ReportedSize) {
        iohandler->UsedSpace = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsUInt32Number FuzzIO_Tell(struct _cms_io_handler* iohandler) {
    if (!iohandler) return 0;
    return iohandler->UsedSpace;
}

static cmsBool FuzzIO_Close(struct _cms_io_handler* iohandler) {
    (void)iohandler;
    return TRUE;
}

// Helper to create an IO handler
static cmsIOHANDLER* CreateIOHandler(const uint8_t *Data, size_t Size, cmsContext ctx) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = ctx;
    io->ReportedSize = Size;
    io->UsedSpace = 0;
    io->stream = (void*)Data;
    
    io->Read = FuzzIO_Read;
    io->Write = FuzzIO_Write;
    io->Seek = FuzzIO_Seek;
    io->Tell = FuzzIO_Tell;
    io->Close = FuzzIO_Close;
    
    return io;
}

// Helper to destroy an IO handler
static void DestroyIOHandler(cmsIOHANDLER* io) {
    if (io) free(io);
}

// Helper to create a dummy profile
static cmsHPROFILE CreateDummyProfile(cmsContext ctx) {
    (void)ctx;
    return cmsCreate_sRGBProfile();
}

// Helper to create a dummy IT8 handle
static cmsHANDLE CreateDummyIT8(cmsContext ctx) {
    (void)ctx;
    // Simulate an IT8 handle with a small allocation
    return (cmsHANDLE)malloc(1);
}

// Helper to destroy a dummy IT8 handle
static void DestroyDummyIT8(cmsHANDLE hIT8) {
    if (hIT8) free(hIT8);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContext(Data, Size);
    if (!ctx) return 0;

    cmsIOHANDLER* io = CreateIOHandler(Data, Size, ctx);
    if (!io) {
        DestroyContext(ctx);
        return 0;
    }

    cmsHPROFILE hProfile = CreateDummyProfile(ctx);
    cmsHANDLE hIT8 = CreateDummyIT8(ctx);

    // Test _cmsWriteUInt16Array
    cmsUInt16Number* arr16 = (cmsUInt16Number*)malloc(Size * sizeof(cmsUInt16Number));
    if (arr16) {
        memset(arr16, 0, Size * sizeof(cmsUInt16Number));
        size_t count = (Size > 1000) ? 1000 : Size;
        for (size_t i = 0; i < count; i++) {
            arr16[i] = Data[i % 256];
        }
        _cmsWriteUInt16Array(io, (cmsUInt32Number)count, arr16);
        free(arr16);
    }

    // Test _cmsWriteUInt32Number
    cmsUInt32Number val32 = 0;
    if (Size >= 4) {
        memcpy(&val32, Data, 4);
        _cmsWriteUInt32Number(io, val32);
    }

    // Test cmsSaveProfileToMem
    cmsUInt32Number bytesNeeded = 0;
    if (hProfile) {
        cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
        if (bytesNeeded > 0) {
            void* memPtr = malloc(bytesNeeded);
            if (memPtr) {
                cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
                free(memPtr);
            }
        }
    }

    // Test cmsSaveProfileToStream
    if (hProfile) {
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            cmsSaveProfileToStream(hProfile, f);
            fclose(f);
        }
    }

    // Test cmsIT8SaveToMem
    if (hIT8) {
        cmsUInt32Number it8BytesNeeded = 0;
        cmsIT8SaveToMem(hIT8, NULL, &it8BytesNeeded);
        if (it8BytesNeeded > 0) {
            void* it8MemPtr = malloc(it8BytesNeeded);
            if (it8MemPtr) {
                cmsIT8SaveToMem(hIT8, it8MemPtr, &it8BytesNeeded);
                free(it8MemPtr);
            }
        }
    }

    // Test cmsOpenProfileFromIOhandler2THR
    cmsHPROFILE hProfileRead = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
    if (hProfileRead) {
        cmsCloseProfile(hProfileRead);
    }
    cmsHPROFILE hProfileWrite = cmsOpenProfileFromIOhandler2THR(ctx, io, TRUE);
    if (hProfileWrite) {
        cmsCloseProfile(hProfileWrite);
    }

    // Cleanup
    DestroyIOHandler(io);
    if (hProfile) cmsCloseProfile(hProfile);
    DestroyDummyIT8(hIT8);
    DestroyContext(ctx);

    return 0;
}