// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void* safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024) return NULL;
    void* ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t* data, size_t size) {
    if (!data || size < 128) return NULL;
    // Attempt to create a profile from data, likely failing for random data which is fine for fuzzing
    // We use a small buffer to avoid huge allocations if the fuzzer provides large input
    size_t copy_size = size < 65536 ? size : 65536;
    uint8_t* buf = safe_malloc(copy_size);
    if (!buf) return NULL;
    memcpy(buf, data, copy_size);
    
    // cmsOpenProfileFromMem signature: (const void * MemPtr, cmsUInt32Number dwSize)
    // The context is handled internally or via global context if not specified in older versions,
    // but the API signature in lcms2.h provided in the error does not take a context as first arg.
    cmsHPROFILE h = cmsOpenProfileFromMem(buf, (cmsUInt32Number)copy_size);
    free(buf);
    return h;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Split input for different purposes
    size_t profile_size = Size / 3;
    size_t profile2_size = Size / 3;
    size_t remaining = Size - profile_size - profile2_size;

    // Create dummy profiles
    cmsHPROFILE hProfiles[2] = { NULL, NULL };
    hProfiles[0] = create_dummy_profile(ctx, Data, profile_size);
    if (Size > profile_size) {
        hProfiles[1] = create_dummy_profile(ctx, Data + profile_size, profile2_size);
    }

    cmsHTRANSFORM hTransform = NULL;
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    // Extract parameters from input if available
    if (Size >= 4) {
        inputFormat = *(const cmsUInt32Number*)(Data);
        if (Size >= 8) outputFormat = *(const cmsUInt32Number*)(Data + 4);
        if (Size >= 12) intent = *(const cmsUInt32Number*)(Data + 8);
        if (Size >= 16) flags = *(const cmsUInt32Number*)(Data + 12);
    }

    // Limit profile count to valid range for cmsCreateMultiprofileTransform
    cmsUInt32Number nProfiles = 0;
    if (hProfiles[0]) nProfiles = 1;
    if (hProfiles[1]) nProfiles = 2;
    
    // Fuzz nProfiles slightly but keep it safe
    if (Size >= 20) {
        uint32_t fuzz_n = *(const uint32_t*)(Data + 16);
        if (fuzz_n > 0 && fuzz_n <= nProfiles) nProfiles = fuzz_n;
        else if (fuzz_n > 255) nProfiles = 0; // Invalid, will fail safely
    }

    if (nProfiles > 0) {
        hTransform = cmsCreateMultiprofileTransform(hProfiles, nProfiles, inputFormat, outputFormat, intent, flags);
    }

    // Test 1: cmsGetTransformInputFormat
    if (hTransform) {
        cmsUInt32Number inFmt = cmsGetTransformInputFormat(hTransform);
        (void)inFmt; // Use to suppress warnings
    }
    // Test with NULL
    cmsGetTransformInputFormat(NULL);

    // Test 2: cmsGetTransformOutputFormat
    if (hTransform) {
        cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
        (void)outFmt;
    }
    cmsGetTransformOutputFormat(NULL);

    // Test 3: _cmsGetTransformFlags
    if (hTransform) {
        _cmsGetTransformFlags((struct _cmstransform_struct*)hTransform);
    }

    // Test 4: _cmsGetTransformWorkerFlags
    if (hTransform) {
        _cmsGetTransformWorkerFlags((struct _cmstransform_struct*)hTransform);
    }

    // Test 5: cmsDoTransformLineStride
    if (hTransform) {
        // Allocate buffers for transformation
        // Use remaining data size to determine buffer sizes, with limits
        size_t bufSize = (remaining > 0) ? remaining : 1024;
        if (bufSize > 1024 * 1024) bufSize = 1024 * 1024; // Cap at 1MB
        
        uint8_t* inBuf = safe_malloc(bufSize);
        uint8_t* outBuf = safe_malloc(bufSize);
        
        if (inBuf && outBuf) {
            // Fill input with fuzz data
            memcpy(inBuf, Data, Size < bufSize ? Size : bufSize);
            
            cmsUInt32Number pixelsPerLine = 0;
            cmsUInt32Number lineCount = 0;
            cmsUInt32Number bytesPerLineIn = 0;
            cmsUInt32Number bytesPerLineOut = 0;
            cmsUInt32Number bytesPerPlaneIn = 0;
            cmsUInt32Number bytesPerPlaneOut = 0;

            if (Size >= 24) {
                pixelsPerLine = *(const cmsUInt32Number*)(Data + 20);
                lineCount = *(const cmsUInt32Number*)(Data + 24);
            }
            if (Size >= 32) {
                bytesPerLineIn = *(const cmsUInt32Number*)(Data + 28);
                bytesPerLineOut = *(const cmsUInt32Number*)(Data + 32);
            }
            if (Size >= 40) {
                bytesPerPlaneIn = *(const cmsUInt32Number*)(Data + 36);
                bytesPerPlaneOut = *(const cmsUInt32Number*)(Data + 40);
            }

            // Sanity checks to avoid huge allocations or invalid logic in the transform
            if (pixelsPerLine > 0 && lineCount > 0) {
                 cmsDoTransformLineStride(hTransform, inBuf, outBuf, pixelsPerLine, lineCount, 
                                          bytesPerLineIn, bytesPerLineOut, 
                                          bytesPerPlaneIn, bytesPerPlaneOut);
            }
        }
        free(inBuf);
        free(outBuf);
    }

    // Cleanup
    if (hTransform) cmsDeleteTransform(hTransform);
    if (hProfiles[0]) cmsCloseProfile(hProfiles[0]);
    if (hProfiles[1]) cmsCloseProfile(hProfiles[1]);
    cmsDeleteContext(ctx);

    return 0;
}