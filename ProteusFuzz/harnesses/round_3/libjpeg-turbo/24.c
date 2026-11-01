// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

/* Helper to extract parameters from fuzz input safely */
static int get_int_param(const uint8_t *data, size_t size, size_t *offset, int min_val, int max_val) {
    if (*offset + 4 > size) return 0;
    int val = *(int*)(data + *offset);
    *offset += 4;
    if (val < min_val || val > max_val) return 0;
    return val;
}

static int get_size_param(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 8 > size) return 0;
    size_t val = *(size_t*)(data + *offset);
    *offset += 8;
    /* Sanity check for huge allocations */
    if (val > 100 * 1024 * 1024) return 0;
    return (int)val;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    size_t offset = 0;
    int initType = get_int_param(Data, Size, &offset, TJINIT_DECOMPRESS, TJINIT_TRANSFORM);
    if (initType == 0) initType = TJINIT_COMPRESS; /* Default to compress if invalid */

    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    /* Fuzz tj3Compress16 */
    if (offset + 24 <= Size) {
        int width = get_int_param(Data, Size, &offset, 1, 4096);
        int pitch = get_int_param(Data, Size, &offset, 0, 8192);
        int height = get_int_param(Data, Size, &offset, 1, 4096);
        int pixelFormat = get_int_param(Data, Size, &offset, TJPF_GRAY, TJPF_CMYK);
        
        if (width > 0 && height > 0 && pixelFormat >= 0) {
            size_t srcSize = (size_t)width * height * 2; /* 16-bit samples */
            if (srcSize > 0 && srcSize <= 10 * 1024 * 1024) {
                unsigned short *srcBuf = (unsigned short *)malloc(srcSize);
                if (srcBuf) {
                    memset(srcBuf, 0, srcSize);
                    /* Fill with fuzz data if available */
                    size_t copyLen = (Size - offset) < srcSize ? (Size - offset) : srcSize;
                    if (copyLen > 0) memcpy(srcBuf, Data + offset, copyLen);
                    
                    unsigned char *jpegBuf = NULL;
                    size_t jpegSize = 0;
                    tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
                    
                    if (jpegBuf) free(jpegBuf);
                    free(srcBuf);
                }
            }
        }
    }

    /* Fuzz tj3JPEGBufSize */
    if (offset + 8 <= Size) {
        int w = get_int_param(Data, Size, &offset, 1, 4096);
        int h = get_int_param(Data, Size, &offset, 1, 4096);
        int sub = get_int_param(Data, Size, &offset, TJSAMP_444, TJSAMP_420);
        if (sub < 0) sub = TJSAMP_444;
        size_t bufSize = tj3JPEGBufSize(w, h, sub);
        (void)bufSize; /* Use return value to avoid unused warning */
    }

    /* Fuzz tj3SetICCProfile */
    if (offset + 8 <= Size) {
        int iccSize = get_size_param(Data, Size, &offset);
        unsigned char *iccBuf = NULL;
        if (iccSize > 0 && iccSize < 1024 * 1024) {
            iccBuf = (unsigned char *)malloc(iccSize);
            if (iccBuf) {
                size_t copyLen = (Size - offset) < iccSize ? (Size - offset) : iccSize;
                if (copyLen > 0) memcpy(iccBuf, Data + offset, copyLen);
                else memset(iccBuf, 0, iccSize);
            }
        }
        tj3SetICCProfile(handle, iccBuf, iccSize);
        if (iccBuf) free(iccBuf);
    }

    /* Fuzz tj3CompressFromYUV8 */
    if (offset + 16 <= Size) {
        int w = get_int_param(Data, Size, &offset, 1, 4096);
        int align = get_int_param(Data, Size, &offset, 1, 32);
        int h = get_int_param(Data, Size, &offset, 1, 4096);
        
        if (w > 0 && h > 0 && align > 0) {
            size_t yuvSize = tj3YUVBufSize(w, align, h, TJSAMP_420);
            if (yuvSize > 0 && yuvSize <= 10 * 1024 * 1024) {
                unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
                if (yuvBuf) {
                    memset(yuvBuf, 0, yuvSize);
                    size_t copyLen = (Size - offset) < yuvSize ? (Size - offset) : yuvSize;
                    if (copyLen > 0) memcpy(yuvBuf, Data + offset, copyLen);
                    
                    unsigned char *jpegBuf = NULL;
                    size_t jpegSize = 0;
                    tj3CompressFromYUV8(handle, yuvBuf, w, align, h, &jpegBuf, &jpegSize);
                    
                    if (jpegBuf) free(jpegBuf);
                    free(yuvBuf);
                }
            }
        }
    }

    /* Fuzz tjCompress2 */
    if (offset + 24 <= Size) {
        int w = get_int_param(Data, Size, &offset, 1, 4096);
        int pitch = get_int_param(Data, Size, &offset, 0, 8192);
        int h = get_int_param(Data, Size, &offset, 1, 4096);
        int pf = get_int_param(Data, Size, &offset, TJPF_GRAY, TJPF_CMYK);
        int sub = get_int_param(Data, Size, &offset, TJSAMP_444, TJSAMP_420);
        int qual = get_int_param(Data, Size, &offset, 1, 100);
        int flags = get_int_param(Data, Size, &offset, 0, 1000);
        
        if (w > 0 && h > 0 && pf >= 0) {
            int ps = tjPixelSize[pf];
            if (ps > 0) {
                size_t srcSize = (size_t)w * h * ps;
                if (srcSize > 0 && srcSize <= 10 * 1024 * 1024) {
                    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
                    if (srcBuf) {
                        memset(srcBuf, 0, srcSize);
                        size_t copyLen = (Size - offset) < srcSize ? (Size - offset) : srcSize;
                        if (copyLen > 0) memcpy(srcBuf, Data + offset, copyLen);
                        
                        unsigned char *jpegBuf = NULL;
                        unsigned long jpegSize = 0;
                        tjCompress2(handle, srcBuf, w, pitch, h, pf, &jpegBuf, &jpegSize, sub, qual, flags);
                        
                        if (jpegBuf) free(jpegBuf);
                        free(srcBuf);
                    }
                }
            }
        }
    }

    /* Cleanup */
    if (handle) {
        tj3Destroy(handle);
    }

    return 0;
}