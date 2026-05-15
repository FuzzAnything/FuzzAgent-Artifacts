// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Split input for different parameters */
    size_t offset = 0;
    
    /* Parameters from first bytes */
    int init_type = Size > 0 ? Data[offset++] % 3 : 0; /* TJINIT_COMPRESS, DECOMPRESS, TRANSFORM */
    int param_type = Size > offset ? Data[offset++] % 10 : 0; /* Various TJPARAM values */
    int param_value = Size > offset ? Data[offset++] : 0;
    int subsamp = Size > offset ? Data[offset++] % 6 : 0; /* TJSAMP values 0-5 */
    int align = Size > offset ? Data[offset++] : 0;
    int pixel_format = Size > offset ? Data[offset++] % 12 : 0; /* TJPF values 0-11 */
    
    /* Ensure align is power of 2 between 1 and 256 */
    if (align <= 0) align = 1;
    else {
        align = 1 << (align % 9); /* 2^0 to 2^8 */
        if (align > 256) align = 256;
    }
    
    /* Get width and height from next bytes if available */
    int width = 16;
    int height = 16;
    if (Size > offset + 1) {
        width = (Data[offset] << 8) | Data[offset + 1];
        offset += 2;
        width = width % 1024 + 1; /* Limit to reasonable size */
    }
    if (Size > offset + 1) {
        height = (Data[offset] << 8) | Data[offset + 1];
        offset += 2;
        height = height % 1024 + 1;
    }
    
    /* Remaining data for buffers */
    size_t remaining = Size - offset;
    const uint8_t *buffer_data = Data + offset;
    
    /* Create dummy file for any file operations */
    write_dummy_file(Data, Size);
    
    /* Initialize TurboJPEG instances */
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (compress_handle) {
        /* Test tj3Set with various parameters */
        tj3Set(compress_handle, param_type, param_value);
        tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
        
        /* Test tjGetErrorCode */
        int error_code = tjGetErrorCode(compress_handle);
        (void)error_code; /* Use variable to avoid unused warning */
        
        /* Test tj3YUVBufSize */
        size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
        
        if (yuv_size > 0 && yuv_size < 100 * 1024 * 1024) { /* Limit to 100MB */
            unsigned char *yuv_buffer = (unsigned char*)malloc(yuv_size);
            if (yuv_buffer) {
                /* Prepare source buffer for encoding */
                int pitch = width * tjPixelSize[pixel_format];
                if (pitch <= 0) pitch = width * 3; /* Default if invalid */
                
                size_t src_size = (size_t)pitch * height;
                unsigned char *src_buffer = NULL;
                
                if (remaining > 0) {
                    src_buffer = (unsigned char*)malloc(src_size);
                    if (src_buffer) {
                        /* Fill with pattern from fuzzing data */
                        for (size_t i = 0; i < src_size; i++) {
                            src_buffer[i] = buffer_data[i % remaining];
                        }
                        
                        /* Test tj3EncodeYUV8 */
                        tj3EncodeYUV8(compress_handle, src_buffer, width, pitch, 
                                     height, pixel_format, yuv_buffer, align);
                        
                        free(src_buffer);
                    }
                }
                
                /* Test decompression with the same buffer */
                if (decompress_handle) {
                    /* Use encoded YUV as fake JPEG data */
                    tj3DecompressToYUV8(decompress_handle, yuv_buffer, 
                                       yuv_size < remaining ? yuv_size : remaining,
                                       yuv_buffer, align);
                }
                
                free(yuv_buffer);
            }
        }
        
        tj3Destroy(compress_handle);
    }
    
    if (decompress_handle) {
        /* Test decompression with fuzzing data directly */
        if (remaining > 0) {
            size_t decomp_yuv_size = tj3YUVBufSize(width, align, height, subsamp);
            if (decomp_yuv_size > 0 && decomp_yuv_size < 10 * 1024 * 1024) {
                unsigned char *decomp_buffer = (unsigned char*)malloc(decomp_yuv_size);
                if (decomp_buffer) {
                    tj3DecompressToYUV8(decompress_handle, buffer_data, 
                                       remaining, decomp_buffer, align);
                    free(decomp_buffer);
                }
            }
        }
        
        tj3Destroy(decompress_handle);
    }
    
    /* Test tj3Init with different init types */
    tjhandle test_handle = tj3Init(init_type);
    if (test_handle) {
        /* Exercise error code function */
        tjGetErrorCode(test_handle);
        
        /* Test setting parameters */
        tj3Set(test_handle, param_type, param_value);
        
        tj3Destroy(test_handle);
    }
    
    return 0;
}