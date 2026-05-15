// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

static tjhandle init_decompressor(void) {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_decompressor(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static tjscalingfactor get_random_scaling_factor(const uint8_t *data, size_t *offset, size_t size) {
    tjscalingfactor factor = {1, 1};
    if (*offset + 1 < size) {
        const tjscalingfactor supported[] = {
            {1, 8}, {1, 4}, {3, 8}, {1, 2}, {5, 8}, {3, 4}, {7, 8}, {1, 1},
            {2, 1}, {4, 1}, {8, 1}
        };
        int idx = data[*offset] % (sizeof(supported) / sizeof(supported[0]));
        factor = supported[idx];
        (*offset)++;
    }
    return factor;
}

static tjregion get_random_region(const uint8_t *data, size_t *offset, size_t size, int max_width, int max_height) {
    tjregion region = {0, 0, 0, 0};
    if (*offset + 8 < size) {
        region.x = data[*offset] % (max_width > 0 ? max_width : 100);
        region.y = data[*offset + 1] % (max_height > 0 ? max_height : 100);
        region.w = data[*offset + 2] % ((max_width > 0 ? max_width : 100) - region.x + 1);
        region.h = data[*offset + 3] % ((max_height > 0 ? max_height : 100) - region.y + 1);
        *offset += 4;
    }
    return region;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    size_t offset = 0;
    tjhandle handle = init_decompressor();
    if (!handle) return 0;

    // Use part of the input as JPEG data
    size_t jpeg_size = Size / 2;
    const unsigned char *jpeg_buf = Data + offset;
    offset += jpeg_size;
    if (offset > Size) {
        cleanup_decompressor(handle);
        return 0;
    }

    // Set random parameters using tj3Set
    if (offset + 2 < Size) {
        int param = (Data[offset] % 10) + TJPARAM_STOPONWARNING;
        int value = Data[offset + 1];
        tj3Set(handle, param, value);
        offset += 2;
    }

    // Set scaling factor
    tjscalingfactor scaling_factor = get_random_scaling_factor(Data, &offset, Size);
    tj3SetScalingFactor(handle, scaling_factor);

    // Try to read JPEG header
    int width, height, subsamp;
    if (tj3DecompressHeader(handle, jpeg_buf, jpeg_size) == 0) {
        width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        subsamp = tj3Get(handle, TJPARAM_SUBSAMP);

        // Set cropping region (relative to scaled dimensions)
        int scaled_width = TJSCALED(width, scaling_factor);
        int scaled_height = TJSCALED(height, scaling_factor);
        tjregion cropping_region = get_random_region(Data, &offset, Size, scaled_width, scaled_height);
        tj3SetCroppingRegion(handle, cropping_region);

        // Allocate buffer for YUV8 decompression
        size_t yuv_buf_size = tj3YUVBufSize(scaled_width, 4, scaled_height, subsamp);
        unsigned char *yuv_buf = NULL;
        if (yuv_buf_size > 0 && yuv_buf_size < 10 * 1024 * 1024) { // Limit size to 10MB
            yuv_buf = (unsigned char *)malloc(yuv_buf_size);
            if (yuv_buf) {
                int align = 1 << (Data[offset] % 4); // 1, 2, 4, 8
                tj3DecompressToYUV8(handle, jpeg_buf, jpeg_size, yuv_buf, align);
                free(yuv_buf);
            }
        }

        // Try tjDecompressToYUVPlanes (legacy API)
        if (scaled_width > 0 && scaled_height > 0 && scaled_width < 4096 && scaled_height < 4096) {
            unsigned char *planes[3] = {NULL, NULL, NULL};
            int strides[3] = {0, 0, 0};
            
            // Calculate plane sizes based on subsampling
            int y_plane_size = scaled_width * scaled_height;
            int uv_width = (subsamp == TJSAMP_GRAY) ? 0 : (scaled_width + 1) / 2;
            int uv_height = (subsamp == TJSAMP_GRAY) ? 0 : (scaled_height + 1) / 2;
            int uv_plane_size = uv_width * uv_height;
            
            if (y_plane_size > 0 && y_plane_size < 5 * 1024 * 1024) {
                planes[0] = (unsigned char *)malloc(y_plane_size);
                if (planes[0]) strides[0] = scaled_width;
            }
            if (uv_plane_size > 0 && uv_plane_size < 5 * 1024 * 1024) {
                planes[1] = (unsigned char *)malloc(uv_plane_size);
                planes[2] = (unsigned char *)malloc(uv_plane_size);
                if (planes[1] && planes[2]) {
                    strides[1] = uv_width;
                    strides[2] = uv_width;
                }
            }
            
            if (planes[0] || planes[1] || planes[2]) {
                tjDecompressToYUVPlanes(handle, jpeg_buf, jpeg_size, planes, scaled_width, strides, scaled_height, 0);
            }
            
            if (planes[0]) free(planes[0]);
            if (planes[1]) free(planes[1]);
            if (planes[2]) free(planes[2]);
        }
    }

    // Test tj3Get with various parameters
    for (int i = 0; i < 5 && offset < Size; i++) {
        int param = (Data[offset] % 20) + TJPARAM_STOPONWARNING;
        tj3Get(handle, param);
        offset++;
    }

    cleanup_decompressor(handle);
    return 0;
}