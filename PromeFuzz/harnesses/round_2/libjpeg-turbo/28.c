// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    // Parse parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pitch = (Data[4] << 8) | Data[5];
    int pixelFormat = Data[6] % (TJPF_CMYK + 1);
    int flags = Data[7];
    int align = 1 << (Data[8] % 5); // 1,2,4,8,16
    int param = Data[9] % 32; // TJPARAM values
    int subsamp = Data[10] % (TJ_NUMSAMP + 1);
    
    // Limit dimensions to avoid excessive memory usage
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (pitch > 4096) pitch = 4096;
    if (pitch == 0) pitch = width * tjPixelSize[pixelFormat];
    
    // Create dummy file with remaining data
    size_t file_data_size = Size - 32;
    if (file_data_size > 0) {
        write_dummy_file(Data + 32, file_data_size);
    } else {
        write_dummy_file(Data, Size);
    }

    // Test tjLoadImage
    int loaded_width = 0, loaded_height = 0, loaded_pixelFormat = 0;
    unsigned char *loaded_buffer = tjLoadImage("./dummy_file", &loaded_width, 
                                               align, &loaded_height, 
                                               &loaded_pixelFormat, flags);
    if (loaded_buffer) {
        free(loaded_buffer);
    }

    // Test tj3LoadImage16
    tjhandle handle1 = tj3Init(TJINIT_COMPRESS);
    if (handle1) {
        int load16_width = 0, load16_height = 0, load16_pixelFormat = TJPF_UNKNOWN;
        unsigned short *load16_buffer = tj3LoadImage16(handle1, "./dummy_file", 
                                                       &load16_width, align, 
                                                       &load16_height, &load16_pixelFormat);
        if (load16_buffer) {
            tj3Free(load16_buffer);
        }
        
        // Test tj3Compress16 with the loaded buffer or synthetic data
        if (load16_buffer) {
            unsigned char *jpegBuf = NULL;
            size_t jpegSize = 0;
            tj3Compress16(handle1, load16_buffer, load16_width, 
                         load16_width * tjPixelSize[load16_pixelFormat], 
                         load16_height, load16_pixelFormat, &jpegBuf, &jpegSize);
            if (jpegBuf) {
                tj3Free(jpegBuf);
            }
        } else {
            // Create synthetic 16-bit data
            size_t samples = width * height * tjPixelSize[pixelFormat];
            if (samples > 0 && samples < 1000000) {
                unsigned short *srcBuf = (unsigned short*)malloc(samples * sizeof(unsigned short));
                if (srcBuf) {
                    for (size_t i = 0; i < samples && i < 1000; i++) {
                        srcBuf[i] = (i < Size - 32) ? Data[32 + i] << 8 : 0x8000;
                    }
                    unsigned char *jpegBuf = NULL;
                    size_t jpegSize = 0;
                    tj3Compress16(handle1, srcBuf, width, pitch, height, 
                                 pixelFormat, &jpegBuf, &jpegSize);
                    if (jpegBuf) {
                        tj3Free(jpegBuf);
                    }
                    free(srcBuf);
                }
            }
        }
        
        // Test tj3Get
        tj3Get(handle1, param);
        tj3Destroy(handle1);
    }

    // Test tjSaveImage with synthetic buffer
    if (width > 0 && height > 0) {
        size_t buf_size = pitch * height;
        if (buf_size > 0 && buf_size < 1000000) {
            unsigned char *buffer = (unsigned char*)malloc(buf_size);
            if (buffer) {
                size_t copy_size = (Size > 32) ? Size - 32 : Size;
                if (copy_size > buf_size) copy_size = buf_size;
                if (copy_size > 0) {
                    memcpy(buffer, Data + 32, copy_size);
                }
                tjSaveImage("./dummy_file", buffer, width, pitch, 
                           height, pixelFormat, flags);
                free(buffer);
            }
        }
    }

    // Test tjDecodeYUVPlanes
    tjhandle handle2 = tj3Init(TJINIT_DECOMPRESS);
    if (handle2) {
        int yuv_width = width;
        int yuv_height = height;
        if (yuv_width > 0 && yuv_height > 0) {
            // Calculate plane sizes based on subsampling
            int y_plane_size = yuv_width * yuv_height;
            int uv_width = yuv_width;
            int uv_height = yuv_height;
            
            if (subsamp == TJSAMP_420) {
                uv_width = (yuv_width + 1) / 2;
                uv_height = (yuv_height + 1) / 2;
            } else if (subsamp == TJSAMP_422) {
                uv_width = (yuv_width + 1) / 2;
            } else if (subsamp == TJSAMP_440) {
                uv_height = (yuv_height + 1) / 2;
            }
            
            int uv_plane_size = uv_width * uv_height;
            int total_yuv_size = y_plane_size + 2 * uv_plane_size;
            
            if (total_yuv_size > 0 && total_yuv_size < 1000000) {
                unsigned char *yuv_data = (unsigned char*)malloc(total_yuv_size);
                if (yuv_data) {
                    // Fill with fuzzer data, but ensure we don't read beyond Data buffer
                    size_t copy_size = (Size > 32) ? Size - 32 : 0;
                    if (copy_size > total_yuv_size) copy_size = total_yuv_size;
                    if (copy_size > 0) {
                        memcpy(yuv_data, Data + 32, copy_size);
                    }
                    
                    const unsigned char *planes[3] = {
                        yuv_data,
                        yuv_data + y_plane_size,
                        yuv_data + y_plane_size + uv_plane_size
                    };
                    
                    int strides[3] = {yuv_width, uv_width, uv_width};
                    
                    size_t dst_size = yuv_width * yuv_height * tjPixelSize[pixelFormat];
                    if (dst_size > 0 && dst_size < 1000000) {
                        unsigned char *dst_buf = (unsigned char*)malloc(dst_size);
                        if (dst_buf) {
                            tjDecodeYUVPlanes(handle2, planes, strides, subsamp,
                                             dst_buf, yuv_width, yuv_width * tjPixelSize[pixelFormat],
                                             yuv_height, pixelFormat, flags);
                            free(dst_buf);
                        }
                    }
                    free(yuv_data);
                }
            }
        }
        tj3Destroy(handle2);
    }

    cleanup_dummy_file();
    return 0;
}