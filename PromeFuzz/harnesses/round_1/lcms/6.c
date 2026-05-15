// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <wchar.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for profile loading
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open profile from dummy file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        remove("./dummy_file");
        return 0;
    }
    
    // Get context from profile for memory operations
    cmsContext context = cmsGetProfileContextID(hProfile);
    
    // Prepare parameters for cmsGetProfileInfo
    cmsInfoType infoType = cmsInfoDescription;
    char languageCode[3] = {'e', 'n', '\0'};
    char countryCode[3] = {'U', 'S', '\0'};
    
    // First call to cmsGetProfileInfo - get required buffer size
    cmsUInt32Number requiredSize = cmsGetProfileInfo(hProfile, infoType, 
                                                     languageCode, countryCode, 
                                                     NULL, 0);
    
    // Allocate memory using _cmsMalloc
    void *allocatedBuffer = NULL;
    if (requiredSize > 0 && requiredSize < 1024*1024) { // Sanity check
        allocatedBuffer = _cmsMalloc(context, requiredSize * sizeof(wchar_t));
    }
    
    // Second call to cmsGetProfileInfo - with allocated buffer
    if (allocatedBuffer) {
        cmsGetProfileInfo(hProfile, infoType, languageCode, countryCode,
                         (wchar_t*)allocatedBuffer, requiredSize);
    }
    
    // Free allocated memory
    if (allocatedBuffer) {
        _cmsFree(context, allocatedBuffer);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    remove("./dummy_file");
    
    return 0;
}