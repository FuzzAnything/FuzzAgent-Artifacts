// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static int get_uv_width(int width, int subsamp) {
    switch (subsamp) {
        case TJSAMP_444: return width;
        case TJSAMP_422: return (width + 1) / 2;
        case TJSAMP_420: return (width + 1) / 2;
        case TJSAMP_GRAY: return 0;
        case TJSAMP_440: return width;
        case TJSAMP_411: return (width + 3) / 4;
        default: return (width + 1) / 2;
    }
}

static int get_uv_height(int height, int subsamp) {
    switch (subsamp) {
        case TJSAMP_444: return height;
        case TJSAMP_422: return height;
        case TJSAMP_420: return (height + 1) / 2;
        case TJSAMP_GRAY: return 0;
        case TJSAMP_440: return (height + 1) / 2;
        case TJSAMP_411: return height;
        default: return (height + 1) / 2;
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        return 0;
    }

    // Use first byte to determine configuration variations
    uint8_t config = Data[0];
    int width = 64;
    int height = 64;
    int pitch = width * 3; // For RGB
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    int flags = 0;
    
    // Vary parameters based on input
    if (config & 0x01) width = 32;
    if (config & 0x02) height = 32;
    if (config & 0x04) pixelFormat = TJPF_BGR;
    if (config & 0x08) subsamp = TJSAMP_444;
    if (config & 0x10) flags = TJFLAG_FASTDCT;
    if (config & 0x20) flags = TJFLAG_ACCURATEDCT;

    // Allocate buffers using tjAlloc
    size_t rgb_size = width * height * tjPixelSize[pixelFormat];
    unsigned char *rgb_buf = tjAlloc(rgb_size);
    if (!rgb_buf) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        return 0;
    }

    // Initialize with fuzzer data (wrap around if needed)
    for (size_t i = 0; i < rgb_size; i++) {
        rgb_buf[i] = Data[i % Size];
    }

    // Test tjEncodeYUVPlanes
    unsigned char *yuv_planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    // Calculate plane sizes and allocate
    for (int i = 0; i < 3; i++) {
        int plane_width = (i == 0) ? width : get_uv_width(width, subsamp);
        int plane_height = (i == 0) ? height : get_uv_height(height, subsamp);
        strides[i] = plane_width;
        yuv_planes[i] = tjAlloc(plane_width * plane_height);
        if (yuv_planes[i]) {
            memset(yuv_planes[i], 0x80, plane_width * plane_height);
        }
    }

    if (yuv_planes[0] && yuv_planes[1] && yuv_planes[2]) {
        tjEncodeYUVPlanes(compress_handle, rgb_buf, width, pitch, height,
                         pixelFormat, yuv_planes, strides, subsamp, flags);
    }

    // Test tjDecodeYUVPlanes
    const unsigned char *src_planes[3] = {yuv_planes[0], yuv_planes[1], yuv_planes[2]};
    unsigned char *decoded_buf = tjAlloc(rgb_size);
    if (decoded_buf) {
        tjDecodeYUVPlanes(decompress_handle, src_planes, strides, subsamp,
                         decoded_buf, width, pitch, height, pixelFormat, flags);
        tjFree(decoded_buf);
    }

    // Test tjDecodeYUV
    // Create a contiguous YUV buffer
    size_t yuv_size = tjBufSizeYUV2(width, 4, height, subsamp);
    unsigned char *contig_yuv = tjAlloc(yuv_size);
    if (contig_yuv) {
        // Fill with some data
        for (size_t i = 0; i < yuv_size && i < Size; i++) {
            contig_yuv[i] = Data[i];
        }
        
        unsigned char *decoded_buf2 = tjAlloc(rgb_size);
        if (decoded_buf2) {
            int align = 4;
            tjDecodeYUV(decompress_handle, contig_yuv, align, subsamp,
                       decoded_buf2, width, pitch, height, pixelFormat, flags);
            tjFree(decoded_buf2);
        }
        tjFree(contig_yuv);
    }

    // Test tjDecompressToYUV2 with dummy JPEG file
    write_dummy_file(Data, Size);
    FILE *f = fopen("./dummy_file", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long jpeg_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if (jpeg_size > 0) {
            unsigned char *jpeg_buf = tjAlloc(jpeg_size);
            if (jpeg_buf) {
                fread(jpeg_buf, 1, jpeg_size, f);
                
                size_t yuv_buf_size = tjBufSizeYUV2(width, 4, height, subsamp);
                unsigned char *yuv_buf = tjAlloc(yuv_buf_size);
                if (yuv_buf) {
                    tjDecompressToYUV2(decompress_handle, jpeg_buf, jpeg_size,
                                      yuv_buf, width, 4, height, flags);
                    tjFree(yuv_buf);
                }
                tjFree(jpeg_buf);
            }
        }
        fclose(f);
    }

    // Clean up
    for (int i = 0; i < 3; i++) {
        if (yuv_planes[i]) {
            tjFree(yuv_planes[i]);
        }
    }
    
    if (rgb_buf) {
        tjFree(rgb_buf);
    }
    
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    
    return 0;
}