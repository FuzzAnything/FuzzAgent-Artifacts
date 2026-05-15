// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromStream at cmsio0.c:1264:24 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    write_dummy_file(Data, Size);
    
    // Test cmsOpenProfileFromFile
    cmsHPROFILE hProfile1 = cmsOpenProfileFromFile("./dummy_file", "r");
    if (hProfile1) {
        // Test cmsSetPCS with random color space signature
        cmsColorSpaceSignature pcs = *(cmsColorSpaceSignature*)Data;
        cmsSetPCS(hProfile1, pcs);
        cmsCloseProfile(hProfile1);
    }
    
    // Test cmsOpenProfileFromStream
    FILE *stream = fopen("./dummy_file", "rb");
    if (stream) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromStream(stream, "r");
        if (hProfile2) {
            cmsColorSpaceSignature pcs2 = *(cmsColorSpaceSignature*)(Data + 4 % Size);
            cmsSetPCS(hProfile2, pcs2);
            cmsCloseProfile(hProfile2);
        }
        fclose(stream);
    }
    
    // Test cmsOpenProfileFromFileTHR
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE hProfile3 = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
        if (hProfile3) {
            cmsColorSpaceSignature pcs3 = *(cmsColorSpaceSignature*)(Data + 8 % Size);
            cmsSetPCS(hProfile3, pcs3);
            cmsCloseProfile(hProfile3);
        }
        
        // Test cmsCreateXYZProfileTHR
        cmsHPROFILE xyzProfile1 = cmsCreateXYZProfileTHR(context);
        if (xyzProfile1) {
            cmsColorSpaceSignature pcs4 = *(cmsColorSpaceSignature*)(Data + 12 % Size);
            cmsSetPCS(xyzProfile1, pcs4);
            cmsCloseProfile(xyzProfile1);
        }
        
        cmsDeleteContext(context);
    }
    
    // Test cmsCreateXYZProfile
    cmsHPROFILE xyzProfile2 = cmsCreateXYZProfile();
    if (xyzProfile2) {
        cmsColorSpaceSignature pcs5 = *(cmsColorSpaceSignature*)(Data + 16 % Size);
        cmsSetPCS(xyzProfile2, pcs5);
        cmsCloseProfile(xyzProfile2);
    }
    
    remove_dummy_file();
    return 0;
}