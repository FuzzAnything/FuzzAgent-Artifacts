// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static unsigned long read_ulong(const uint8_t **data, size_t *size) {
    if (*size < sizeof(unsigned long)) {
        return 0;
    }
    unsigned long value;
    memcpy(&value, *data, sizeof(unsigned long));
    *data += sizeof(unsigned long);
    *size -= sizeof(unsigned long);
    return value;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    uint8_t function_selector = data[0] % 6;
    data++;
    size--;

    switch (function_selector) {
        case 0: { // tjDecodeYUVPlanes
            tjhandle handle = tjInitCompress();
            if (!handle) {
                return 0;
            }

            int width = abs(read_int(&data, &size)) % 1024 + 1;
            int height = abs(read_int(&data, &size)) % 1024 + 1;
            int pitch = abs(read_int(&data, &size)) % (width * 4) + 1;
            int subsamp = abs(read_int(&data, &size)) % 4;
            int pixelFormat = abs(read_int(&data, &size)) % 12;
            int flags = abs(read_int(&data, &size));

            const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
            int strides[3] = {0, 0, 0};
            
            size_t y_size = width * height;
            size_t uv_size = 0;
            switch (subsamp) {
                case TJSAMP_444: uv_size = width * height; break;
                case TJSAMP_422: uv_size = width * height / 2; break;
                case TJSAMP_420: uv_size = width * height / 4; break;
                case TJSAMP_GRAY: uv_size = 0; break;
                default: uv_size = width * height / 4; break;
            }

            unsigned char *y_plane = (unsigned char *)malloc(y_size);
            unsigned char *u_plane = (unsigned char *)malloc(uv_size);
            unsigned char *v_plane = (unsigned char *)malloc(uv_size);
            
            if (y_plane && u_plane && v_plane) {
                size_t copy_y = size > y_size ? y_size : size;
                if (copy_y > 0) {
                    memcpy(y_plane, data, copy_y);
                    data += copy_y;
                    size -= copy_y;
                }
                
                size_t copy_u = size > uv_size ? uv_size : size;
                if (copy_u > 0) {
                    memcpy(u_plane, data, copy_u);
                    data += copy_u;
                    size -= copy_u;
                }
                
                size_t copy_v = size > uv_size ? uv_size : size;
                if (copy_v > 0) {
                    memcpy(v_plane, data, copy_v);
                    data += copy_v;
                    size -= copy_v;
                }
                
                srcPlanes[0] = y_plane;
                srcPlanes[1] = u_plane;
                srcPlanes[2] = v_plane;
                
                strides[0] = width;
                strides[1] = width / 2;
                strides[2] = width / 2;
                
                unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
                if (dstBuf) {
                    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                     dstBuf, width, pitch, height, pixelFormat, flags);
                    free(dstBuf);
                }
            }
            
            if (y_plane) free(y_plane);
            if (u_plane) free(u_plane);
            if (v_plane) free(v_plane);
            tjDestroy(handle);
            break;
        }
        
        case 1: { // tjFree
            size_t alloc_size = abs(read_int(&data, &size)) % 1024 + 1;
            unsigned char *buffer = (unsigned char *)tjAlloc(alloc_size);
            if (buffer) {
                tjFree(buffer);
            }
            break;
        }
        
        case 2: { // tjCompressFromYUV
            tjhandle handle = tjInitCompress();
            if (!handle) {
                return 0;
            }

            int width = abs(read_int(&data, &size)) % 1024 + 1;
            int height = abs(read_int(&data, &size)) % 1024 + 1;
            int align = abs(read_int(&data, &size)) % 16 + 1;
            int subsamp = abs(read_int(&data, &size)) % 4;
            int jpegQual = abs(read_int(&data, &size)) % 100 + 1;
            int flags = abs(read_int(&data, &size));

            // Calculate correct buffer size with alignment
            size_t yuv_size;
            if (subsamp == TJSAMP_GRAY) {
                int stride0 = ((width + align - 1) / align) * align;
                yuv_size = stride0 * height;
            } else {
                int stride0 = ((width + align - 1) / align) * align;
                int stride1 = (((width + 1) / 2 + align - 1) / align) * align;
                int ph0 = height;
                int ph1 = (height + 1) / 2;
                yuv_size = stride0 * ph0 + 2 * stride1 * ph1;
            }
            
            unsigned char *srcBuf = (unsigned char *)malloc(yuv_size);
            if (srcBuf) {
                size_t copy_size = size > yuv_size ? yuv_size : size;
                if (copy_size > 0) {
                    memcpy(srcBuf, data, copy_size);
                }
                
                unsigned char *jpegBuf = NULL;
                unsigned long jpegSize = 0;
                
                tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                                 &jpegBuf, &jpegSize, jpegQual, flags);
                
                if (jpegBuf) {
                    tjFree(jpegBuf);
                }
                free(srcBuf);
            }
            
            tjDestroy(handle);
            break;
        }
        
        case 3: { // tjInitCompress
            tjhandle handle = tjInitCompress();
            if (handle) {
                tjDestroy(handle);
            }
            break;
        }
        
        case 4: { // tjCompress2
            tjhandle handle = tjInitCompress();
            if (!handle) {
                return 0;
            }

            int width = abs(read_int(&data, &size)) % 1024 + 1;
            int height = abs(read_int(&data, &size)) % 1024 + 1;
            int pitch = abs(read_int(&data, &size)) % (width * 4) + 1;
            int pixelFormat = abs(read_int(&data, &size)) % 12;
            int jpegSubsamp = abs(read_int(&data, &size)) % 4;
            int jpegQual = abs(read_int(&data, &size)) % 100 + 1;
            int flags = abs(read_int(&data, &size));

            size_t src_size = pitch * height;
            unsigned char *srcBuf = (unsigned char *)malloc(src_size);
            if (srcBuf) {
                size_t copy_size = size > src_size ? src_size : size;
                if (copy_size > 0) {
                    memcpy(srcBuf, data, copy_size);
                }
                
                unsigned char *jpegBuf = NULL;
                unsigned long jpegSize = 0;
                
                tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                           &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
                
                if (jpegBuf) {
                    tjFree(jpegBuf);
                }
                free(srcBuf);
            }
            
            tjDestroy(handle);
            break;
        }
        
        case 5: { // tjCompress
            tjhandle handle = tjInitCompress();
            if (!handle) {
                return 0;
            }

            int width = abs(read_int(&data, &size)) % 1024 + 1;
            int height = abs(read_int(&data, &size)) % 1024 + 1;
            int pitch = abs(read_int(&data, &size)) % (width * 4) + 1;
            int pixelSize = abs(read_int(&data, &size)) % 4 + 1;
            int jpegSubsamp = abs(read_int(&data, &size)) % 4;
            int jpegQual = abs(read_int(&data, &size)) % 100 + 1;
            int flags = abs(read_int(&data, &size));

            size_t src_size = pitch * height;
            unsigned char *srcBuf = (unsigned char *)malloc(src_size);
            if (srcBuf) {
                size_t copy_size = size > src_size ? src_size : size;
                if (copy_size > 0) {
                    memcpy(srcBuf, data, copy_size);
                }
                
                unsigned long compressedSize = 0;
                size_t dst_size = width * height * pixelSize + 1024;
                unsigned char *dstBuf = (unsigned char *)malloc(dst_size);
                if (dstBuf) {
                    tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                              dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags);
                    free(dstBuf);
                }
                free(srcBuf);
            }
            
            tjDestroy(handle);
            break;
        }
    }

    return 0;
}