// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

#define MAX_PATH 260
#define cmsMAX_PATH MAX_PATH

typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} MockStream;

static cmsUInt32Number mockRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MockStream* stream = (MockStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
        if (total < size) return 0;
        total = total - (total % size);
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total / size;
}

static cmsBool mockSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MockStream* stream = (MockStream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsBool mockClose(cmsIOHANDLER* iohandler) {
    if (iohandler->stream) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number mockTell(cmsIOHANDLER* iohandler) {
    MockStream* stream = (MockStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool mockWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    MockStream* stream = (MockStream*)iohandler->stream;
    
    if (stream->pos + size > stream->size) {
        return FALSE;
    }
    
    memcpy((void*)(stream->data + stream->pos), Buffer, size);
    stream->pos += size;
    return TRUE;
}

static cmsIOHANDLER* createMockIOHandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MockStream* stream = (MockStream*)calloc(1, sizeof(MockStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = mockRead;
    io->Seek = mockSeek;
    io->Close = mockClose;
    io->Tell = mockTell;
    io->Write = mockWrite;
    
    return io;
}

static void destroyMockIOHandler(cmsIOHANDLER* io) {
    if (io) {
        mockClose(io);
        free(io);
    }
}

static cmsHPROFILE createTestProfile(const uint8_t* data, size_t size) {
    if (size < 128) return NULL;
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    fwrite(data, 1, size > 4096 ? 4096 : size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove("./dummy_file");
    
    return profile;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test _cms15Fixed16toDouble
    cmsS15Fixed16Number fix32;
    memcpy(&fix32, Data, sizeof(fix32));
    cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
    (void)result1;
    
    // Test _cmsDoubleTo15Fixed16
    if (Size >= 12) {
        cmsFloat64Number d;
        memcpy(&d, Data + 4, sizeof(d));
        cmsS15Fixed16Number result2 = _cmsDoubleTo15Fixed16(d);
        (void)result2;
    }
    
    // Test _cmsRead15Fixed16Number
    cmsIOHANDLER* io1 = createMockIOHandler(Data, Size);
    if (io1) {
        cmsFloat64Number n1;
        cmsBool read_result = _cmsRead15Fixed16Number(io1, &n1);
        (void)read_result;
        destroyMockIOHandler(io1);
    }
    
    // Test _cmsWrite15Fixed16Number
    cmsIOHANDLER* io2 = createMockIOHandler(Data, Size);
    if (io2 && Size >= 12) {
        cmsFloat64Number d2;
        memcpy(&d2, Data + 4, sizeof(d2));
        cmsBool write_result = _cmsWrite15Fixed16Number(io2, d2);
        (void)write_result;
        destroyMockIOHandler(io2);
    }
    
    // Test cmsGetProfileVersion
    cmsHPROFILE profile = createTestProfile(Data, Size);
    if (profile) {
        cmsFloat64Number version = cmsGetProfileVersion(profile);
        (void)version;
        cmsCloseProfile(profile);
    }
    
    // Test cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    (void)cmm_version;
    
    return 0;
}