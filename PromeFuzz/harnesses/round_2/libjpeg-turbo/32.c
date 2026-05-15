// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int customFilter(short *coeffs, tjregion arrayRegion,
                       tjregion planeRegion, int componentID, int transformID,
                       struct tjtransform *transform) {
    // Simple custom filter that does nothing
    return 0;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Create a dummy JPEG file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Read the file back
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (file_size < 100) {
        fclose(fp);
        return 0;
    }

    unsigned char *jpegBuf = (unsigned char *)malloc(file_size);
    if (!jpegBuf) {
        fclose(fp);
        return 0;
    }
    
    fread(jpegBuf, 1, file_size, fp);
    fclose(fp);

    // Test tj3Alloc
    size_t alloc_size = Size % 1024 + 1;
    void *allocated_buf = tj3Alloc(alloc_size);
    if (allocated_buf) {
        // Test with different allocation sizes
        void *small_alloc = tj3Alloc(1);
        void *large_alloc = tj3Alloc(alloc_size * 2);
        
        // Free allocated memory
        if (small_alloc) tj3Free(small_alloc);
        if (large_alloc) tj3Free(large_alloc);
        tj3Free(allocated_buf);
    }

    // Test old API tjInitTransform and tjTransform
    tjhandle old_handle = tjInitTransform();
    if (old_handle) {
        int width, height;
        // Test tjDecompressHeader
        int header_result = tjDecompressHeader(old_handle, jpegBuf, (unsigned long)file_size, &width, &height);
        
        if (header_result == 0 && width > 0 && height > 0) {
            // Prepare transform parameters
            tjtransform transform;
            transform.r.x = 0;
            transform.r.y = 0;
            transform.r.w = width / 2;
            transform.r.h = height / 2;
            transform.op = TJXOP_NONE;
            transform.options = TJXOPT_PERFECT | TJXOPT_CROP;
            transform.data = NULL;
            transform.customFilter = customFilter;
            
            // Prepare destination buffers
            unsigned char *dstBuf = NULL;
            unsigned long dstSize = 0;
            
            // Test tjTransform
            int transform_result = tjTransform(old_handle, jpegBuf, (unsigned long)file_size,
                                              1, &dstBuf, &dstSize, &transform, 0);
            
            if (dstBuf) {
                free(dstBuf);
            }
            
            // Test with different transform operations
            transform.op = Data[0] % 4;  // Random transform operation
            transform.options = Data[1] % 256;
            
            dstBuf = NULL;
            dstSize = 0;
            tjTransform(old_handle, jpegBuf, (unsigned long)file_size,
                       1, &dstBuf, &dstSize, &transform, 0);
            
            if (dstBuf) {
                free(dstBuf);
            }
        }
        
        // Cleanup old handle
        tjDestroy(old_handle);
    }

    // Test new API tj3Transform functions
    tjhandle new_handle = tj3Init(TJINIT_TRANSFORM);
    if (new_handle) {
        // Set some parameters
        tj3Set(new_handle, TJPARAM_FASTDCT, Data[2] % 2);
        tj3Set(new_handle, TJPARAM_FASTUPSAMPLE, Data[3] % 2);
        
        // Read JPEG header for new API
        int width, height, jpegSubsamp, jpegColorspace;
        int decompress_result = tj3DecompressHeader(new_handle, jpegBuf, file_size);
        
        if (decompress_result == 0) {
            width = tj3Get(new_handle, TJPARAM_JPEGWIDTH);
            height = tj3Get(new_handle, TJPARAM_JPEGHEIGHT);
            
            if (width > 0 && height > 0) {
                // Test tj3TransformBufSize
                tjtransform transform;
                transform.r.x = Data[4] % width;
                transform.r.y = Data[5] % height;
                transform.r.w = (Data[6] % (width - transform.r.x)) + 1;
                transform.r.h = (Data[7] % (height - transform.r.y)) + 1;
                transform.op = Data[8] % 4;
                transform.options = Data[9] % 256;
                transform.data = NULL;
                transform.customFilter = customFilter;
                
                size_t buf_size = tj3TransformBufSize(new_handle, &transform);
                
                // Test tj3Transform with pre-allocated buffer
                if (buf_size > 0 && buf_size < 10 * 1024 * 1024) {  // Limit to 10MB
                    unsigned char *dstBuf = (unsigned char *)tj3Alloc(buf_size);
                    size_t dstSize = buf_size;
                    
                    if (dstBuf) {
                        int n = 1;
                        size_t dstSizes[1] = {dstSize};
                        unsigned char *dstBufs[1] = {dstBuf};
                        tjtransform transforms[1] = {transform};
                        
                        int result = tj3Transform(new_handle, jpegBuf, file_size,
                                                 n, dstBufs, dstSizes, transforms);
                        
                        tj3Free(dstBuf);
                    }
                }
                
                // Test tj3Transform with auto-allocation (NULL buffer)
                unsigned char *autoBuf = NULL;
                size_t autoSize = 0;
                int n = 1;
                size_t dstSizes[1] = {0};
                unsigned char *dstBufs[1] = {NULL};
                tjtransform transforms[1] = {transform};
                
                int auto_result = tj3Transform(new_handle, jpegBuf, file_size,
                                             n, dstBufs, dstSizes, transforms);
                
                if (dstBufs[0]) {
                    tj3Free(dstBufs[0]);
                }
                
                // Test multiple transforms
                if (Size > 200) {
                    n = 2;
                    tjtransform transforms2[2];
                    unsigned char *dstBufs2[2] = {NULL, NULL};
                    size_t dstSizes2[2] = {0, 0};
                    
                    for (int i = 0; i < 2; i++) {
                        transforms2[i].r.x = Data[10 + i*4] % width;
                        transforms2[i].r.y = Data[11 + i*4] % height;
                        transforms2[i].r.w = (Data[12 + i*4] % (width - transforms2[i].r.x)) + 1;
                        transforms2[i].r.h = (Data[13 + i*4] % (height - transforms2[i].r.y)) + 1;
                        transforms2[i].op = Data[14 + i] % 4;
                        transforms2[i].options = Data[15 + i] % 256;
                        transforms2[i].data = NULL;
                        transforms2[i].customFilter = customFilter;
                    }
                    
                    tj3Transform(new_handle, jpegBuf, file_size,
                               n, dstBufs2, dstSizes2, transforms2);
                    
                    for (int i = 0; i < 2; i++) {
                        if (dstBufs2[i]) {
                            tj3Free(dstBufs2[i]);
                        }
                    }
                }
            }
        }
        
        // Cleanup new handle
        tj3Destroy(new_handle);
    }

    free(jpegBuf);
    return 0;
}