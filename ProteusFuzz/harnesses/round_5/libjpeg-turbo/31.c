// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_buffers(unsigned char **dstBufs, int n) {
    if (!dstBufs) return;
    for (int i = 0; i < n; i++) {
        if (dstBufs[i]) {
            tjFree(dstBufs[i]);
            dstBufs[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Minimum size check to safely read initial parameters
    if (Size < 8) return 0;

    // Safely read operation code and number of transforms
    // Ensure alignment is not an issue by using memcpy or careful casting
    uint32_t op_code;
    uint32_t n_transforms;
    
    memcpy(&op_code, Data, sizeof(uint32_t));
    memcpy(&n_transforms, Data + 4, sizeof(uint32_t));
    
    // Limit n_transforms to prevent excessive allocation
    if (n_transforms > 10) n_transforms = 10;
    
    // Ensure we have enough data for the JPEG buffer if needed
    size_t jpeg_data_start = 8;
    size_t jpeg_size = (Size > jpeg_data_start) ? (Size - jpeg_data_start) : 0;
    const unsigned char *jpeg_data = (Size > jpeg_data_start) ? (Data + jpeg_data_start) : NULL;

    // Initialize handle based on operation code
    tjhandle handle = NULL;
    int init_type = op_code & 0xFF;
    
    if (init_type == 0) {
        handle = tjInitTransform();
    } else if (init_type == 1) {
        handle = tj3Init(TJINIT_DECOMPRESS);
    } else if (init_type == 2) {
        handle = tj3Init(TJINIT_COMPRESS);
    } else {
        handle = tjInitTransform();
    }

    if (!handle) {
        if (init_type != 0) handle = tjInitTransform();
        if (!handle) return 0;
    }

    // Prepare transform structures
    tjtransform *transforms = NULL;
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    
    if (n_transforms > 0) {
        transforms = (tjtransform *)calloc(n_transforms, sizeof(tjtransform));
        dstBufs = (unsigned char **)calloc(n_transforms, sizeof(unsigned char *));
        dstSizes = (size_t *)calloc(n_transforms, sizeof(size_t));
        
        if (!transforms || !dstBufs || !dstSizes) {
            free(transforms);
            free(dstBufs);
            free(dstSizes);
            if (handle) tjDestroy(handle);
            return 0;
        }

        // Initialize transforms with random but valid-looking data
        for (int i = 0; i < n_transforms; i++) {
            transforms[i].r.x = (i % 100);
            transforms[i].r.y = (i % 100);
            transforms[i].r.w = (i % 100) + 1;
            transforms[i].r.h = (i % 100) + 1;
            
            transforms[i].op = (i % 8); 
            transforms[i].options = (i % 4);
            transforms[i].customFilter = NULL;
            transforms[i].data = NULL;
        }
    }

    int result = 0;

    // Execute target functions based on op_code
    switch (op_code & 0xFF) {
        case 0: // Test tj3TransformBufSize
            if (n_transforms > 0 && jpeg_data && jpeg_size > 0) {
                if (tj3DecompressHeader(handle, jpeg_data, jpeg_size) == 0) {
                    size_t buf_size = tj3TransformBufSize(handle, &transforms[0]);
                    if (buf_size > 0) {
                        unsigned char *test_buf = (unsigned char *)tjAlloc(buf_size);
                        if (test_buf) {
                            unsigned char *test_dst = NULL;
                            size_t test_size = 0;
                            tjtransform test_trans = transforms[0];
                            
                            test_dst = test_buf;
                            test_size = buf_size;
                            tj3Transform(handle, jpeg_data, jpeg_size, 1, &test_dst, &test_size, &test_trans);
                            
                            if (test_dst != test_buf) {
                                tjFree(test_dst);
                            } else {
                                tjFree(test_buf);
                            }
                        }
                    }
                }
            }
            break;

        case 1: // Test tjTransform (legacy)
            if (n_transforms > 0 && jpeg_data && jpeg_size > 0) {
                unsigned long *dstSizes_legacy = (unsigned long *)calloc(n_transforms, sizeof(unsigned long));
                if (dstSizes_legacy) {
                    for(int i=0; i<n_transforms; i++) dstSizes_legacy[i] = 0;
                    
                    result = tjTransform(handle, jpeg_data, (unsigned long)jpeg_size, n_transforms, 
                                        dstBufs, dstSizes_legacy, transforms, 0);
                    
                    for(int i=0; i<n_transforms; i++) {
                        if (dstBufs[i]) {
                            tjFree(dstBufs[i]);
                            dstBufs[i] = NULL;
                        }
                    }
                    free(dstSizes_legacy);
                }
            }
            break;

        case 2: // Test tj3Transform
            if (n_transforms > 0 && jpeg_data && jpeg_size > 0) {
                for(int i=0; i<n_transforms; i++) dstBufs[i] = NULL;
                
                result = tj3Transform(handle, jpeg_data, jpeg_size, n_transforms, 
                                      dstBufs, dstSizes, transforms);
                
                cleanup_buffers(dstBufs, n_transforms);
            }
            break;

        case 3: // Test tj3DecompressHeader
            if (jpeg_data && jpeg_size > 0) {
                result = tj3DecompressHeader(handle, jpeg_data, jpeg_size);
            }
            break;
            
        case 4: // Test tjInitTransform
            if (handle) {
                // Just ensure handle is valid
            }
            break;
            
        default:
            if (jpeg_data && jpeg_size > 0) {
                tj3DecompressHeader(handle, jpeg_data, jpeg_size);
                if (n_transforms > 0) {
                    tj3Transform(handle, jpeg_data, jpeg_size, n_transforms, 
                                 dstBufs, dstSizes, transforms);
                    cleanup_buffers(dstBufs, n_transforms);
                }
            }
            break;
    }

    // Cleanup
    free(transforms);
    free(dstBufs);
    free(dstSizes);
    
    if (handle) {
        tjDestroy(handle);
    }

    return 0;
}