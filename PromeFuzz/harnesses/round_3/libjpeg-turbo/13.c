// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
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
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_subsamp_from_byte(uint8_t byte) {
    switch (byte % 6) {
        case 0: return TJSAMP_444;
        case 1: return TJSAMP_422;
        case 2: return TJSAMP_420;
        case 3: return TJSAMP_GRAY;
        case 4: return TJSAMP_440;
        case 5: return TJSAMP_411;
        default: return TJSAMP_444;
    }
}

static int get_pixel_format_from_byte(uint8_t byte) {
    switch (byte % 12) {
        case 0: return TJPF_RGB;
        case 1: return TJPF_BGR;
        case 2: return TJPF_RGBX;
        case 3: return TJPF_BGRX;
        case 4: return TJPF_XBGR;
        case 5: return TJPF_XRGB;
        case 6: return TJPF_GRAY;
        case 7: return TJPF_RGBA;
        case 8: return TJPF_BGRA;
        case 9: return TJPF_ABGR;
        case 10: return TJPF_ARGB;
        case 11: return TJPF_CMYK;
        default: return TJPF_RGB;
    }
}

static int get_align_from_byte(uint8_t byte) {
    switch (byte % 4) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default: return 4;
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    tjhandle handle_comp = tjInitCompress();
    tjhandle handle_decomp = tjInitDecompress();
    
    if (!handle_comp || !handle_decomp) {
        if (handle_comp) tjDestroy(handle_comp);
        if (handle_decomp) tjDestroy(handle_decomp);
        return 0;
    }
    
    // Use first 4 bytes for width and height
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    
    // Use next bytes for parameters
    int pixelSize = (Data[4] % 4) + 1;
    int jpegQual = Data[5] % 101;
    int flags = Data[6] & 1 ? TJFLAG_FASTDCT : TJFLAG_ACCURATEDCT;
    int subsamp = get_subsamp_from_byte(Data[7]);
    int pixelFormat = get_pixel_format_from_byte(Data[8]);
    int align = get_align_from_byte(Data[9]);
    
    // Adjust for CMYK
    if (pixelFormat == TJPF_CMYK) {
        pixelSize = 4;
    }
    
    // Allocate source buffer
    int pitch = width * pixelSize;
    int srcBufSize = pitch * height;
    unsigned char *srcBuf = NULL;
    
    if (srcBufSize > 0 && srcBufSize <= 1000000) {
        srcBuf = tjAlloc(srcBufSize);
        if (srcBuf) {
            size_t copySize = Size - 10;
            if (copySize > (size_t)srcBufSize) copySize = srcBufSize;
            memcpy(srcBuf, Data + 10, copySize);
            
            // Allocate destination buffer for compression
            unsigned long compressedSize = 0;
            unsigned char *dstBuf = tjAlloc(tjBufSize(width, height, subsamp));
            
            if (dstBuf) {
                // Test tjCompress
                tjCompress(handle_comp, srcBuf, width, pitch, height, pixelSize,
                          dstBuf, &compressedSize, subsamp, jpegQual, flags);
                
                if (compressedSize > 0 && compressedSize <= Size + 1000) {
                    // Allocate YUV buffer for decompression
                    int yuvWidth = width;
                    int yuvHeight = height;
                    int yuvAlign = align;
                    unsigned long yuvBufSize = tjBufSizeYUV2(yuvWidth, yuvAlign, yuvHeight, subsamp);
                    unsigned char *yuvBuf = NULL;
                    
                    if (yuvBufSize > 0 && yuvBufSize <= 1000000) {
                        yuvBuf = tjAlloc(yuvBufSize);
                        if (yuvBuf) {
                            // Test tjDecompressToYUV2
                            tjDecompressToYUV2(handle_decomp, dstBuf, compressedSize,
                                             yuvBuf, yuvWidth, yuvAlign, yuvHeight, flags);
                            
                            // Allocate destination buffer for YUV decoding
                            int decodeWidth = width;
                            int decodeHeight = height;
                            int decodePitch = decodeWidth * pixelSize;
                            int decodeBufSize = decodePitch * decodeHeight;
                            unsigned char *decodeBuf = NULL;
                            
                            if (decodeBufSize > 0 && decodeBufSize <= 1000000) {
                                decodeBuf = tjAlloc(decodeBufSize);
                                if (decodeBuf) {
                                    // Test tjDecodeYUV
                                    tjDecodeYUV(handle_decomp, yuvBuf, yuvAlign, subsamp,
                                               decodeBuf, decodeWidth, decodePitch, decodeHeight,
                                               pixelFormat, flags);
                                    
                                    // Test tjDecodeYUVPlanes
                                    const unsigned char *srcPlanes[3];
                                    int strides[3];
                                    int planeWidth = tjPlaneWidth(0, decodeWidth, subsamp);
                                    int planeHeight = tjPlaneHeight(0, decodeHeight, subsamp);
                                    
                                    if (planeWidth > 0 && planeHeight > 0) {
                                        unsigned long planeSize = planeWidth * planeHeight;
                                        if (planeSize <= yuvBufSize) {
                                            srcPlanes[0] = yuvBuf;
                                            strides[0] = planeWidth;
                                            
                                            if (subsamp != TJSAMP_GRAY) {
                                                int uvWidth = tjPlaneWidth(1, decodeWidth, subsamp);
                                                int uvHeight = tjPlaneHeight(1, decodeHeight, subsamp);
                                                unsigned long uvOffset = planeSize;
                                                unsigned long uvSize = uvWidth * uvHeight;
                                                
                                                if (uvOffset + uvSize <= yuvBufSize) {
                                                    srcPlanes[1] = yuvBuf + uvOffset;
                                                    srcPlanes[2] = yuvBuf + uvOffset + uvSize;
                                                    strides[1] = uvWidth;
                                                    strides[2] = uvWidth;
                                                    
                                                    tjDecodeYUVPlanes(handle_decomp, srcPlanes, strides,
                                                                     subsamp, decodeBuf, decodeWidth,
                                                                     decodePitch, decodeHeight,
                                                                     pixelFormat, flags);
                                                }
                                            }
                                        }
                                    }
                                    
                                    tjFree(decodeBuf);
                                }
                            }
                            
                            tjFree(yuvBuf);
                        }
                    }
                }
                
                tjFree(dstBuf);
            }
            
            tjFree(srcBuf);
        }
    }
    
    tjDestroy(handle_comp);
    tjDestroy(handle_decomp);
    
    return 0;
}