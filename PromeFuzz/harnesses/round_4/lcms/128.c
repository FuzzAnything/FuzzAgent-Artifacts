// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsIOHANDLER* create_dummy_iohandler(void) {
    // Create a simple memory-based I/O handler for testing
    static struct _cms_io_handler handler;
    static unsigned char buffer[4096];
    
    handler.stream = buffer;
    handler.ContextID = NULL;
    handler.UsedSpace = 0;
    handler.ReportedSize = sizeof(buffer);
    handler.PhysicalFile[0] = '\0';
    
    // Minimal implementations for fuzzing
    handler.Read = NULL;  // Not needed for writing
    handler.Seek = NULL;
    handler.Close = NULL;
    handler.Tell = NULL;
    handler.Write = NULL;  // We'll handle writes directly if needed
    
    return &handler;
}

static cmsHPROFILE create_dummy_profile(void) {
    // Create a simple RGB profile for testing
    cmsToneCurve* curve = cmsBuildGamma(NULL, 2.2);
    if (!curve) return NULL;
    
    cmsHPROFILE profile = cmsCreateRGBProfile(
        cmsD50_xyY(),
        (cmsCIExyYTRIPLE[]){{0.64, 0.33, 1.0}, {0.30, 0.60, 1.0}, {0.15, 0.06, 1.0}},
        (cmsToneCurve*[3]){curve, curve, curve});
    
    cmsFreeToneCurve(curve);
    
    return profile;
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    // Test _cmsAdjustEndianess32
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number value;
        memcpy(&value, Data, sizeof(cmsUInt32Number));
        cmsUInt32Number result = _cmsAdjustEndianess32(value);
        (void)result;  // Use result to avoid unused variable warning
    }
    
    // Test cmsWriteRawTag
    if (Size >= sizeof(cmsTagSignature) + 4) {
        cmsHPROFILE profile = create_dummy_profile();
        if (profile) {
            cmsTagSignature sig;
            memcpy(&sig, Data, sizeof(cmsTagSignature));
            
            size_t data_size = Size - sizeof(cmsTagSignature);
            if (data_size > 0) {
                const void* tag_data = Data + sizeof(cmsTagSignature);
                cmsWriteRawTag(profile, sig, tag_data, (cmsUInt32Number)data_size);
            }
            
            cmsCloseProfile(profile);
        }
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsIOHANDLER* io = create_dummy_iohandler();
        if (io) {
            cmsFloat64Number value;
            memcpy(&value, Data, sizeof(cmsFloat64Number));
            _cmsWrite15Fixed16Number(io, value);
        }
    }
    
    // Test _cmsWriteUInt32Number
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsIOHANDLER* io = create_dummy_iohandler();
        if (io) {
            cmsUInt32Number value;
            memcpy(&value, Data, sizeof(cmsUInt32Number));
            _cmsWriteUInt32Number(io, value);
        }
    }
    
    // Test _cmsWriteFloat32Number
    if (Size >= sizeof(cmsFloat32Number)) {
        cmsIOHANDLER* io = create_dummy_iohandler();
        if (io) {
            cmsFloat32Number value;
            memcpy(&value, Data, sizeof(cmsFloat32Number));
            _cmsWriteFloat32Number(io, value);
        }
    }
    
    // Test _cmsWriteUInt16Number
    if (Size >= sizeof(cmsUInt16Number)) {
        cmsIOHANDLER* io = create_dummy_iohandler();
        if (io) {
            cmsUInt16Number value;
            memcpy(&value, Data, sizeof(cmsUInt16Number));
            _cmsWriteUInt16Number(io, value);
        }
    }
    
    return 0;
}