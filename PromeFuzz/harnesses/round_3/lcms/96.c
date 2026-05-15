// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess32 at cmsplugin.c:58:28 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // 1. Test _cmsAdjustEndianess32
    if (Size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number value;
        memcpy(&value, Data, sizeof(cmsUInt32Number));
        cmsUInt32Number result = _cmsAdjustEndianess32(value);
        (void)result;
    }

    // 2. Test _cmsDecodeDateTimeNumber
    if (Size >= sizeof(cmsDateTimeNumber)) {
        cmsDateTimeNumber dt;
        memcpy(&dt, Data, sizeof(cmsDateTimeNumber));
        struct tm dest;
        _cmsDecodeDateTimeNumber(&dt, &dest);
    }

    // 3. Test _cmsWriteUInt32Number
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(NULL, "./dummy_file", "w");
        if (io) {
            cmsUInt32Number n = 0;
            if (Size >= sizeof(cmsUInt32Number)) {
                memcpy(&n, Data, sizeof(cmsUInt32Number));
            }
            cmsBool write_result = _cmsWriteUInt32Number(io, n);
            (void)write_result;
            cmsCloseIOhandler(io);
        }
        fclose(dummy_file);
        remove("./dummy_file");
    }

    // 4. Test cmsWriteRawTag
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        cmsTagSignature sig = 0;
        if (Size >= sizeof(cmsTagSignature)) {
            memcpy(&sig, Data, sizeof(cmsTagSignature));
        }
        
        const void* tag_data = Data;
        cmsUInt32Number tag_size = (cmsUInt32Number)(Size > 0xFFFFFFFF ? 0xFFFFFFFF : Size);
        
        cmsBool write_tag_result = cmsWriteRawTag(hProfile, sig, tag_data, tag_size);
        (void)write_tag_result;
        
        cmsCloseProfile(hProfile);
    }

    // 5. Test _cmsEncodeDateTimeNumber
    if (Size >= sizeof(struct tm)) {
        struct tm source;
        memcpy(&source, Data, sizeof(struct tm));
        
        source.tm_year = source.tm_year % 200;
        source.tm_mon = source.tm_mon % 12;
        source.tm_mday = (source.tm_mday % 31) + 1;
        source.tm_hour = source.tm_hour % 24;
        source.tm_min = source.tm_min % 60;
        source.tm_sec = source.tm_sec % 60;
        source.tm_wday = source.tm_wday % 7;
        source.tm_yday = source.tm_yday % 366;
        source.tm_isdst = source.tm_isdst & 1;
        
        cmsDateTimeNumber dest;
        _cmsEncodeDateTimeNumber(&dest, &source);
    }

    // 6. Test cmsGetHeaderCreationDateTime
    cmsHPROFILE hProfile2 = cmsCreate_sRGBProfile();
    if (hProfile2) {
        struct tm dest_tm;
        cmsBool get_date_result = cmsGetHeaderCreationDateTime(hProfile2, &dest_tm);
        (void)get_date_result;
        cmsCloseProfile(hProfile2);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif