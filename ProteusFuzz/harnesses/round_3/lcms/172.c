// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static void write_dummy_profile(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_172(const uint8_t *Data, size_t Size) {
    if (Size < 128) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsUInt64Number input64, output64;
    cmsTagSignature sig;
    cmsInt32Number tagCount;
    cmsUInt32Number index;
    void *tagData = NULL;

    // Initialize 64-bit values for _cmsAdjustEndianess64
    input64 = 0;
    output64 = 0;

    // Fuzz _cmsAdjustEndianess64
    if (Size >= 16) {
        memcpy(&input64, Data, sizeof(cmsUInt64Number));
        _cmsAdjustEndianess64(&output64, &input64);
        // Also test with zeroed input
        _cmsAdjustEndianess64(&output64, &input64);
    }

    // Write dummy file for profile loading
    write_dummy_profile(Data, Size, "./dummy_file");

    // Attempt to load profile
    hProfile = cmsOpenProfileFromFile("./dummy_file", "r");

    if (hProfile) {
        // Fuzz cmsGetTagCount
        tagCount = cmsGetTagCount(hProfile);

        // Fuzz cmsGetTagSignature with various indices
        if (tagCount > 0) {
            for (int i = 0; i < 10 && i < tagCount; i++) {
                index = (Size > 0) ? (Data[i] % (tagCount + 5)) : 0;
                sig = cmsGetTagSignature(hProfile, index);
                if (sig != 0) {
                    // Fuzz cmsReadTag
                    tagData = cmsReadTag(hProfile, sig);
                    if (tagData) {
                        // Do nothing, just ensure no crash
                    }
                }
            }
            // Also test out-of-bounds index
            sig = cmsGetTagSignature(hProfile, (cmsUInt32Number)(tagCount + 100));
        }

        // Fuzz cmsGetHeaderAttributes
        cmsUInt64Number flags = 0;
        cmsGetHeaderAttributes(hProfile, &flags);

        // Fuzz cmsSetHeaderAttributes with various flag values
        if (Size >= 8) {
            cmsUInt64Number fuzzFlags;
            memcpy(&fuzzFlags, Data + 8, sizeof(cmsUInt64Number));
            cmsSetHeaderAttributes(hProfile, fuzzFlags);
        }

        // Cleanup
        cmsCloseProfile(hProfile);
    }

    return 0;
}