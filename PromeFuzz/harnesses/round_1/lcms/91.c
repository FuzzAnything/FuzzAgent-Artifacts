// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    // Create a context first since many functions require it
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Test cmsCreateLab2ProfileTHR with white point from fuzzer input
    if (Size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint;
        memcpy(&whitePoint, Data, sizeof(cmsCIExyY));
        cmsHPROFILE lab2Profile = cmsCreateLab2ProfileTHR(context, &whitePoint);
        if (lab2Profile) {
            cmsCloseProfile(lab2Profile);
        }
    } else {
        // Test with NULL white point (defaults to D50)
        cmsHPROFILE lab2Profile = cmsCreateLab2ProfileTHR(context, NULL);
        if (lab2Profile) {
            cmsCloseProfile(lab2Profile);
        }
    }

    // Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    if (xyzProfile) {
        cmsCloseProfile(xyzProfile);
    }

    // Test cmsOpenProfileFromFileTHR with fuzzer data written to file
    FILE *file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(Data, 1, Size, file);
        fclose(file);
        
        cmsHPROFILE fileProfile = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
        if (fileProfile) {
            // Test cmsGetProfileContextID on the opened profile
            cmsContext profileContext = cmsGetProfileContextID(fileProfile);
            (void)profileContext; // Use variable to avoid unused warning
            
            cmsCloseProfile(fileProfile);
        }
        remove("./dummy_file");
    }

    // Test cmsCreateLab4ProfileTHR with white point from fuzzer input
    if (Size >= sizeof(cmsCIExyY)) {
        cmsCIExyY whitePoint2;
        // Use different offset for second white point if enough data
        size_t offset = Size >= 2 * sizeof(cmsCIExyY) ? sizeof(cmsCIExyY) : 0;
        memcpy(&whitePoint2, Data + offset, sizeof(cmsCIExyY));
        cmsHPROFILE lab4Profile = cmsCreateLab4ProfileTHR(context, &whitePoint2);
        if (lab4Profile) {
            cmsCloseProfile(lab4Profile);
        }
    } else {
        // Test with NULL white point (defaults to D50)
        cmsHPROFILE lab4Profile = cmsCreateLab4ProfileTHR(context, NULL);
        if (lab4Profile) {
            cmsCloseProfile(lab4Profile);
        }
    }

    // Cleanup context
    cmsDeleteContext(context);

    return 0;
}