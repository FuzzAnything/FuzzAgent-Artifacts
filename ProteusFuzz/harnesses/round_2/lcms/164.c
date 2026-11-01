// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// cmsGetHeaderManufacturer at cmsio0.c:1008:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ContextID, Data, Size);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt64Number flags = 0;
    cmsUInt64Number flagsInput = 0;
    cmsUInt32Number version = 0;
    cmsUInt32Number manufacturer = 0;
    cmsUInt64Number endianSource = 0;
    cmsUInt64Number endianResult = 0;

    // Fuzz cmsGetHeaderAttributes
    cmsGetHeaderAttributes(hProfile, &flags);

    // Fuzz cmsSetHeaderAttributes with fuzzed flags
    flagsInput = ((cmsUInt64Number)Data[0] << 56) | 
                 ((cmsUInt64Number)Data[1] << 48) | 
                 ((cmsUInt64Number)Data[2] << 40) | 
                 ((cmsUInt64Number)Data[3] << 32) | 
                 ((cmsUInt64Number)Data[4] << 24) | 
                 ((cmsUInt64Number)Data[5] << 16) | 
                 ((cmsUInt64Number)Data[6] << 8) | 
                 ((cmsUInt64Number)Data[7]);
    cmsSetHeaderAttributes(hProfile, flagsInput);

    // Fuzz cmsSetEncodedICCversion
    version = (cmsUInt32Number)Data[8] | 
              ((cmsUInt32Number)Data[9] << 8) | 
              ((cmsUInt32Number)Data[10] << 16) | 
              ((cmsUInt32Number)Data[11] << 24);
    cmsSetEncodedICCversion(hProfile, version);

    // Fuzz _cmsAdjustEndianess64
    endianSource = ((cmsUInt64Number)Data[12] << 56) | 
                   ((cmsUInt64Number)Data[13] << 48) | 
                   ((cmsUInt64Number)Data[14] << 40) | 
                   ((cmsUInt64Number)Data[15] << 32) | 
                   ((cmsUInt64Number)Data[16] << 24) | 
                   ((cmsUInt64Number)Data[17] << 16) | 
                   ((cmsUInt64Number)Data[18] << 8) | 
                   ((cmsUInt64Number)Data[19]);
    _cmsAdjustEndianess64(&endianResult, &endianSource);

    // Fuzz cmsGetHeaderManufacturer
    manufacturer = cmsGetHeaderManufacturer(hProfile);

    // Additional state exploration: re-call functions with modified inputs
    cmsGetHeaderAttributes(hProfile, &flags);
    cmsSetHeaderAttributes(hProfile, flags ^ 0xFFFFFFFFFFFFFFFF);
    cmsSetEncodedICCversion(hProfile, ~version);
    _cmsAdjustEndianess64(&endianResult, &endianSource);
    manufacturer = cmsGetHeaderManufacturer(hProfile);

    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}