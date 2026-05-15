// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Write dummy file for functions that need it
    write_dummy_file(fuzz_data, fuzz_size < 1024 ? fuzz_size : 1024);
    
    switch (selector) {
        case 0: {  // Test tj3Alloc
            size_t alloc_size = fuzz_size % 65536;  // Limit to reasonable size
            if (alloc_size == 0) alloc_size = 1;
            void *buffer = tj3Alloc(alloc_size);
            if (buffer) {
                // Try to use the buffer
                memset(buffer, 0, alloc_size);
                tj3Free(buffer);
            }
            break;
        }
        
        case 1: {  // Test tj3SaveImage16
            if (fuzz_size < 16) break;
            
            tjhandle handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            // Extract parameters from fuzz data
            int width = (fuzz_data[0] << 8) | fuzz_data[1];
            int height = (fuzz_data[2] << 8) | fuzz_data[3];
            int pitch = (fuzz_data[4] << 8) | fuzz_data[5];
            int pixelFormat = fuzz_data[6] % 13;  // TJPF values 0-12
            
            // Limit dimensions to prevent excessive memory usage
            if (width > 1024) width = 1024;
            if (height > 1024) height = 1024;
            if (width <= 0) width = 1;
            if (height <= 0) height = 1;
            
            // Calculate buffer size needed
            int pixelSize = 0;
            switch (pixelFormat) {
                case TJPF_RGB: case TJPF_BGR: pixelSize = 3; break;
                case TJPF_RGBX: case TJPF_BGRX: case TJPF_XBGR: case TJPF_XRGB: pixelSize = 4; break;
                case TJPF_GRAY: pixelSize = 1; break;
                case TJPF_RGBA: case TJPF_BGRA: case TJPF_ABGR: case TJPF_ARGB: pixelSize = 4; break;
                case TJPF_CMYK: pixelSize = 4; break;
                default: pixelSize = 3; break;
            }
            
            if (pitch == 0) pitch = width * pixelSize;
            
            size_t buffer_size = pitch * height * sizeof(unsigned short);
            if (buffer_size > 1024 * 1024 * 16) {  // Limit to 16MB
                tj3Destroy(handle);
                break;
            }
            
            unsigned short *buffer = (unsigned short *)tj3Alloc(buffer_size);
            if (!buffer) {
                tj3Destroy(handle);
                break;
            }
            
            // Fill buffer with fuzz data
            size_t copy_size = fuzz_size - 7 < buffer_size ? fuzz_size - 7 : buffer_size;
            if (copy_size > 0) {
                memcpy(buffer, fuzz_data + 7, copy_size);
            }
            
            tj3SaveImage16(handle, "./dummy_file", buffer, width, pitch, height, pixelFormat);
            
            tj3Free(buffer);
            tj3Destroy(handle);
            break;
        }
        
        case 2: {  // Test tj3Compress16
            if (fuzz_size < 16) break;
            
            tjhandle handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            // Extract parameters from fuzz data
            int width = (fuzz_data[0] << 8) | fuzz_data[1];
            int height = (fuzz_data[2] << 8) | fuzz_data[3];
            int pitch = (fuzz_data[4] << 8) | fuzz_data[5];
            int pixelFormat = fuzz_data[6] % 13;
            
            // Limit dimensions
            if (width > 1024) width = 1024;
            if (height > 1024) height = 1024;
            if (width <= 0) width = 1;
            if (height <= 0) height = 1;
            
            int pixelSize = 0;
            switch (pixelFormat) {
                case TJPF_RGB: case TJPF_BGR: pixelSize = 3; break;
                case TJPF_RGBX: case TJPF_BGRX: case TJPF_XBGR: case TJPF_XRGB: pixelSize = 4; break;
                case TJPF_GRAY: pixelSize = 1; break;
                case TJPF_RGBA: case TJPF_BGRA: case TJPF_ABGR: case TJPF_ARGB: pixelSize = 4; break;
                case TJPF_CMYK: pixelSize = 4; break;
                default: pixelSize = 3; break;
            }
            
            if (pitch == 0) pitch = width * pixelSize;
            
            size_t src_size = pitch * height * sizeof(unsigned short);
            if (src_size > 1024 * 1024 * 16) {
                tj3Destroy(handle);
                break;
            }
            
            unsigned short *srcBuf = (unsigned short *)tj3Alloc(src_size);
            if (!srcBuf) {
                tj3Destroy(handle);
                break;
            }
            
            // Fill source buffer
            size_t copy_size = fuzz_size - 7 < src_size ? fuzz_size - 7 : src_size;
            if (copy_size > 0) {
                memcpy(srcBuf, fuzz_data + 7, copy_size);
            }
            
            unsigned char *jpegBuf = NULL;
            size_t jpegSize = 0;
            
            tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
            
            if (jpegBuf) tj3Free(jpegBuf);
            tj3Free(srcBuf);
            tj3Destroy(handle);
            break;
        }
        
        case 3: {  // Test tj3Decompress16
            if (fuzz_size < 16) break;
            
            tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
            if (!handle) break;
            
            // Use first part as JPEG data
            size_t jpegSize = fuzz_size < 65536 ? fuzz_size : 65536;
            const unsigned char *jpegBuf = fuzz_data;
            
            // Extract parameters for destination
            int pitch = (fuzz_data[jpegSize % fuzz_size] << 8) | 
                       (fuzz_data[(jpegSize + 1) % fuzz_size]);
            int pixelFormat = fuzz_data[(jpegSize + 2) % fuzz_size] % 13;
            
            // Create a reasonable destination buffer
            int width = 256;
            int height = 256;
            
            int pixelSize = 0;
            switch (pixelFormat) {
                case TJPF_RGB: case TJPF_BGR: pixelSize = 3; break;
                case TJPF_RGBX: case TJPF_BGRX: case TJPF_XBGR: case TJPF_XRGB: pixelSize = 4; break;
                case TJPF_GRAY: pixelSize = 1; break;
                case TJPF_RGBA: case TJPF_BGRA: case TJPF_ABGR: case TJPF_ARGB: pixelSize = 4; break;
                case TJPF_CMYK: pixelSize = 4; break;
                default: pixelSize = 3; break;
            }
            
            if (pitch == 0) pitch = width * pixelSize;
            
            size_t dst_size = pitch * height * sizeof(unsigned short);
            if (dst_size > 1024 * 1024 * 16) {
                tj3Destroy(handle);
                break;
            }
            
            unsigned short *dstBuf = (unsigned short *)tj3Alloc(dst_size);
            if (!dstBuf) {
                tj3Destroy(handle);
                break;
            }
            
            tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
            
            tj3Free(dstBuf);
            tj3Destroy(handle);
            break;
        }
        
        case 4: {  // Test tj3LoadImage16
            tjhandle handle = tj3Init(TJINIT_COMPRESS);
            if (!handle) break;
            
            int width, height;
            int pixelFormat = TJPF_UNKNOWN;
            int align = 1;  // No padding
            
            unsigned short *buffer = tj3LoadImage16(handle, "./dummy_file", 
                                                   &width, align, &height, 
                                                   &pixelFormat);
            
            if (buffer) {
                tj3Free(buffer);
            }
            
            tj3Destroy(handle);
            break;
        }
        
        case 5: {  // Test tj3JPEGBufSize
            if (fuzz_size < 6) break;
            
            int width = (fuzz_data[0] << 8) | fuzz_data[1];
            int height = (fuzz_data[2] << 8) | fuzz_data[3];
            int jpegSubsamp = fuzz_data[4] % 6;  // TJSAMP values 0-5
            
            // Limit dimensions
            if (width > 16384) width = 16384;
            if (height > 16384) height = 16384;
            if (width < 0) width = 0;
            if (height < 0) height = 0;
            
            size_t buf_size = tj3JPEGBufSize(width, height, jpegSubsamp);
            (void)buf_size;  // Use result to avoid unused variable warning
            break;
        }
    }
    
    remove_dummy_file();
    return 0;
}