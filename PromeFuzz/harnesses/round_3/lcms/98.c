// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
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
#include <time.h>

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    return count;
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

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                           const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(void) {
    static cmsIOHANDLER dummy_handler;
    memset(&dummy_handler, 0, sizeof(dummy_handler));
    
    dummy_handler.Write = dummy_write;
    dummy_handler.Read = dummy_read;
    dummy_handler.Seek = dummy_seek;
    dummy_handler.Close = dummy_close;
    dummy_handler.Tell = dummy_tell;
    
    return &dummy_handler;
}

static cmsHPROFILE create_dummy_profile(void) {
    cmsToneCurve* transferFunction[3];
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    
    transferFunction[0] = cmsBuildGamma(ctx, 2.2);
    transferFunction[1] = cmsBuildGamma(ctx, 2.2);
    transferFunction[2] = cmsBuildGamma(ctx, 2.2);
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx, 
        cmsD50_xyY(),
        NULL,
        transferFunction);
    
    cmsFreeToneCurve(transferFunction[0]);
    cmsFreeToneCurve(transferFunction[1]);
    cmsFreeToneCurve(transferFunction[2]);
    cmsDeleteContext(ctx);
    return profile;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Test _cmsAdjustEndianess32
    cmsUInt32Number value;
    memcpy(&value, Data, sizeof(cmsUInt32Number));
    cmsUInt32Number adjusted = _cmsAdjustEndianess32(value);
    (void)adjusted;
    
    // Test _cmsDecodeDateTimeNumber
    if (Size >= sizeof(cmsDateTimeNumber)) {
        cmsDateTimeNumber dateTime;
        memcpy(&dateTime, Data, sizeof(cmsDateTimeNumber));
        struct tm decoded_tm;
        _cmsDecodeDateTimeNumber(&dateTime, &decoded_tm);
    }
    
    // Test _cmsWriteUInt32Number
    cmsIOHANDLER* io = create_dummy_iohandler();
    if (Size >= 8) {
        cmsUInt32Number write_value;
        memcpy(&write_value, Data + 4, sizeof(cmsUInt32Number));
        _cmsWriteUInt32Number(io, write_value);
    }
    
    // Test cmsWriteRawTag
    cmsHPROFILE profile = create_dummy_profile();
    if (profile && Size >= 12) {
        cmsTagSignature sig;
        memcpy(&sig, Data + 8, sizeof(cmsTagSignature));
        size_t data_size = Size > 12 ? Size - 12 : 0;
        if (data_size > 0) {
            const void* tag_data = Data + 12;
            cmsWriteRawTag(profile, sig, tag_data, (cmsUInt32Number)data_size);
        }
        cmsCloseProfile(profile);
    }
    
    // Test _cmsEncodeDateTimeNumber
    if (Size >= sizeof(struct tm)) {
        struct tm source_tm;
        memcpy(&source_tm, Data, sizeof(struct tm));
        source_tm.tm_year = source_tm.tm_year % 1000;
        source_tm.tm_mon = source_tm.tm_mon % 12;
        source_tm.tm_mday = (source_tm.tm_mday % 31) + 1;
        source_tm.tm_hour = source_tm.tm_hour % 24;
        source_tm.tm_min = source_tm.tm_min % 60;
        source_tm.tm_sec = source_tm.tm_sec % 60;
        
        cmsDateTimeNumber encoded;
        _cmsEncodeDateTimeNumber(&encoded, &source_tm);
    }
    
    // Test cmsGetHeaderCreationDateTime
    profile = create_dummy_profile();
    if (profile) {
        struct tm creation_tm;
        cmsGetHeaderCreationDateTime(profile, &creation_tm);
        cmsCloseProfile(profile);
    }
    
    return 0;
}