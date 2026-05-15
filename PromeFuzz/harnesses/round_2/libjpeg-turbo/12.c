// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_subsamp_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return TJSAMP_444;
    int subsamp = data[*offset] % 6; // TJSAMP_444, 422, 420, GRAY, 440, 411
    (*offset)++;
    return subsamp;
}

static int get_pixel_format_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return TJPF_RGB;
    int pixel_format = data[*offset] % 12; // 12 pixel formats
    (*offset)++;
    return pixel_format;
}

static int get_align_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 1;
    int align = 1 << (data[*offset] % 4); // 1, 2, 4, 8
    (*offset)++;
    return align;
}

static int get_dimension_from_data(const uint8_t *data, size_t size, size_t *offset, int max_dim) {
    if (*offset + 1 >= size) return 1;
    int dim = (data[*offset] << 8) | data[*offset + 1];
    (*offset) += 2;
    if (dim <= 0) dim = 1;
    if (dim > max_dim) dim = max_dim;
    return dim;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    
    // Initialize handles
    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tj3Destroy(comp_handle);
        if (decomp_handle) tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Get parameters from fuzzer input
    int subsamp = get_subsamp_from_data(Data, Size, &offset);
    int pixel_format = get_pixel_format_from_data(Data, Size, &offset);
    int align = get_align_from_data(Data, Size, &offset);
    int width = get_dimension_from_data(Data, Size, &offset, 4096);
    int height = get_dimension_from_data(Data, Size, &offset, 4096);
    
    // Set subsampling parameter
    tj3Set(comp_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(decomp_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Calculate buffer sizes
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    int bytes_per_pixel = (pixel_format == TJPF_GRAY) ? 1 : 
                         (pixel_format == TJPF_CMYK) ? 4 : 
                         (pixel_format <= TJPF_XBGR) ? 4 : 3;
    size_t rgb_buf_size = width * height * bytes_per_pixel;
    
    if (yuv_buf_size == 0 || jpeg_buf_size == 0 || rgb_buf_size == 0) {
        tj3Destroy(comp_handle);
        tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Allocate buffers
    unsigned char *yuv_buf = (unsigned char*)malloc(yuv_buf_size);
    unsigned char *rgb_buf = (unsigned char*)malloc(rgb_buf_size);
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = jpeg_buf_size;
    
    if (!yuv_buf || !rgb_buf) {
        free(yuv_buf);
        free(rgb_buf);
        tj3Destroy(comp_handle);
        tj3Destroy(decomp_handle);
        return 0;
    }
    
    // Fill buffers with fuzzer data
    size_t data_to_copy = Size - offset;
    if (data_to_copy > 0) {
        size_t yuv_copy = data_to_copy < yuv_buf_size ? data_to_copy : yuv_buf_size;
        memcpy(yuv_buf, Data + offset, yuv_copy);
        
        size_t rgb_copy = data_to_copy < rgb_buf_size ? data_to_copy : rgb_buf_size;
        memcpy(rgb_buf, Data + offset, rgb_copy);
    }
    
    // Test tj3YUVBufSize
    size_t calculated_size = tj3YUVBufSize(width, align, height, subsamp);
    
    // Test tj3JPEGBufSize
    size_t max_jpeg_size = tj3JPEGBufSize(width, height, subsamp);
    
    // Test tj3EncodeYUV8
    unsigned char *encoded_yuv = (unsigned char*)malloc(yuv_buf_size);
    if (encoded_yuv) {
        tj3EncodeYUV8(comp_handle, rgb_buf, width, 0, height, pixel_format, encoded_yuv, align);
        free(encoded_yuv);
    }
    
    // Test tj3DecodeYUV8
    tj3DecodeYUV8(decomp_handle, yuv_buf, align, rgb_buf, width, 0, height, pixel_format);
    
    // Test tj3CompressFromYUV8
    jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (jpeg_buf) {
        tj3CompressFromYUV8(comp_handle, yuv_buf, width, align, height, &jpeg_buf, &jpeg_size);
        tj3Free(jpeg_buf);
    }
    
    // Test tjEncodeYUV2 (legacy function)
    unsigned char *yuv_buf2 = (unsigned char*)malloc(yuv_buf_size);
    if (yuv_buf2) {
        tjEncodeYUV2(comp_handle, rgb_buf, width, 0, height, pixel_format, yuv_buf2, subsamp, 0);
        free(yuv_buf2);
    }
    
    // Cleanup
    free(yuv_buf);
    free(rgb_buf);
    tj3Destroy(comp_handle);
    tj3Destroy(decomp_handle);
    
    return 0;
}