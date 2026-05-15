// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int width = 0, height = 0, pixelFormat = 0;
    unsigned short *srcBuf16 = NULL;
    unsigned char *srcBuf8 = NULL;
    int ret = -1;
    
    write_dummy_file(Data, Size);
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    switch (selector % 6) {
        case 0: {
            handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            width = 16;
            height = 16;
            pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            size_t buf_size = pitch * height * sizeof(unsigned short);
            
            srcBuf16 = (unsigned short *)malloc(buf_size);
            if (!srcBuf16) break;
            
            size_t copy_size = buf_size;
            if (Size - data_offset < copy_size) {
                copy_size = Size - data_offset;
            }
            if (copy_size > 0) {
                memcpy(srcBuf16, Data + data_offset, copy_size);
            }
            
            jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
            jpegBuf = tj3Alloc(jpegSize);
            
            ret = tj3Compress16(handle, srcBuf16, width, pitch, height,
                               pixelFormat, &jpegBuf, &jpegSize);
            
            if (jpegBuf) tj3Free(jpegBuf);
            free(srcBuf16);
            tj3Destroy(handle);
            break;
        }
        
        case 1: {
            handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            pixelFormat = TJPF_UNKNOWN;
            srcBuf16 = tj3LoadImage16(handle, "./dummy_file", &width, 1,
                                     &height, &pixelFormat);
            
            if (srcBuf16) tj3Free(srcBuf16);
            tj3Destroy(handle);
            break;
        }
        
        case 2: {
            handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            int param = TJPARAM_QUALITY;
            if (Size - data_offset >= 1) {
                param = Data[data_offset] % 32;
            }
            
            ret = tj3Get(handle, param);
            tj3Destroy(handle);
            break;
        }
        
        case 3: {
            pixelFormat = TJPF_UNKNOWN;
            srcBuf8 = tjLoadImage("./dummy_file", &width, 1, &height,
                                 &pixelFormat, 0);
            
            if (srcBuf8) free(srcBuf8);
            break;
        }
        
        case 4: {
            width = 16;
            height = 16;
            pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            size_t buf_size = pitch * height;
            
            srcBuf8 = (unsigned char *)malloc(buf_size);
            if (!srcBuf8) break;
            
            size_t copy_size = buf_size;
            if (Size - data_offset < copy_size) {
                copy_size = Size - data_offset;
            }
            if (copy_size > 0) {
                memcpy(srcBuf8, Data + data_offset, copy_size);
            }
            
            ret = tjSaveImage("./dummy_file", srcBuf8, width, pitch, height,
                             pixelFormat, 0);
            
            free(srcBuf8);
            break;
        }
        
        case 5: {
            handle = tj3Init(TJINIT_DECOMPRESS);
            if (!handle) break;
            
            width = 16;
            height = 16;
            pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            size_t dst_size = pitch * height;
            
            unsigned char *dstBuf = (unsigned char *)malloc(dst_size);
            if (!dstBuf) {
                tj3Destroy(handle);
                break;
            }
            
            const unsigned char *srcPlanes[3];
            int strides[3] = {0};
            int subsamp = TJSAMP_420;
            
            size_t y_size = width * height;
            size_t uv_size = ((width + 1) / 2) * ((height + 1) / 2);
            
            unsigned char *y_plane = (unsigned char *)malloc(y_size);
            unsigned char *u_plane = (unsigned char *)malloc(uv_size);
            unsigned char *v_plane = (unsigned char *)malloc(uv_size);
            
            if (y_plane && u_plane && v_plane) {
                srcPlanes[0] = y_plane;
                srcPlanes[1] = u_plane;
                srcPlanes[2] = v_plane;
                
                size_t total_plane_size = y_size + 2 * uv_size;
                size_t copy_size = total_plane_size;
                if (Size - data_offset < copy_size) {
                    copy_size = Size - data_offset;
                }
                
                if (copy_size > 0) {
                    size_t offset = 0;
                    size_t y_copy = (copy_size > y_size) ? y_size : copy_size;
                    if (y_copy > 0) memcpy(y_plane, Data + data_offset, y_copy);
                    offset += y_copy;
                    
                    if (offset < copy_size) {
                        size_t u_copy = (copy_size - offset > uv_size) ? uv_size : copy_size - offset;
                        memcpy(u_plane, Data + data_offset + offset, u_copy);
                        offset += u_copy;
                    }
                    
                    if (offset < copy_size) {
                        size_t v_copy = (copy_size - offset > uv_size) ? uv_size : copy_size - offset;
                        memcpy(v_plane, Data + data_offset + offset, v_copy);
                    }
                }
                
                ret = tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                       dstBuf, width, pitch, height,
                                       pixelFormat, 0);
            }
            
            if (y_plane) free(y_plane);
            if (u_plane) free(u_plane);
            if (v_plane) free(v_plane);
            if (dstBuf) free(dstBuf);
            tj3Destroy(handle);
            break;
        }
    }
    
    cleanup_dummy_file();
    return 0;
}