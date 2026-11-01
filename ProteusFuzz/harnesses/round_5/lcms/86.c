// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE create_editable_profile(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hSrc = NULL;
    
    // Try to create a profile from the fuzzed data if it looks like an ICC profile
    if (Size >= 12) {
        hSrc = cmsOpenProfileFromMem(Data, Size);
        if (hSrc) {
            // Create a new editable profile (sRGB-like) to write to
            hProfile = cmsCreate_sRGBProfile();
            if (hProfile) {
                // Try to copy some tags from the source if it's valid
                cmsTagSignature sigs[] = { 
                    cmsSigAToB0Tag, 
                    cmsSigAToB1Tag, 
                    cmsSigBToA0Tag, 
                    cmsSigBToA1Tag, 
                    cmsSigChromaticAdaptationTag 
                };
                for (int i = 0; i < 5; i++) {
                    void *tagData = cmsReadTag(hSrc, sigs[i]);
                    if (tagData) {
                        cmsWriteTag(hProfile, sigs[i], tagData);
                    }
                }
            }
            cmsCloseProfile(hSrc);
        }
    }
    
    // If we couldn't create from data or data was invalid, create a fresh sRGB profile
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = create_editable_profile(Data, Size);
    if (!hProfile) return 0;

    // Extract potential tag signatures from input data
    cmsTagSignature sig1 = 0;
    cmsTagSignature sig2 = 0;
    
    if (Size >= 4) {
        memcpy(&sig1, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&sig2, Data + 4, 4);
    }
    
    // Fuzz cmsTagLinkedTo
    cmsTagSignature linked = cmsTagLinkedTo(hProfile, sig1);
    (void)linked; // Suppress unused warning

    // Fuzz cmsIsTag
    cmsBool exists = cmsIsTag(hProfile, sig1);
    (void)exists;

    // Fuzz cmsWriteRawTag
    // Use input data as raw tag data
    if (Size > 4) {
        cmsBool writeResult = cmsWriteRawTag(hProfile, sig1, Data + 4, Size - 4);
        (void)writeResult;
    }

    // Fuzz cmsReadTag
    void *tagData = cmsReadTag(hProfile, sig1);
    if (tagData) {
        // Just access to ensure it's valid, don't free (managed by lcms)
        // We can't safely free it here as we don't know the type
    }
    
    // Fuzz cmsLinkTag
    cmsBool linkResult = cmsLinkTag(hProfile, sig1, sig2);
    (void)linkResult;

    // Fuzz cmsWriteTag
    // Write a simple dummy structure or the input data
    if (Size > 4) {
        cmsBool writeTagResult = cmsWriteTag(hProfile, sig1, Data + 4);
        (void)writeTagResult;
    }
    
    // Try to delete a tag by passing NULL
    cmsBool deleteResult = cmsWriteTag(hProfile, sig1, NULL);
    (void)deleteResult;

    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}