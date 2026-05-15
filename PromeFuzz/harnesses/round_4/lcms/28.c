// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // First, try to open a profile from the fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    
    if (hProfile != NULL) {
        // Test cmsGetTagCount
        cmsInt32Number tagCount = cmsGetTagCount(hProfile);
        (void)tagCount; // Use result to avoid unused variable warning
        
        // Test cmsGetHeaderCreationDateTime
        struct tm creationDateTime;
        cmsBool result = cmsGetHeaderCreationDateTime(hProfile, &creationDateTime);
        (void)result; // Use result to avoid unused variable warning
        
        // Test cmsGetHeaderRenderingIntent
        cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
        (void)intent; // Use result to avoid unused variable warning
        
        // Test cmsReadTag with some common tag signatures
        // Try a few different tag signatures to increase coverage
        const cmsTagSignature tagsToTry[] = {
            cmsSigMediaWhitePointTag,
            cmsSigRedColorantTag,
            cmsSigGreenColorantTag,
            cmsSigBlueColorantTag,
            cmsSigCopyrightTag,
            cmsSigProfileDescriptionTag,
            cmsSigAToB0Tag,
            cmsSigBToA0Tag
        };
        
        for (size_t i = 0; i < sizeof(tagsToTry) / sizeof(tagsToTry[0]); i++) {
            void* tagData = cmsReadTag(hProfile, tagsToTry[i]);
            (void)tagData; // Use result to avoid unused variable warning
        }
        
        // Clean up the profile
        cmsCloseProfile(hProfile);
    }
    
    // Test _cmsDecodeDateTimeNumber and _cmsEncodeDateTimeNumber
    // These functions don't require a profile handle
    if (Size >= sizeof(cmsDateTimeNumber) + sizeof(struct tm)) {
        cmsDateTimeNumber dateTimeNumber;
        struct tm tmStruct;
        
        // Copy data from fuzzer input to our structures
        memcpy(&dateTimeNumber, Data, sizeof(cmsDateTimeNumber));
        memcpy(&tmStruct, Data + sizeof(cmsDateTimeNumber), sizeof(struct tm));
        
        // Test encoding
        cmsDateTimeNumber encodedDateTime;
        _cmsEncodeDateTimeNumber(&encodedDateTime, &tmStruct);
        
        // Test decoding
        struct tm decodedTm;
        _cmsDecodeDateTimeNumber(&dateTimeNumber, &decodedTm);
    }
    
    return 0;
}