// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static unsigned long get_ulong(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned long)) {
        return 0;
    }
    unsigned long value;
    memcpy(&value, *data, sizeof(unsigned long));
    *data += sizeof(unsigned long);
    *size -= sizeof(unsigned long);
    return value;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    // Initialize handles
    tjhandle decompress_handle = tjInitDecompress();
    tjhandle compress_handle = tjInitCompress();
    
    if (!decompress_handle || !compress_handle) {
        if (decompress_handle) tjDestroy(decompress_handle);
        if (compress_handle) tjDestroy(compress_handle);
        return 0;
    }

    // Extract parameters from fuzz data
    int width = get_int(&data_ptr, &data_size);
    int height = get_int(&data_ptr, &data_size);
    int subsamp = get_int(&data_ptr, &data_size) % 5;  // 0-4 valid subsamp values
    int componentID = get_int(&data_ptr, &data_size) % 3;  // 0-2 valid component IDs
    int align = get_int(&data_ptr, &data_size) % 32 + 1;  // 1-32
    int jpegQual = get_int(&data_ptr, &data_size) % 101;  // 0-100
    int flags = get_int(&data_ptr, &data_size);
    int pixelFormat = get_int(&data_ptr, &data_size) % 12;  // 0-11 valid pixel formats
    int pitch = get_int(&data_ptr, &data_size);
    int pixelSize = get_int(&data_ptr, &data_size) % 5;  // 1-4 valid pixel sizes

    // Ensure reasonable dimensions to avoid excessive memory usage
    if (width < 1) width = 1;
    if (width > 4096) width = 4096;
    if (height < 1) height = 1;
    if (height > 4096) height = 4096;
    if (pitch < width * pixelSize) pitch = width * pixelSize;

    // Test tjDecompressHeader2
    if (data_size > 0) {
        unsigned long jpegSize = data_size;
        unsigned char *jpegBuf = (unsigned char *)data_ptr;
        int out_width, out_height, out_subsamp;
        
        tjDecompressHeader2(decompress_handle, jpegBuf, jpegSize, 
                           &out_width, &out_height, &out_subsamp);
    }

    // Test tj3YUVPlaneWidth
    tj3YUVPlaneWidth(componentID, width, subsamp);

    // Test tjCompressFromYUV
    if (data_size > 0) {
        size_t yuv_size = tjPlaneSizeYUV(componentID, width, align, height, subsamp);
        if (yuv_size > 0 && yuv_size <= data_size) {
            const unsigned char *srcBuf = data_ptr;
            unsigned char *jpegBuf = NULL;
            unsigned long jpegSize = 0;
            
            tjCompressFromYUV(compress_handle, srcBuf, width, align, height, subsamp,
                             &jpegBuf, &jpegSize, jpegQual, flags);
            
            if (jpegBuf) {
                tjFree(jpegBuf);
            }
        }
    }

    // Test tjDecodeYUV
    if (data_size > 0) {
        size_t yuv_size = tjPlaneSizeYUV(0, width, align, height, subsamp) +
                         tjPlaneSizeYUV(1, width, align, height, subsamp) +
                         tjPlaneSizeYUV(2, width, align, height, subsamp);
        
        if (yuv_size > 0 && yuv_size <= data_size) {
            const unsigned char *srcBuf = data_ptr;
            size_t dst_size = (size_t)pitch * height;
            unsigned char *dstBuf = NULL;
            
            if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {  // Limit to 100MB
                dstBuf = (unsigned char *)malloc(dst_size);
                if (dstBuf) {
                    tjDecodeYUV(decompress_handle, srcBuf, align, subsamp, dstBuf,
                               width, pitch, height, pixelFormat, flags);
                    free(dstBuf);
                }
            }
        }
    }

    // Test tjDecodeYUVPlanes
    if (data_size > 100) {
        const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        // Calculate plane sizes
        size_t plane_sizes[3];
        for (int i = 0; i < 3; i++) {
            plane_sizes[i] = tjPlaneSizeYUV(i, width, align, height, subsamp);
        }
        
        // Check if we have enough data for all planes
        size_t total_plane_size = plane_sizes[0] + plane_sizes[1] + plane_sizes[2];
        if (total_plane_size > 0 && total_plane_size <= data_size) {
            const uint8_t *plane_ptr = data_ptr;
            for (int i = 0; i < 3; i++) {
                if (plane_sizes[i] > 0) {
                    srcPlanes[i] = plane_ptr;
                    strides[i] = tjPlaneWidth(i, width, subsamp) * align;
                    plane_ptr += plane_sizes[i];
                }
            }
            
            size_t dst_size = (size_t)pitch * height;
            unsigned char *dstBuf = NULL;
            
            if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {  // Limit to 100MB
                dstBuf = (unsigned char *)malloc(dst_size);
                if (dstBuf) {
                    tjDecodeYUVPlanes(decompress_handle, srcPlanes, strides, subsamp,
                                     dstBuf, width, pitch, height, pixelFormat, flags);
                    free(dstBuf);
                }
            }
        }
    }

    // Test tjCompress
    if (data_size > 0) {
        size_t src_size = (size_t)pitch * height;
        if (src_size > 0 && src_size <= data_size) {
            unsigned char *srcBuf = (unsigned char *)data_ptr;
            unsigned char *dstBuf = NULL;
            unsigned long compressedSize = 0;
            
            // Allocate destination buffer (max 100MB)
            size_t max_dst_size = 100 * 1024 * 1024;
            dstBuf = (unsigned char *)malloc(max_dst_size);
            if (dstBuf) {
                tjCompress(compress_handle, srcBuf, width, pitch, height, pixelSize,
                          dstBuf, &compressedSize, subsamp, jpegQual, flags);
                free(dstBuf);
            }
        }
    }

    // Cleanup
    tjDestroy(decompress_handle);
    tjDestroy(compress_handle);
    
    return 0;
}