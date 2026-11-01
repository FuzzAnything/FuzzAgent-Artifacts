// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static void cleanup_buffers(unsigned char *buf, unsigned char **planes, int num_planes) {
    if (buf) free(buf);
    if (planes) {
        for (int i = 0; i < num_planes; i++) {
            if (planes[i]) free(planes[i]);
        }
        free(planes);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Determine operation mode based on first byte
    int mode = Data[0] % 4;
    const uint8_t *jpeg_data = Data + 1;
    size_t jpeg_size = Size - 1;

    // Ensure minimum size for a valid-ish JPEG header to avoid immediate crashes on parsing
    if (jpeg_size < 10) return 0;

    // Prepare buffers
    unsigned char *yuv_buf = NULL;
    unsigned char **yuv_planes = NULL;
    unsigned char *jpeg_out_buf = NULL;
    size_t jpeg_out_size = 0;
    int width = 0, height = 0;
    int strides[3] = {0, 0, 0};
    int align = 1 << (Data[1] % 6); // Power of 2 between 1 and 32

    // Estimate dimensions for buffer allocation (clamped to reasonable limits)
    if (jpeg_size > 0) {
        width = (jpeg_size % 1000) + 16;
        height = (jpeg_size % 1000) + 16;
    }

    // Allocate YUV buffers if needed
    if (mode == 1 || mode == 2) {
        // For tj3DecompressToYUV8
        size_t buf_size = tj3YUVBufSize(width, height, TJCS_YCbCr, align);
        if (buf_size > 0 && buf_size < 100 * 1024 * 1024) {
            yuv_buf = (unsigned char *)malloc(buf_size);
        }
    }

    if (mode == 3 || mode == 4) {
        // For tj3DecompressToYUVPlanes8 or tjDecompressToYUVPlanes
        yuv_planes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
        if (yuv_planes) {
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            
            // Allocate planes
            size_t plane_sizes[3];
            int subsamp_val = 1; // TJPARAM_SUBSAMP_420

            // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
            plane_sizes[0] = tj3YUVPlaneSize(0, width, strides[0], height, subsamp_val);
            plane_sizes[1] = tj3YUVPlaneSize(1, width / 2, strides[1], height / 2, subsamp_val);
            plane_sizes[2] = tj3YUVPlaneSize(2, width / 2, strides[2], height / 2, subsamp_val);

            for (int i = 0; i < 3; i++) {
                if (plane_sizes[i] > 0 && plane_sizes[i] < 100 * 1024 * 1024) {
                    yuv_planes[i] = (unsigned char *)malloc(plane_sizes[i]);
                } else {
                    yuv_planes[i] = NULL;
                }
            }
        }
    }

    if (mode == 0) {
        // Compression: tj3Init(TJINIT_COMPRESS) -> tj3CompressFromYUVPlanes8
        tjhandle handle = tj3Init(TJINIT_COMPRESS);
        if (!handle) {
            cleanup_buffers(yuv_buf, yuv_planes, 3);
            return 0;
        }

        // Set some parameters
        tj3Set(handle, TJPARAM_QUALITY, Data[1] % 101);
        
        // Prepare source planes for compression
        unsigned char *src_planes[3];
        int src_strides[3];
        src_strides[0] = width;
        src_strides[1] = width / 2;
        src_strides[2] = width / 2;

        // Allocate source planes
        size_t p_sizes[3];
        int subsamp_val = 1; // TJPARAM_SUBSAMP_420

        // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
        p_sizes[0] = tj3YUVPlaneSize(0, width, src_strides[0], height, subsamp_val);
        p_sizes[1] = tj3YUVPlaneSize(1, width / 2, src_strides[1], height / 2, subsamp_val);
        p_sizes[2] = tj3YUVPlaneSize(2, width / 2, src_strides[2], height / 2, subsamp_val);

        for (int i = 0; i < 3; i++) {
            if (p_sizes[i] > 0 && p_sizes[i] < 100 * 1024 * 1024) {
                src_planes[i] = (unsigned char *)malloc(p_sizes[i]);
                if (src_planes[i]) {
                    memset(src_planes[i], 0, p_sizes[i]);
                }
            } else {
                src_planes[i] = NULL;
            }
        }

        if (src_planes[0]) {
            tj3CompressFromYUVPlanes8(handle, (const unsigned char *const *)src_planes, 
                                      width, src_strides, height, &jpeg_out_buf, &jpeg_out_size);
        }

        // Cleanup compression specific
        for (int i = 0; i < 3; i++) if (src_planes[i]) free(src_planes[i]);
        if (jpeg_out_buf) free(jpeg_out_buf);
        cleanup_handle(handle);
    } 
    else if (mode == 1) {
        // Decompression: tj3Init(TJINIT_DECOMPRESS) -> tj3DecompressToYUV8
        tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
        if (!handle) {
            cleanup_buffers(yuv_buf, yuv_planes, 3);
            return 0;
        }

        if (yuv_buf) {
            tj3DecompressToYUV8(handle, jpeg_data, jpeg_size, yuv_buf, align);
        }
        cleanup_handle(handle);
    }
    else if (mode == 2) {
        // Decompression: tj3Init(TJINIT_DECOMPRESS) -> tj3DecompressToYUVPlanes8
        tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
        if (!handle) {
            cleanup_buffers(yuv_buf, yuv_planes, 3);
            return 0;
        }

        if (yuv_planes && yuv_planes[0]) {
            tj3DecompressToYUVPlanes8(handle, jpeg_data, jpeg_size, yuv_planes, strides);
        }
        cleanup_handle(handle);
    }
    else if (mode == 3) {
        // Legacy Decompression: tjInitDecompress -> tjDecompressToYUVPlanes
        tjhandle handle = tjInitDecompress();
        if (!handle) {
            cleanup_buffers(yuv_buf, yuv_planes, 3);
            return 0;
        }

        if (yuv_planes && yuv_planes[0]) {
            tjDecompressToYUVPlanes(handle, jpeg_data, jpeg_size, yuv_planes, width, strides, height, 0);
        }
        cleanup_handle(handle);
    }
    else {
        // Edge case: Invalid init type or just init/destroy
        tjhandle handle = tj3Init(-1);
        if (handle) cleanup_handle(handle);
        
        handle = tj3Init(TJINIT_COMPRESS);
        if (handle) {
            tj3Set(handle, 9999, 100); // Invalid param
            cleanup_handle(handle);
        }
    }

    cleanup_buffers(yuv_buf, yuv_planes, 3);
    return 0;
}