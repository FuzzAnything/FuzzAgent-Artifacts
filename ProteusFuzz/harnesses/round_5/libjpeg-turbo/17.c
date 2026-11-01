// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static tjhandle init_handle(void) {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int try_tj3DecompressToYUV8(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (!handle || Size < 1) return -1;
    
    // We need to allocate a buffer for YUV output. 
    // Since we don't know the dimensions, we allocate a safe maximum size.
    // If the JPEG is valid but larger than our buffer, the function should return an error.
    // If the JPEG is invalid, it returns an error.
    size_t max_buf = 10 * 1024 * 1024; // 10MB
    unsigned char *dstBuf = (unsigned char *)malloc(max_buf);
    if (!dstBuf) return -1;

    int aligns[] = {1, 2, 4, 8, 16};
    for (int i = 0; i < 5; i++) {
        int ret = tj3DecompressToYUV8(handle, Data, Size, dstBuf, aligns[i]);
        if (ret == 0) {
            free(dstBuf);
            return 0;
        }
    }
    free(dstBuf);
    return -1;
}

static int try_tj3DecodeYUV8(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (!handle || Size < 1) return -1;
    
    // We interpret the input data as a YUV buffer.
    // We must ensure we don't read past the end of the input buffer.
    // We iterate through possible dimensions and check if the required YUV size fits in 'Size'.
    
    int widths[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int heights[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int aligns[] = {1, 2, 4, 8, 16};
    int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_RGBX, TJPF_XRGB, TJPF_RGBA, TJPF_ABGR, TJPF_BGRA};
    
    size_t max_rgb_size = 10 * 1024 * 1024;
    unsigned char *dstBuf = (unsigned char *)malloc(max_rgb_size);
    if (!dstBuf) return -1;

    for (int w_idx = 0; w_idx < 10; w_idx++) {
        for (int h_idx = 0; h_idx < 10; h_idx++) {
            int w = widths[w_idx];
            int h = heights[h_idx];
            
            // Calculate required YUV size for 4:2:0 (TJSAMP_420) as a guess
            // YUV size = w*h + (w/2)*(h/2)*2 = w*h + w*h/2 = 1.5 * w * h
            // With alignment, it might be larger.
            // We use tj3YUVBufSize to get the exact size needed for a specific subsampling.
            // We try TJSAMP_420 as it's common.
            size_t needed_size = tj3YUVBufSize(w, 1, h, TJSAMP_420);
            if (needed_size == 0 || needed_size > Size) continue;

            // Also check with alignment
            for (int a_idx = 0; a_idx < 5; a_idx++) {
                int align = aligns[a_idx];
                size_t needed_size_align = tj3YUVBufSize(w, align, h, TJSAMP_420);
                if (needed_size_align == 0 || needed_size_align > Size) continue;

                for (int f_idx = 0; f_idx < 8; f_idx++) {
                    int pf = formats[f_idx];
                    int pitch = 0; // Auto-calculate
                    
                    // Ensure we pass a valid pointer to the input data
                    int ret = tj3DecodeYUV8(handle, (const unsigned char *)Data, align, dstBuf, w, pitch, h, pf);
                    if (ret == 0) {
                        free(dstBuf);
                        return 0;
                    }
                }
            }
        }
    }
    free(dstBuf);
    return -1;
}

static int try_tjDecodeYUV(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (!handle || Size < 1) return -1;

    int widths[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int heights[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int aligns[] = {1, 2, 4, 8, 16};
    int subsamps[] = {TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY};
    int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_RGBX, TJPF_XRGB, TJPF_RGBA, TJPF_ABGR, TJPF_BGRA};
    int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT};

    size_t max_rgb_size = 10 * 1024 * 1024;
    unsigned char *dstBuf = (unsigned char *)malloc(max_rgb_size);
    if (!dstBuf) return -1;

    for (int w_idx = 0; w_idx < 10; w_idx++) {
        for (int h_idx = 0; h_idx < 10; h_idx++) {
            int w = widths[w_idx];
            int h = heights[h_idx];
            
            for (int a_idx = 0; a_idx < 5; a_idx++) {
                int align = aligns[a_idx];
                for (int s_idx = 0; s_idx < 4; s_idx++) {
                    int subsamp = subsamps[s_idx];
                    
                    // Calculate required YUV size for the specific subsampling and alignment
                    size_t needed_size = tj3YUVBufSize(w, align, h, subsamp);
                    if (needed_size == 0 || needed_size > Size) continue;

                    for (int f_idx = 0; f_idx < 8; f_idx++) {
                        int pf = formats[f_idx];
                        for (int fl_idx = 0; fl_idx < 3; fl_idx++) {
                            int flags_val = flags[fl_idx];
                            int pitch = 0;
                            
                            // Pass the input data as the source YUV buffer
                            int ret = tjDecodeYUV(handle, (const unsigned char *)Data, align, subsamp, dstBuf, w, pitch, h, pf, flags_val);
                            if (ret == 0) {
                                free(dstBuf);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    free(dstBuf);
    return -1;
}

static int try_tjDecompressToYUV(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (!handle || Size < 1) return -1;
    
    // Allocate a large buffer for YUV output
    size_t max_buf = 10 * 1024 * 1024;
    unsigned char *dstBuf = (unsigned char *)malloc(max_buf);
    if (!dstBuf) return -1;

    int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT};
    for (int i = 0; i < 3; i++) {
        int ret = tjDecompressToYUV(handle, (unsigned char *)Data, (unsigned long)Size, dstBuf, flags[i]);
        if (ret == 0) {
            free(dstBuf);
            return 0;
        }
    }
    free(dstBuf);
    return -1;
}

static int try_tjDecompress(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (!handle || Size < 1) return -1;

    // We need width, height, pixelSize.
    // We try a range of dimensions.
    int widths[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int heights[] = {1, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int pixelSizes[] = {1, 2, 3, 4}; // Grayscale, RGB, etc.
    int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_RGBX, TJPF_XRGB, TJPF_RGBA, TJPF_ABGR, TJPF_BGRA};
    int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT, TJ_YUV};

    size_t max_rgb_size = 10 * 1024 * 1024;
    unsigned char *dstBuf = (unsigned char *)malloc(max_rgb_size);
    if (!dstBuf) return -1;

    for (int w_idx = 0; w_idx < 10; w_idx++) {
        for (int h_idx = 0; h_idx < 10; h_idx++) {
            int w = widths[w_idx];
            int h = heights[h_idx];
            
            for (int ps_idx = 0; ps_idx < 4; ps_idx++) {
                int ps = pixelSizes[ps_idx];
                for (int f_idx = 0; f_idx < 8; f_idx++) {
                    int pf = formats[f_idx];
                    for (int fl_idx = 0; fl_idx < 4; fl_idx++) {
                        int flags_val = flags[fl_idx];
                        int pitch = 0; // Auto
                        
                        int ret = tjDecompress(handle, (unsigned char *)Data, (unsigned long)Size, dstBuf, w, pitch, h, ps, flags_val);
                        if (ret == 0) {
                            free(dstBuf);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    free(dstBuf);
    return -1;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = init_handle();
    if (!handle) return 0;

    // Try all target functions
    // 1. tj3DecompressToYUV8
    try_tj3DecompressToYUV8(handle, Data, Size);

    // 2. tj3DecodeYUV8
    try_tj3DecodeYUV8(handle, Data, Size);

    // 3. tjDecodeYUV
    try_tjDecodeYUV(handle, Data, Size);

    // 4. tjDecompressToYUV
    try_tjDecompressToYUV(handle, Data, Size);

    // 5. tjDecompress
    try_tjDecompress(handle, Data, Size);

    cleanup_handle(handle);
    return 0;
}