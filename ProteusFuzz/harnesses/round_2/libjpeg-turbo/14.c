// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Use first 4 bytes to determine operation mode
    int op_mode = Data[0] % 6;
    
    tjhandle handle = NULL;
    int ret = 0;
    unsigned char *dstBuf = NULL;
    unsigned long dstSize = 0;
    unsigned char *iccBuf = NULL;
    size_t iccSize = 0;
    tjtransform transform;
    
    // Initialize transform structure safely
    memset(&transform, 0, sizeof(transform));
    transform.r.w = 0;
    transform.r.h = 0;
    transform.r.x = 0;
    transform.r.y = 0;
    transform.op = 0;
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // Ensure we have enough data for JPEG-like content
    if (Size < 10) return 0;

    // Initialize decompression handle for operations 0-3
    if (op_mode < 4) {
        handle = tjInitDecompress();
        if (!handle) return 0;
    } 
    // Initialize compression handle for operations 4-5
    else {
        handle = tjInitCompress();
        if (!handle) return 0;
    }

    switch (op_mode) {
        case 0: // Test tj3DecompressHeader
            ret = tj3DecompressHeader(handle, Data, Size);
            break;
            
        case 1: // Test tj3DecompressHeader with SAVEMARKERS and tj3GetICCProfile
            {
                tjhandle h2 = tjInitDecompress();
                if (h2) {
                    ret = tj3DecompressHeader(h2, Data, Size);
                    if (ret == 0) {
                        ret = tj3GetICCProfile(h2, &iccBuf, &iccSize);
                        if (iccBuf) {
                            tj3Free(iccBuf);
                            iccBuf = NULL;
                        }
                    }
                    tjDestroy(h2);
                }
            }
            break;
            
        case 2: // Test tjTransform
            {
                tjhandle h3 = tjInitDecompress();
                if (h3) {
                    transform.op = Data[1] % 7; 
                    transform.options = Data[2] % 8;
                    
                    dstBuf = malloc(Size * 2);
                    if (dstBuf) {
                        dstSize = Size * 2;
                        ret = tjTransform(h3, Data, Size, 1, &dstBuf, &dstSize, &transform, 0);
                        free(dstBuf);
                        dstBuf = NULL;
                    }
                    tjDestroy(h3);
                }
            }
            break;
            
        case 3: // Test error handling with tj3GetErrorCode
            {
                tjhandle h4 = tjInitDecompress();
                if (h4) {
                    ret = tj3DecompressHeader(h4, NULL, 0);
                    if (ret == -1) {
                        int errorCode = tj3GetErrorCode(h4);
                        (void)errorCode;
                    }
                    tjDestroy(h4);
                }
            }
            break;
            
        case 4: // Test tj3SetICCProfile
            {
                tjhandle h5 = tjInitCompress();
                if (h5) {
                    size_t iccLen = (Data[1] % (Size - 2)) + 1;
                    ret = tj3SetICCProfile(h5, (unsigned char*)&Data[2], iccLen);
                    ret = tj3SetICCProfile(h5, NULL, 0);
                    tjDestroy(h5);
                }
            }
            break;
            
        case 5: // Test tjDestroy with invalid handle
            {
                // Test destroying NULL handle
                ret = tjDestroy(NULL);
                
                // Test destroying a handle after it's been destroyed (Double Free check)
                tjhandle h6 = tjInitDecompress();
                if (h6) {
                    tjDestroy(h6);
                    // Do NOT call tjDestroy(h6) again. 
                    // Instead, verify the handle is invalid by checking error code if possible,
                    // or just skip the double free to prevent ASAN errors.
                    // The library might return -1 for invalid handles, but calling it again
                    // on a freed pointer causes Use-After-Free.
                    // We can safely ignore the second call or just not do it.
                    // To test the "invalid handle" behavior safely, we can pass a known invalid pointer
                    // if the API allows, but passing a freed pointer is unsafe.
                    // We'll just rely on the NULL test above for invalid handle logic.
                }
            }
            break;
    }

    // Cleanup
    if (handle) {
        tjDestroy(handle);
    }
    if (iccBuf) {
        tj3Free(iccBuf);
    }
    if (dstBuf) {
        free(dstBuf);
    }

    return 0;
}