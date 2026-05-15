// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Control byte to select function */
    uint8_t selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    /* Common variables */
    tjhandle handle = NULL;
    int width = 16, height = 16, pitch, pixelFormat, subsamp, jpegQual = 75, flags, align;
    unsigned char *srcBuf = NULL, *dstBuf = NULL, *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    const unsigned char *srcPlanes[3];
    int strides[3];
    
    /* Initialize with safe defaults */
    if (remaining_size >= 1) {
        width = (remaining_data[0] % 64) + 1;
    }
    if (remaining_size >= 2) {
        height = (remaining_data[1] % 64) + 1;
    }
    if (remaining_size >= 3) {
        jpegQual = (remaining_data[2] % 100) + 1;
    }
    
    pitch = width * 3;
    pixelFormat = TJPF_RGB;
    subsamp = TJSAMP_420;
    flags = 0;
    align = 4; /* Use a safe alignment value */
    
    /* For tjCompressFromYUV: allocate YUV buffer with correct size */
    if (selector == 2) {
        handle = tjInitCompress();
        if (!handle) goto cleanup;
        
        /* Calculate required YUV buffer size */
        int yuvWidth = (width + align - 1) & ~(align - 1);
        int yuvHeight = height;
        unsigned long yuvBufSize = tjBufSizeYUV2(yuvWidth, align, yuvHeight, subsamp);
        
        if (yuvBufSize == 0 || yuvBufSize > 1024 * 1024 * 10) { /* 10MB limit */
            if (handle) tjDestroy(handle);
            handle = NULL;
            goto cleanup;
        }
        
        srcBuf = (unsigned char *)malloc(yuvBufSize);
        if (!srcBuf) {
            if (handle) tjDestroy(handle);
            handle = NULL;
            goto cleanup;
        }
        
        /* Fill YUV buffer with fuzzer data safely */
        size_t copy_size = remaining_size < yuvBufSize ? remaining_size : yuvBufSize;
        if (copy_size > 0 && remaining_size >= 3) {
            /* Skip the first 3 bytes we used for width, height, jpegQual */
            size_t data_offset = 3;
            if (remaining_size > data_offset) {
                size_t available = remaining_size - data_offset;
                copy_size = available < yuvBufSize ? available : yuvBufSize;
                memcpy(srcBuf, remaining_data + data_offset, copy_size);
            }
        }
        
        tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                         &jpegBuf, &jpegSize, jpegQual, flags);
        
        if (jpegBuf) tjFree(jpegBuf);
        jpegBuf = NULL;
        
        if (handle) tjDestroy(handle);
        handle = NULL;
        
        if (srcBuf) free(srcBuf);
        srcBuf = NULL;
        
        goto cleanup;
    }
    
    /* For other functions: allocate RGB buffer */
    size_t srcBufSize = pitch * height;
    if (srcBufSize == 0 || srcBufSize > 1024 * 1024 * 10) goto cleanup; /* 10MB limit */
    
    srcBuf = (unsigned char *)malloc(srcBufSize);
    if (!srcBuf) goto cleanup;
    
    /* Fill source buffer with fuzzer data safely */
    size_t copy_size = remaining_size < srcBufSize ? remaining_size : srcBufSize;
    if (copy_size > 0) {
        memcpy(srcBuf, remaining_data, copy_size);
    }
    
    /* Initialize YUV planes */
    for (int i = 0; i < 3; i++) {
        srcPlanes[i] = srcBuf;
        strides[i] = width;
    }
    
    switch (selector) {
        case 0: /* tjDecodeYUVPlanes */
            handle = tjInitDecompress();
            if (!handle) break;
            
            dstBuf = (unsigned char *)malloc(width * height * tjPixelSize[pixelFormat]);
            if (!dstBuf) break;
            
            tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                             dstBuf, width, pitch, height, pixelFormat, flags);
            free(dstBuf);
            dstBuf = NULL;
            
            if (handle) tjDestroy(handle);
            handle = NULL;
            break;
            
        case 1: /* tjFree */
            if (remaining_size > 0) {
                unsigned char *buffer = tjAlloc(remaining_size);
                if (buffer) {
                    memcpy(buffer, remaining_data, remaining_size);
                    tjFree(buffer);
                }
            }
            break;
            
        case 3: /* tjInitCompress */
            handle = tjInitCompress();
            if (handle) tjDestroy(handle);
            handle = NULL;
            break;
            
        case 4: /* tjCompress2 */
            handle = tjInitCompress();
            if (!handle) break;
            
            tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                       &jpegBuf, &jpegSize, subsamp, jpegQual, flags);
            
            if (jpegBuf) tjFree(jpegBuf);
            jpegBuf = NULL;
            
            if (handle) tjDestroy(handle);
            handle = NULL;
            break;
            
        case 5: /* tjCompress */
            handle = tjInitCompress();
            if (!handle) break;
            
            /* Create destination buffer with size check */
            unsigned long bufSize = tjBufSize(width, height, subsamp);
            if (bufSize == 0 || bufSize > 1024 * 1024 * 10) {
                if (handle) tjDestroy(handle);
                handle = NULL;
                break;
            }
            
            dstBuf = (unsigned char *)malloc(bufSize);
            if (!dstBuf) break;
            
            unsigned long compressedSize = 0;
            tjCompress(handle, srcBuf, width, pitch, height, 3,
                      dstBuf, &compressedSize, subsamp, jpegQual, flags);
            
            free(dstBuf);
            dstBuf = NULL;
            
            if (handle) tjDestroy(handle);
            handle = NULL;
            break;
    }
    
cleanup:
    if (srcBuf) free(srcBuf);
    if (dstBuf) free(dstBuf);
    if (jpegBuf) tjFree(jpegBuf);
    if (handle) tjDestroy(handle);
    
    /* Clean up any dummy file */
    cleanup_dummy_file();
    
    return 0;
}