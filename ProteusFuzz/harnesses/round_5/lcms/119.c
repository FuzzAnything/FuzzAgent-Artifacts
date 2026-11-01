// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Helper to create a dummy profile for testing
// Using cmsCreateProfilePlaceholder as a valid way to create an empty profile
static cmsHPROFILE create_dummy_profile(void) {
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    return hProfile;
}

// Custom IO handler for testing
static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    return 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(void) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    handler->stream = NULL;
    handler->ContextID = NULL;
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    memset(handler->PhysicalFile, 0, sizeof(handler->PhysicalFile));
    
    handler->Read = dummy_read;
    handler->Seek = dummy_seek;
    handler->Close = dummy_close;
    handler->Tell = dummy_tell;
    handler->Write = dummy_write;
    
    return handler;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize LCMS context
    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (!Context) return 0;

    // Test _cmsEncodeDateTimeNumber and _cmsDecodeDateTimeNumber
    struct tm tm_source = {0};
    cmsDateTimeNumber dt_encoded = {0};
    struct tm tm_decoded = {0};

    // Use input data to populate tm_source
    if (Size >= 9) {
        tm_source.tm_sec = Data[0] % 60;
        tm_source.tm_min = Data[1] % 60;
        tm_source.tm_hour = Data[2] % 24;
        tm_source.tm_mday = (Data[3] % 28) + 1;
        tm_source.tm_mon = Data[4] % 12;
        tm_source.tm_year = (Data[5] % 100) + 100;
        tm_source.tm_isdst = Data[6] % 2;
        tm_source.tm_wday = Data[7] % 7;
        tm_source.tm_yday = Data[8] % 365;
    }

    _cmsEncodeDateTimeNumber(&dt_encoded, &tm_source);
    _cmsDecodeDateTimeNumber(&dt_encoded, &tm_decoded);

    // Test _cmsWriteUInt16Number
    cmsIOHANDLER* io_handler = create_dummy_io_handler();
    if (io_handler) {
        cmsUInt16Number test_value = (Size > 2) ? (Data[0] << 8 | Data[1]) : 0;
        _cmsWriteUInt16Number(io_handler, test_value);
        free(io_handler);
    }

    // Test cmsSaveProfileToIOhandler
    cmsHPROFILE hProfile = create_dummy_profile();
    if (hProfile) {
        cmsIOHANDLER* io_handler2 = create_dummy_io_handler();
        if (io_handler2) {
            cmsSaveProfileToIOhandler(hProfile, io_handler2);
            free(io_handler2);
        }
        cmsSaveProfileToIOhandler(hProfile, NULL);
        cmsCloseProfile(hProfile);
    }

    // Test cmsGetHeaderCreationDateTime
    hProfile = create_dummy_profile();
    if (hProfile) {
        struct tm creation_time = {0};
        cmsGetHeaderCreationDateTime(hProfile, &creation_time);
        cmsCloseProfile(hProfile);
    }

    // Test cmsWriteTag
    hProfile = create_dummy_profile();
    if (hProfile) {
        cmsTagSignature tag_sig = (Size > 4) ? ((cmsTagSignature)(Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3])) : 0;
        cmsWriteTag(hProfile, tag_sig, Data);
        cmsWriteTag(hProfile, tag_sig, NULL); // Delete tag
        cmsCloseProfile(hProfile);
    }

    cmsDeleteContext(Context);
    return 0;
}