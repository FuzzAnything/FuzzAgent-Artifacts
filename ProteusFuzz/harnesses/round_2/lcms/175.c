// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsTagSignature sig = 0;
    cmsUInt32Number tagCount = 0;
    void *tagData = NULL;
    unsigned char buffer[4096];
    cmsUInt32Number bytesRead = 0;

    // Create a default XYZ profile to use as a base
    hProfile = cmsCreateXYZProfile();
    if (hProfile == NULL) {
        return 0;
    }

    // Get tag count
    cmsInt32Number count = cmsGetTagCount(hProfile);
    if (count > 0) {
        tagCount = (cmsUInt32Number)count;
    }

    // Use input data to determine which tag to inspect
    if (Size >= 4) {
        sig = *(const cmsTagSignature *)Data;
    } else if (Size > 0) {
        // Construct a signature from available bytes
        sig = Data[0];
        if (Size > 1) sig |= (Data[1] << 8);
        if (Size > 2) sig |= (Data[2] << 16);
        if (Size > 3) sig |= (Data[3] << 24);
    }

    // Test cmsGetTagSignature with various indices
    if (tagCount > 0) {
        cmsUInt32Number idx = 0;
        
        // Safely extract index from input data
        if (Size >= 4) {
            // Ensure we don't read past the buffer
            if (Size >= 8) {
                idx = *(const cmsUInt32Number *)(Data + 4);
            } else {
                // Fallback if data is between 4 and 8 bytes
                idx = 0;
                if (Size > 4) idx |= Data[4];
                if (Size > 5) idx |= (Data[5] << 8);
                if (Size > 6) idx |= (Data[6] << 16);
                if (Size > 7) idx |= (Data[7] << 24);
            }
        } else {
            idx = 0;
        }
        
        // Test valid index
        if (idx < tagCount) {
            cmsTagSignature testSig = cmsGetTagSignature(hProfile, idx);
            if (testSig != 0) {
                // Test cmsTagLinkedTo
                cmsTagSignature linkedSig = cmsTagLinkedTo(hProfile, testSig);
                (void)linkedSig;

                // Test cmsReadTag
                tagData = cmsReadTag(hProfile, testSig);
                if (tagData != NULL) {
                    // Data is managed by lcms, do not free manually
                }

                // Test cmsReadRawTag
                bytesRead = cmsReadRawTag(hProfile, testSig, buffer, sizeof(buffer));
                (void)bytesRead;
            }
        }

        // Test out of bounds index
        cmsTagSignature outOfBoundsSig = cmsGetTagSignature(hProfile, tagCount + 100);
        (void)outOfBoundsSig;
    }

    // Test with the signature derived from input
    if (sig != 0) {
        cmsTagSignature linkedSig = cmsTagLinkedTo(hProfile, sig);
        (void)linkedSig;

        tagData = cmsReadTag(hProfile, sig);
        if (tagData != NULL) {
            // Data managed by lcms
        }

        bytesRead = cmsReadRawTag(hProfile, sig, buffer, sizeof(buffer));
        (void)bytesRead;
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}