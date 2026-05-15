// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Custom I/O handler implementation for fuzzing
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} FuzzStream;

static cmsUInt32Number fuzz_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    
    if (stream->pos >= stream->size) {
        return 0;
    }
    
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = (cmsUInt32Number)(stream->size - stream->pos);
    }
    
    memcpy(Buffer, stream->data + stream->pos, total_bytes);
    stream->pos += total_bytes;
    
    return total_bytes / size;
}

static cmsBool fuzz_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    
    if (offset > stream->size) {
        return FALSE;
    }
    
    stream->pos = offset;
    return TRUE;
}

static cmsBool fuzz_close(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for fuzzing
    return TRUE;
}

static cmsUInt32Number fuzz_tell(cmsIOHANDLER* iohandler) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool fuzz_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    
    if (stream->pos + size > stream->size) {
        return FALSE;
    }
    
    memcpy((void*)(stream->data + stream->pos), Buffer, size);
    stream->pos += size;
    return TRUE;
}

static cmsIOHANDLER* create_fuzz_iohandler(const uint8_t* data, size_t size) {
    static cmsIOHANDLER handler;
    static FuzzStream stream;
    
    stream.data = data;
    stream.size = size;
    stream.pos = 0;
    
    handler.stream = &stream;
    handler.ContextID = NULL;
    handler.UsedSpace = 0;
    handler.ReportedSize = (cmsUInt32Number)size;
    memset(handler.PhysicalFile, 0, cmsMAX_PATH);
    handler.Read = fuzz_read;
    handler.Seek = fuzz_seek;
    handler.Close = fuzz_close;
    handler.Tell = fuzz_tell;
    handler.Write = fuzz_write;
    
    return &handler;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    // Function 1: _cms15Fixed16toDouble
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(cmsS15Fixed16Number));
        cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
        (void)result1; // Avoid unused variable warning
    }
    
    // Function 2: _cmsRead15Fixed16Number
    if (Size >= 4) {
        cmsIOHANDLER* io = create_fuzz_iohandler(Data, Size);
        cmsFloat64Number n = 0.0;
        cmsBool success = _cmsRead15Fixed16Number(io, &n);
        (void)success; // Avoid unused variable warning
    }
    
    // Function 3: cmsGetEncodedCMMversion
    int version = cmsGetEncodedCMMversion();
    (void)version; // Avoid unused variable warning
    
    // Function 4: cmsGetProfileVersion
    // Create a dummy profile for testing using sRGB parameters
    const cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(NULL,
        cmsD50_xyY(),
        &primaries,
        (cmsToneCurve*[3]){NULL, NULL, NULL});
    
    if (profile) {
        cmsFloat64Number profile_version = cmsGetProfileVersion(profile);
        (void)profile_version; // Avoid unused variable warning
        cmsCloseProfile(profile);
    }
    
    // Function 5: _cmsDoubleTo15Fixed16
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number v;
        memcpy(&v, Data, sizeof(cmsFloat64Number));
        cmsS15Fixed16Number fixed = _cmsDoubleTo15Fixed16(v);
        (void)fixed; // Avoid unused variable warning
    }
    
    // Function 6: _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsIOHANDLER* io = create_fuzz_iohandler(Data, Size);
        cmsFloat64Number n;
        memcpy(&n, Data, sizeof(cmsFloat64Number));
        cmsBool write_success = _cmsWrite15Fixed16Number(io, n);
        (void)write_success; // Avoid unused variable warning
    }
    
    // Additional exploration: Try different combinations
    if (Size >= sizeof(cmsS15Fixed16Number) + sizeof(cmsFloat64Number)) {
        // Convert fixed to double and back
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(cmsS15Fixed16Number));
        cmsFloat64Number d = _cms15Fixed16toDouble(fix32);
        cmsS15Fixed16Number fix32_back = _cmsDoubleTo15Fixed16(d);
        (void)fix32_back; // Avoid unused variable warning
        
        // Try with different offsets in the data
        if (Size >= 2 * sizeof(cmsFloat64Number)) {
            cmsFloat64Number v1, v2;
            memcpy(&v1, Data, sizeof(cmsFloat64Number));
            memcpy(&v2, Data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
            
            cmsS15Fixed16Number f1 = _cmsDoubleTo15Fixed16(v1);
            cmsS15Fixed16Number f2 = _cmsDoubleTo15Fixed16(v2);
            (void)f1; (void)f2; // Avoid unused variable warnings
            
            cmsFloat64Number d1 = _cms15Fixed16toDouble(f1);
            cmsFloat64Number d2 = _cms15Fixed16toDouble(f2);
            (void)d1; (void)d2; // Avoid unused variable warnings
        }
    }
    
    return 0;
}