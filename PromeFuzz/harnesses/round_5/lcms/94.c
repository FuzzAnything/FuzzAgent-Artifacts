// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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
#include <time.h>

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    return 0;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return 0;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->ContextID = 0;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    strncpy(io->PhysicalFile, "./dummy_file", cmsMAX_PATH - 1);
    io->PhysicalFile[cmsMAX_PATH - 1] = '\0';
    
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

static cmsHPROFILE create_dummy_profile(void) {
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},
        {0.3000, 0.6000, 1.0},
        {0.1500, 0.0600, 1.0}
    };
    
    cmsToneCurve* curve = cmsBuildGamma(0, 2.2);
    if (!curve) return NULL;
    
    cmsHPROFILE profile = cmsCreateRGBProfile(&whitePoint, &primaries, &curve);
    cmsFreeToneCurve(curve);
    
    return profile;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test _cmsWriteUInt32Number
    cmsIOHANDLER* io = create_dummy_iohandler();
    if (io) {
        cmsUInt32Number value = *(cmsUInt32Number*)Data;
        _cmsWriteUInt32Number(io, value);
        free(io);
    }
    
    // Test _cmsEncodeDateTimeNumber and _cmsDecodeDateTimeNumber
    if (Size >= sizeof(struct tm) + sizeof(cmsDateTimeNumber)) {
        struct tm source_tm;
        memcpy(&source_tm, Data, sizeof(struct tm));
        
        cmsDateTimeNumber encoded;
        _cmsEncodeDateTimeNumber(&encoded, &source_tm);
        
        struct tm decoded_tm;
        _cmsDecodeDateTimeNumber(&encoded, &decoded_tm);
    }
    
    // Test cmsGetHeaderCreationDateTime
    cmsHPROFILE profile = create_dummy_profile();
    if (profile) {
        struct tm creation_tm;
        cmsGetHeaderCreationDateTime(profile, &creation_tm);
        cmsCloseProfile(profile);
    }
    
    // Test cmsWriteRawTag
    if (Size >= 8) {
        profile = create_dummy_profile();
        if (profile) {
            cmsTagSignature sig = *(cmsTagSignature*)Data;
            const void* tag_data = Data + 4;
            cmsUInt32Number tag_size = (Size - 4) < 1024 ? (Size - 4) : 1024;
            
            cmsWriteRawTag(profile, sig, tag_data, tag_size);
            cmsCloseProfile(profile);
        }
    }
    
    // Test _cmsAdjustEndianess32
    if (Size >= 4) {
        cmsUInt32Number value = *(cmsUInt32Number*)Data;
        _cmsAdjustEndianess32(value);
    }
    
    return 0;
}