// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    FILE* fp = (FILE*)iohandler->stream;
    return fread(buffer, size, count, fp);
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FILE* fp = (FILE*)iohandler->stream;
    return fseek(fp, offset, SEEK_SET) == 0;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    FILE* fp = (FILE*)iohandler->stream;
    fclose(fp);
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    FILE* fp = (FILE*)iohandler->stream;
    return ftell(fp);
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                           const void* buffer) {
    FILE* fp = (FILE*)iohandler->stream;
    return fwrite(buffer, 1, size, fp) == size;
}

static cmsIOHANDLER* create_dummy_handler(const char* mode) {
    FILE* fp = fopen("./dummy_file", mode);
    if (!fp) return NULL;
    
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) {
        fclose(fp);
        return NULL;
    }
    
    handler->stream = fp;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    strcpy(handler->PhysicalFile, "./dummy_file");
    handler->Read = dummy_read;
    handler->Seek = dummy_seek;
    handler->Close = dummy_close;
    handler->Tell = dummy_tell;
    handler->Write = dummy_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    cmsIOHANDLER* read_handler = create_dummy_handler("rb");
    if (!read_handler) return 0;
    
    cmsIOHANDLER* write_handler = create_dummy_handler("wb");
    if (!write_handler) {
        read_handler->Close(read_handler);
        return 0;
    }
    
    cmsUInt32Number n32;
    cmsBool result;
    
    result = _cmsReadUInt32Number(read_handler, &n32);
    
    read_handler->Seek(read_handler, 0);
    result = _cmsReadAlignment(read_handler);
    
    read_handler->Seek(read_handler, 0);
    if (Size >= 2) {
        cmsUInt32Number array_size = (cmsUInt32Number)(Size / 2);
        if (array_size > 100) array_size = 100;
        cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        if (array) {
            result = _cmsReadUInt16Array(read_handler, array_size, array);
            free(array);
        }
    }
    
    n32 = *((cmsUInt32Number*)Data);
    result = _cmsWriteUInt32Number(write_handler, n32);
    
    result = _cmsWriteAlignment(write_handler);
    
    if (Size >= 4) {
        cmsUInt32Number array_size = (cmsUInt32Number)(Size / 2);
        if (array_size > 100) array_size = 100;
        cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        if (array) {
            for (cmsUInt32Number i = 0; i < array_size; i++) {
                array[i] = (cmsUInt16Number)(Data[i % Size] | (Data[(i + 1) % Size] << 8));
            }
            result = _cmsWriteUInt16Array(write_handler, array_size, array);
            free(array);
        }
    }
    
    read_handler->Close(read_handler);
    write_handler->Close(write_handler);
    
    return 0;
}