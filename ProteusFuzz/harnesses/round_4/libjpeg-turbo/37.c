// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void cleanup_buffers(unsigned char **dstBufs, int n) {
    if (dstBufs) {
        for (int i = 0; i < n; i++) {
            if (dstBufs[i]) {
                free(dstBufs[i]);
                dstBufs[i] = NULL;
            }
        }
    }
}

static uint8_t get_data_byte(size_t idx, const uint8_t *Data, size_t Size) {
    if (idx < Size) {
        return Data[idx];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    // Use first byte to determine operation mode
    int mode = Data[0] % 5;
    
    // Create a dummy JPEG file for testing if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    tjhandle handle = NULL;
    tjhandle transformHandle = NULL;
    unsigned char *jpegBuf = (unsigned char *)Data;
    size_t jpegSize = Size;
    int result = 0;
    
    // Allocate buffers for transforms
    // Ensure n is at least 1 and bounded by available data
    int n = (Data[1] % 5) + 1;
    if (n > 10) n = 10; // Cap to prevent excessive allocation
    
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    tjtransform *transforms = NULL;

    // Initialize buffers
    if (n > 0) {
        dstBufs = (unsigned char **)calloc(n, sizeof(unsigned char *));
        dstSizes = (size_t *)calloc(n, sizeof(size_t));
        transforms = (tjtransform *)calloc(n, sizeof(tjtransform));
        
        if (!dstBufs || !dstSizes || !transforms) {
            free(dstBufs);
            free(dstSizes);
            free(transforms);
            return 0;
        }
    }

    switch (mode) {
        case 0:
            // Test tj3Init + tj3DecompressHeader + tj3TransformBufSize
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle) {
                if (tj3DecompressHeader(handle, jpegBuf, jpegSize) == 0) {
                    if (n > 0) {
                        // Set up a simple transform
                        transforms[0].op = TJXOP_NONE;
                        transforms[0].options = 0;
                        transforms[0].r.w = 0;
                        transforms[0].r.h = 0;
                        
                        size_t bufSize = tj3TransformBufSize(handle, &transforms[0]);
                        (void)bufSize; // Use the result to avoid unused variable warning
                    }
                }
                cleanup_handle(handle);
            }
            break;
            
        case 1:
            // Test tjInitTransform + tj3Transform
            transformHandle = tjInitTransform();
            if (transformHandle) {
                if (n > 0) {
                    // Set up transform parameters
                    for (int i = 0; i < n; i++) {
                        // Safe index calculation: use modulo on available size
                        size_t idx = 2 + (i % (Size > 2 ? Size - 2 : 1));
                        if (idx >= Size) idx = 2;
                        
                        transforms[i].op = get_data_byte(idx, Data, Size) % 6;
                        transforms[i].options = 0;
                        transforms[i].r.w = 0;
                        transforms[i].r.h = 0;
                        transforms[i].customFilter = NULL;
                        transforms[i].data = NULL;
                    }
                    
                    result = tj3Transform(transformHandle, jpegBuf, jpegSize, n, 
                                        dstBufs, dstSizes, transforms);
                    (void)result;
                    
                    // Cleanup allocated buffers
                    cleanup_buffers(dstBufs, n);
                }
                cleanup_handle(transformHandle);
            }
            break;
            
        case 2:
            // Test tj3Init + tj3DecompressHeader + tjTransform (legacy)
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle) {
                if (tj3DecompressHeader(handle, jpegBuf, jpegSize) == 0) {
                    if (n > 0) {
                        unsigned long *legacyDstSizes = (unsigned long *)calloc(n, sizeof(unsigned long));
                        if (legacyDstSizes) {
                            for (int i = 0; i < n; i++) {
                                size_t idx = 2 + (i % (Size > 2 ? Size - 2 : 1));
                                if (idx >= Size) idx = 2;
                                
                                transforms[i].op = get_data_byte(idx, Data, Size) % 6;
                                transforms[i].options = 0;
                                transforms[i].r.w = 0;
                                transforms[i].r.h = 0;
                                transforms[i].customFilter = NULL;
                                transforms[i].data = NULL;
                            }
                            
                            result = tjTransform(handle, jpegBuf, (unsigned long)jpegSize, n,
                                               dstBufs, legacyDstSizes, transforms, 0);
                            (void)result;
                            
                            cleanup_buffers(dstBufs, n);
                            free(legacyDstSizes);
                        }
                    }
                }
                cleanup_handle(handle);
            }
            break;
            
        case 3:
            // Test tjInitTransform with multiple transforms
            transformHandle = tjInitTransform();
            if (transformHandle) {
                if (n > 0) {
                    // Set up various transform operations
                    for (int i = 0; i < n; i++) {
                        size_t idx1 = 2 + (i % (Size > 2 ? Size - 2 : 1));
                        size_t idx2 = 3 + (i % (Size > 3 ? Size - 3 : 1));
                        if (idx1 >= Size) idx1 = 2;
                        if (idx2 >= Size) idx2 = 3;
                        
                        transforms[i].op = get_data_byte(idx1, Data, Size) % 6;
                        transforms[i].options = get_data_byte(idx2, Data, Size) % 4;
                        transforms[i].r.w = 0;
                        transforms[i].r.h = 0;
                        transforms[i].customFilter = NULL;
                        transforms[i].data = NULL;
                    }
                    
                    // Try with pre-allocated buffers
                    for (int i = 0; i < n; i++) {
                        dstBufs[i] = (unsigned char *)malloc(Size * 2);
                        if (dstBufs[i]) {
                            dstSizes[i] = Size * 2;
                        }
                    }
                    
                    result = tj3Transform(transformHandle, jpegBuf, jpegSize, n,
                                        dstBufs, dstSizes, transforms);
                    (void)result;
                    
                    cleanup_buffers(dstBufs, n);
                }
                cleanup_handle(transformHandle);
            }
            break;
            
        case 4:
            // Test error conditions with invalid data
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (handle) {
                // Try with very small size
                if (tj3DecompressHeader(handle, jpegBuf, 1) != 0) {
                    // Expected error
                }
                
                // Try with NULL buffer
                if (tj3DecompressHeader(handle, NULL, jpegSize) != 0) {
                    // Expected error
                }
                
                cleanup_handle(handle);
            }
            break;
    }

    // Cleanup
    free(dstBufs);
    free(dstSizes);
    free(transforms);
    
    return 0;
}