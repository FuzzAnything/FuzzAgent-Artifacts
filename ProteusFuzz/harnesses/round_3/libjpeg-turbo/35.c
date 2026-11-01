// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int subsamp = Data[2] & 0x07; // Mask to valid range 0-7
    int pixelFormat = Data[3] & 0x0F; // Mask to valid range 0-15

    // Clamp dimensions to reasonable limits to prevent allocation failures or crashes
    if (width == 0 || height == 0) return 0;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Test tjPlaneWidth
    int y_width = tjPlaneWidth(0, width, subsamp);
    int cb_width = tjPlaneWidth(1, width, subsamp);
    int cr_width = tjPlaneWidth(2, width, subsamp);
    if (y_width < 0 || cb_width < 0 || cr_width < 0) {
        // Invalid arguments, but function handled gracefully
    }

    // Test tjPlaneHeight
    int y_height = tjPlaneHeight(0, height, subsamp);
    int cb_height = tjPlaneHeight(1, height, subsamp);
    int cr_height = tjPlaneHeight(2, height, subsamp);
    if (y_height < 0 || cb_height < 0 || cr_height < 0) {
        // Invalid arguments
    }

    // Test tj3YUVPlaneHeight
    int y_height_3 = tj3YUVPlaneHeight(0, height, subsamp);
    int cb_height_3 = tj3YUVPlaneHeight(1, height, subsamp);
    int cr_height_3 = tj3YUVPlaneHeight(2, height, subsamp);
    // Return 0 on error is expected for invalid args

    // Prepare source buffer for encoding
    int bytes_per_pixel = 3; // Assume RGB for simplicity
    if (pixelFormat == TJPF_GRAY) bytes_per_pixel = 1;
    else if (pixelFormat == TJPF_XBGR || pixelFormat == TJPF_BGR || pixelFormat == TJPF_XRGB || pixelFormat == TJPF_RGB) bytes_per_pixel = 3;
    else if (pixelFormat == TJPF_ABGR || pixelFormat == TJPF_ARGB || pixelFormat == TJPF_BGRA || pixelFormat == TJPF_RGBA) bytes_per_pixel = 4;
    else bytes_per_pixel = 3; // Default fallback

    int pitch = width * bytes_per_pixel;
    size_t src_size = (size_t)pitch * height;
    
    // Ensure we don't overflow or allocate too much
    if (src_size > (size_t)0x10000000) src_size = 0x10000000;

    unsigned char *srcBuf = NULL;
    if (src_size > 0) {
        srcBuf = (unsigned char *)malloc(src_size);
        if (!srcBuf) return 0;
        if (Size > 0) {
            size_t copy_len = (Size < src_size) ? Size : src_size;
            memcpy(srcBuf, Data, copy_len);
        }
    }

    // Initialize TurboJPEG handle
    tjhandle handle = tjInitCompress();
    if (!handle) {
        free(srcBuf);
        return 0;
    }

    // Allocate YUV planes for tjEncodeYUVPlanes and tj3EncodeYUVPlanes8
    unsigned char *yPlane = NULL;
    unsigned char *cbPlane = NULL;
    unsigned char *crPlane = NULL;
    unsigned char **dstPlanes = NULL;
    int *strides = NULL;

    // Only proceed with plane allocation if dimensions are valid
    if (y_width > 0 && y_height > 0) {
        dstPlanes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
        strides = (int *)malloc(3 * sizeof(int));

        if (!dstPlanes || !strides) {
            free(srcBuf);
            cleanup_handle(handle);
            free(dstPlanes);
            free(strides);
            return 0;
        }

        // Calculate plane sizes safely
        size_t y_size = (size_t)y_width * y_height;
        size_t cb_size = 0;
        size_t cr_size = 0;

        if (cb_width > 0 && cb_height > 0) cb_size = (size_t)cb_width * cb_height;
        if (cr_width > 0 && cr_height > 0) cr_size = (size_t)cr_width * cr_height;

        // Limit allocation sizes to prevent OOM
        if (y_size > 0x10000000) y_size = 0x10000000;
        if (cb_size > 0x10000000) cb_size = 0x10000000;
        if (cr_size > 0x10000000) cr_size = 0x10000000;

        if (y_size > 0) yPlane = (unsigned char *)malloc(y_size);
        if (cb_size > 0) cbPlane = (unsigned char *)malloc(cb_size);
        if (cr_size > 0) crPlane = (unsigned char *)malloc(cr_size);

        dstPlanes[0] = yPlane;
        dstPlanes[1] = cbPlane;
        dstPlanes[2] = crPlane;

        strides[0] = 0; // Auto-calculate
        strides[1] = 0;
        strides[2] = 0;

        // Test tjEncodeYUVPlanes
        // Ensure srcBuf is valid before calling
        if (srcBuf) {
            int ret1 = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides, subsamp, 0);
            if (ret1 != 0) {
                // Error expected for invalid subsamp or other issues
            }
        }

        // Test tj3EncodeYUVPlanes8
        if (srcBuf) {
            int ret2 = tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides);
            if (ret2 != 0) {
                // Error expected
            }
        }
    }

    // Allocate buffer for tjEncodeYUV2
    // tjEncodeYUV2 requires a single destination buffer.
    int yuv_size = tjBufSize(width, height, subsamp);
    unsigned char *dstBuf = NULL;
    
    if (yuv_size > 0 && yuv_size < 0x10000000) {
        dstBuf = (unsigned char *)malloc(yuv_size);
        if (dstBuf && srcBuf) {
            // Test tjEncodeYUV2
            int ret3 = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, 0);
            if (ret3 != 0) {
                // Error expected
            }
        }
    }

    // Cleanup
    free(srcBuf);
    free(yPlane);
    free(cbPlane);
    free(crPlane);
    free(dstPlanes);
    free(strides);
    free(dstBuf);
    cleanup_handle(handle);

    return 0;
}