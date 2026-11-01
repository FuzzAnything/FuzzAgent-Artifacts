// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromIOhandler2THR at cmsio0.c:1177:23 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Custom memory buffer for IO handler
typedef struct {
    uint8_t *data;
    size_t size;
    size_t pos;
    int is_allocated; // Flag to track if data was allocated by us
} MemBuffer;

static cmsUInt32Number MemRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemBuffer *buf = (MemBuffer*)iohandler->stream;
    cmsUInt32Number total = size * count;
    if (buf->pos + total > buf->size) {
        total = (buf->size - buf->pos) / size;
    }
    if (total == 0) return 0;
    memcpy(Buffer, buf->data + buf->pos, total * size);
    buf->pos += total * size;
    return total;
}

static cmsBool MemSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemBuffer *buf = (MemBuffer*)iohandler->stream;
    if (offset > buf->size) return FALSE;
    buf->pos = offset;
    return TRUE;
}

static cmsBool MemClose(cmsIOHANDLER* iohandler) {
    MemBuffer *buf = (MemBuffer*)iohandler->stream;
    if (buf) {
        // Do NOT free data here. The caller (CleanupMemIOHandler) is responsible.
        // We only free the buffer structure itself if needed, but usually we leave it for the caller.
        // However, to prevent double free, we set data to NULL after freeing if we were to free it.
        // But the design is: Create allocates, Close is called by lcms, Cleanup frees everything.
        // So Close should do nothing regarding memory deallocation of the buffer content.
        return TRUE;
    }
    return FALSE;
}

static cmsUInt32Number MemTell(cmsIOHANDLER* iohandler) {
    MemBuffer *buf = (MemBuffer*)iohandler->stream;
    return buf->pos;
}

static cmsBool MemWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MemBuffer *buf = (MemBuffer*)iohandler->stream;
    if (buf->pos + size > buf->size) return FALSE;
    memcpy(buf->data + buf->pos, Buffer, size);
    buf->pos += size;
    return TRUE;
}

static cmsIOHANDLER* CreateMemIOHandler(const uint8_t *Data, size_t Size, cmsBool write, cmsContext ContextID) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemBuffer *buf = (MemBuffer*)malloc(sizeof(MemBuffer));
    if (!buf) {
        free(io);
        return NULL;
    }
    
    if (write) {
        buf->data = (uint8_t*)malloc(Size);
        if (!buf->data) {
            free(buf);
            free(io);
            return NULL;
        }
        buf->size = Size;
        buf->pos = 0;
        buf->is_allocated = 1;
        memset(buf->data, 0, Size);
    } else {
        // For read mode, we copy data to ensure we own it and can free it later
        buf->data = (uint8_t*)malloc(Size);
        if (!buf->data) {
            free(buf);
            free(io);
            return NULL;
        }
        buf->size = Size;
        buf->pos = 0;
        buf->is_allocated = 1;
        memcpy(buf->data, Data, Size);
    }
    
    io->stream = buf;
    io->ContextID = ContextID;
    io->UsedSpace = 0;
    io->ReportedSize = Size;
    io->Read = MemRead;
    io->Seek = MemSeek;
    io->Close = MemClose;
    io->Tell = MemTell;
    io->Write = MemWrite;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    return io;
}

static void CleanupMemIOHandler(cmsIOHANDLER *io) {
    if (io) {
        MemBuffer *buf = (MemBuffer*)io->stream;
        if (buf) {
            if (buf->is_allocated) {
                free(buf->data);
            }
            free(buf);
        }
        free(io);
    }
}

static void* AllocateMemBuffer(size_t size) {
    return malloc(size);
}

static void FreeMemBuffer(void *ptr) {
    free(ptr);
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;
    
    cmsBool success = FALSE;
    
    // Test 1: cmsOpenProfileFromIOhandler2THR with read mode
    cmsIOHANDLER *ioRead = CreateMemIOHandler(Data, Size, FALSE, ContextID);
    if (ioRead) {
        cmsHPROFILE hProfile = cmsOpenProfileFromIOhandler2THR(ContextID, ioRead, FALSE);
        if (hProfile) {
            // Test 5: cmsSaveProfileToStream
            FILE *f = fopen("./dummy_file", "wb");
            if (f) {
                success = cmsSaveProfileToStream(hProfile, f);
                fclose(f);
            }
            
            // Test 6: cmsSaveProfileToMem
            cmsUInt32Number BytesNeeded = 0;
            cmsSaveProfileToMem(hProfile, NULL, &BytesNeeded);
            if (BytesNeeded > 0) {
                void *MemPtr = AllocateMemBuffer(BytesNeeded);
                if (MemPtr) {
                    success = cmsSaveProfileToMem(hProfile, MemPtr, &BytesNeeded);
                    FreeMemBuffer(MemPtr);
                }
            }
            
            cmsCloseProfile(hProfile);
        }
        // Cleanup the IO handler after use. 
        // Note: cmsOpenProfileFromIOhandler2THR might have taken ownership or called Close internally.
        // If it called Close, our MemClose does not free memory, so it's safe to free here.
        // If it didn't, we still need to free.
        CleanupMemIOHandler(ioRead);
        ioRead = NULL; // Prevent double free if logic changes
    }
    
    // Test 2: _cmsWriteUInt16Array
    if (Size >= 2) {
        cmsIOHANDLER *ioWrite = CreateMemIOHandler(NULL, 1024, TRUE, ContextID);
        if (ioWrite) {
            cmsUInt16Number arr[100];
            size_t count = (Size / 2) > 100 ? 100 : (Size / 2);
            for (size_t i = 0; i < count; i++) {
                arr[i] = Data[i * 2] | (Data[i * 2 + 1] << 8);
            }
            success = _cmsWriteUInt16Array(ioWrite, count, arr);
            CleanupMemIOHandler(ioWrite);
        }
    }
    
    // Test 4: _cmsWriteUInt32Number
    if (Size >= 4) {
        cmsIOHANDLER *ioWrite2 = CreateMemIOHandler(NULL, 1024, TRUE, ContextID);
        if (ioWrite2) {
            cmsUInt32Number val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
            success = _cmsWriteUInt32Number(ioWrite2, val);
            CleanupMemIOHandler(ioWrite2);
        }
    }
    
    cmsDeleteContext(ContextID);
    return 0;
}