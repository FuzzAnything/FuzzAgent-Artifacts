// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_DIM 4096
#define MAX_PLANES 3

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static int read_int(const uint8_t **data, size_t *size, int default_val) {
    if (*size >= 4) {
        int val = *(const int*)(*data);
        *data += 4;
        *size -= 4;
        return val;
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to choose function
    int func_choice = Data[0] % 6;
    Data++; Size--;
    
    // Read basic parameters from fuzzer input
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    int width = read_int(&ptr, &remaining, 64);
    int height = read_int(&ptr, &remaining, 64);
    int pitch = read_int(&ptr, &remaining, 0);
    int pixelFormat = read_int(&ptr, &remaining, TJPF_RGB);
    int subsamp = read_int(&ptr, &remaining, TJSAMP_420);
    int align = read_int(&ptr, &remaining, 1);
    int flags = read_int(&ptr, &remaining, 0);
    int param = read_int(&ptr, &remaining, TJPARAM_QUALITY);
    
    // Clamp dimensions to avoid excessive memory usage
    if (width <= 0 || width > MAX_DIM) width = 64;
    if (height <= 0 || height > MAX_DIM) height = 64;
    if (pitch < 0) pitch = 0;
    if (align <= 0) align = 1;
    
    // Ensure align is power of 2
    align = 1 << (align % 5);
    
    switch (func_choice) {
        case 0: { // tjDecodeYUVPlanes
            tjhandle handle = tjInitDecompress();
            if (!handle) break;
            
            // Calculate plane sizes based on subsampling
            int y_width = width;
            int y_height = height;
            int uv_width, uv_height;
            
            switch (subsamp) {
                case TJSAMP_420:
                    uv_width = (width + 1) / 2;
                    uv_height = (height + 1) / 2;
                    break;
                case TJSAMP_422:
                    uv_width = (width + 1) / 2;
                    uv_height = height;
                    break;
                case TJSAMP_444:
                    uv_width = width;
                    uv_height = height;
                    break;
                case TJSAMP_GRAY:
                    uv_width = 0;
                    uv_height = 0;
                    break;
                case TJSAMP_440:
                    uv_width = width;
                    uv_height = (height + 1) / 2;
                    break;
                case TJSAMP_411:
                    uv_width = (width + 3) / 4;
                    uv_height = height;
                    break;
                default:
                    uv_width = (width + 1) / 2;
                    uv_height = (height + 1) / 2;
                    break;
            }
            
            // Allocate and fill planes
            const unsigned char *srcPlanes[MAX_PLANES] = {NULL, NULL, NULL};
            int strides[MAX_PLANES] = {0, 0, 0};
            unsigned char *planes[MAX_PLANES] = {NULL, NULL, NULL};
            
            size_t y_size = y_width * y_height;
            size_t uv_size = uv_width * uv_height;
            
            if (remaining >= y_size) {
                planes[0] = (unsigned char*)malloc(y_size);
                if (planes[0]) {
                    memcpy(planes[0], ptr, y_size);
                    srcPlanes[0] = planes[0];
                    ptr += y_size;
                    remaining -= y_size;
                }
            }
            
            if (uv_size > 0 && remaining >= uv_size) {
                planes[1] = (unsigned char*)malloc(uv_size);
                if (planes[1]) {
                    memcpy(planes[1], ptr, uv_size);
                    srcPlanes[1] = planes[1];
                    ptr += uv_size;
                    remaining -= uv_size;
                }
            }
            
            if (uv_size > 0 && remaining >= uv_size) {
                planes[2] = (unsigned char*)malloc(uv_size);
                if (planes[2]) {
                    memcpy(planes[2], ptr, uv_size);
                    srcPlanes[2] = planes[2];
                }
            }
            
            // Allocate destination buffer
            int pixel_size = tjPixelSize[pixelFormat];
            int dst_pitch = pitch ? pitch : width * pixel_size;
            unsigned char *dstBuf = (unsigned char*)malloc(dst_pitch * height);
            
            if (dstBuf) {
                tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, 
                                 dstBuf, width, dst_pitch, height, pixelFormat, flags);
                free(dstBuf);
            }
            
            // Cleanup
            for (int i = 0; i < MAX_PLANES; i++) {
                free(planes[i]);
            }
            tjDestroy(handle);
            break;
        }
        
        case 1: { // tj3LoadImage16
            tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
            if (!handle) break;
            
            // Write remaining data to file
            write_dummy_file(ptr, remaining);
            
            int w, h, pf;
            unsigned short *img = tj3LoadImage16(handle, "./dummy_file", &w, align, &h, &pf);
            if (img) {
                tj3Free(img);
            }
            
            tj3Destroy(handle);
            break;
        }
        
        case 2: { // tjSaveImage
            // Create a dummy buffer
            int pixel_size = tjPixelSize[pixelFormat];
            int row_pitch = pitch ? pitch : width * pixel_size;
            size_t buf_size = row_pitch * height;
            
            unsigned char *buffer = (unsigned char*)malloc(buf_size);
            if (buffer) {
                // Fill with fuzzer data if available
                size_t to_copy = remaining < buf_size ? remaining : buf_size;
                if (to_copy > 0) {
                    memcpy(buffer, ptr, to_copy);
                }
                
                tjSaveImage("./dummy_file", buffer, width, row_pitch, height, pixelFormat, flags);
                free(buffer);
            }
            break;
        }
        
        case 3: { // tjLoadImage
            // Write remaining data to file
            write_dummy_file(ptr, remaining);
            
            int w, h, pf;
            unsigned char *img = tjLoadImage("./dummy_file", &w, align, &h, &pf, flags);
            if (img) {
                free(img);
            }
            break;
        }
        
        case 4: { // tj3Get
            tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
            if (!handle) break;
            
            int value = tj3Get(handle, param);
            (void)value; // Use value to avoid unused warning
            
            tj3Destroy(handle);
            break;
        }
        
        case 5: { // tj3Compress16
            tjhandle handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            // Create source buffer
            int pixel_size = tjPixelSize[pixelFormat];
            int row_pitch = pitch ? pitch : width * pixel_size;
            size_t buf_size = row_pitch * height * sizeof(unsigned short);
            
            unsigned short *srcBuf = (unsigned short*)malloc(buf_size);
            if (srcBuf) {
                // Fill with fuzzer data if available
                size_t to_copy = remaining < buf_size ? remaining : buf_size;
                if (to_copy > 0) {
                    memcpy(srcBuf, ptr, to_copy);
                }
                
                unsigned char *jpegBuf = NULL;
                size_t jpegSize = 0;
                tj3Compress16(handle, srcBuf, width, row_pitch, height, pixelFormat, &jpegBuf, &jpegSize);
                
                if (jpegBuf) {
                    tj3Free(jpegBuf);
                }
                free(srcBuf);
            }
            
            tj3Destroy(handle);
            break;
        }
    }
    
    return 0;
}