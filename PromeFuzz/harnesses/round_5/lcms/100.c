// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile1) {
        cmsCloseProfile(hProfile1);
    }
    
    // Test cmsOpenProfileFromFileTHR
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", "r");
        if (hProfile2) {
            cmsCloseProfile(hProfile2);
        }
        
        // Test cmsOpenProfileFromMemTHR
        cmsHPROFILE hProfile3 = cmsOpenProfileFromMemTHR(ctx, Data, Size);
        if (hProfile3) {
            // Test cmsCreateTransformTHR
            cmsUInt32Number intent = Data[0] % 4;
            cmsUInt32Number flags = Data[1];
            cmsUInt32Number inputFormat = Data[2] % 20;
            cmsUInt32Number outputFormat = Data[3] % 20;
            
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                ctx,
                hProfile3,
                inputFormat,
                NULL,
                outputFormat,
                intent,
                flags
            );
            
            if (transform) {
                cmsDeleteTransform(transform);
            }
            
            // Test cmsCreateMultiprofileTransform
            cmsHPROFILE profiles[2] = {hProfile3, hProfile3};
            cmsUInt32Number nProfiles = 2;
            
            cmsHTRANSFORM multiTransform = cmsCreateMultiprofileTransform(
                profiles,
                nProfiles,
                inputFormat,
                outputFormat,
                intent,
                flags
            );
            
            if (multiTransform) {
                cmsDeleteTransform(multiTransform);
            }
            
            cmsCloseProfile(hProfile3);
        }
        
        // Test cmsSignalError
        char errorText[256];
        snprintf(errorText, sizeof(errorText), "Fuzzer generated error with size %zu", Size);
        cmsSignalError(ctx, Data[0] % 100, "%s", errorText);
        
        cmsDeleteContext(ctx);
    }
    
    cleanup_file();
    return 0;
}