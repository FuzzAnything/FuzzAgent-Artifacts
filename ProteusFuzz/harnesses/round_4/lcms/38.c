// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsUInt64Number flags = 0;
    cmsUInt64Number qword_src = 0;
    cmsUInt64Number qword_dst = 0;
    cmsInt32Number tagCount = 0;
    cmsTagSignature sig = 0;
    void *tagData = NULL;

    // Attempt to open a profile from the fuzzed data
    // lcms expects a valid ICC profile, but we pass raw data to trigger parsing errors
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile == NULL) {
        // If profile is invalid, we can still test functions that handle NULL gracefully
        // or test with dummy values for non-profile dependent functions
        
        // Test _cmsAdjustEndianess64 with dummy data
        qword_src = Data[0] | ((uint64_t)Data[0] << 8);
        _cmsAdjustEndianess64(&qword_dst, &qword_src);

        // cmsGetTagCount with NULL should return -1
        tagCount = cmsGetTagCount(NULL);
        if (tagCount != -1) return 0; // Sanity check

        return 0;
    }

    // 1. Test cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        // 2. Test cmsGetTagSignature for each tag
        for (cmsUInt32Number i = 0; i < (cmsUInt32Number)tagCount; i++) {
            sig = cmsGetTagSignature(hProfile, i);
            if (sig != 0) {
                // 3. Test cmsReadTag
                tagData = cmsReadTag(hProfile, sig);
                if (tagData != NULL) {
                    // We don't free tagData as lcms manages it internally in the profile
                    // Just ensure we don't crash accessing it if it's valid
                }
            }
        }
    }

    // 4. Test cmsSetHeaderAttributes
    // Use fuzzed data to generate flags, ensuring we don't overflow if Size is small
    if (Size >= 8) {
        memcpy(&flags, Data, 8);
    } else if (Size > 0) {
        flags = Data[0];
    }
    cmsSetHeaderAttributes(hProfile, flags);

    // 5. Test cmsGetHeaderAttributes
    cmsGetHeaderAttributes(hProfile, &flags);

    // 6. Test _cmsAdjustEndianess64 with profile-related data if possible
    // Reuse flags as input
    _cmsAdjustEndianess64(&qword_dst, &flags);

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}