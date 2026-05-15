// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
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

static int parse_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    return data[(*offset)++];
}

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    int value = 0;
    for (int i = 0; i < 4; i++) {
        value = (value << 8) | data[(*offset)++];
    }
    return value;
}

static unsigned long parse_ulong(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 8 > size) return 0;
    unsigned long value = 0;
    for (int i = 0; i < 8; i++) {
        value = (value << 8) | data[(*offset)++];
    }
    return value;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    int selector = parse_byte(Data, Size, &offset) % 6;
    
    tjhandle handle = NULL;
    unsigned char *buffer = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int width = 0, height = 0, pitch = 0;
    int pixelFormat = 0, subsamp = 0, flags = 0;
    int jpegQual = 0, jpegSubsamp = 0;
    int pixelSize = 0, align = 0;
    int bytes = 0;
    
    switch (selector) {
        case 0: { // tjDecodeYUVPlanes
            handle = tjInitDecompress();
            if (!handle) return 0;
            
            width = parse_int(Data, Size, &offset) % 1024 + 1;
            height = parse_int(Data, Size, &offset) % 1024 + 1;
            pitch = parse_int(Data, Size, &offset) % (width * 4) + width;
            pixelFormat = parse_byte(Data, Size, &offset) % 12;
            subsamp = parse_byte(Data, Size, &offset) % 4;
            flags = parse_int(Data, Size, &offset);
            
            int maxPlanes = 3;
            const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
            int strides[3] = {0, 0, 0};
            
            for (int i = 0; i < maxPlanes; i++) {
                strides[i] = parse_int(Data, Size, &offset) % 1024;
                int planeSize = width * height;
                if (i > 0 && subsamp != TJSAMP_GRAY) {
                    planeSize = (width + 7) / 8 * ((height + 7) / 8) * 64;
                }
                if (offset + planeSize > Size) {
                    planeSize = Size - offset;
                }
                if (planeSize > 0) {
                    srcPlanes[i] = Data + offset;
                    offset += planeSize;
                }
            }
            
            int dstBufSize = pitch * height;
            buffer = tjAlloc(dstBufSize);
            if (buffer) {
                tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                  buffer, width, pitch, height, pixelFormat, flags);
                tjFree(buffer);
            }
            
            tjDestroy(handle);
            break;
        }
        
        case 1: { // tjFree
            bytes = parse_int(Data, Size, &offset) % 65536;
            buffer = tjAlloc(bytes);
            if (buffer) {
                tjFree(buffer);
            }
            break;
        }
        
        case 2: { // tjCompress2
            handle = tjInitCompress();
            if (!handle) return 0;
            
            width = parse_int(Data, Size, &offset) % 1024 + 1;
            height = parse_int(Data, Size, &offset) % 1024 + 1;
            pitch = parse_int(Data, Size, &offset) % (width * 4) + width;
            pixelFormat = parse_byte(Data, Size, &offset) % 12;
            jpegSubsamp = parse_byte(Data, Size, &offset) % 4;
            jpegQual = parse_byte(Data, Size, &offset) % 101;
            flags = parse_int(Data, Size, &offset);
            
            int srcBufSize = pitch * height;
            if (offset + srcBufSize > Size) {
                srcBufSize = Size - offset;
            }
            const unsigned char *srcBuf = Data + offset;
            offset += srcBufSize;
            
            if (srcBufSize > 0) {
                tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                           &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
                if (jpegBuf) {
                    tjFree(jpegBuf);
                }
            }
            
            tjDestroy(handle);
            break;
        }
        
        case 3: { // tjCompress
            handle = tjInitCompress();
            if (!handle) return 0;
            
            width = parse_int(Data, Size, &offset) % 1024 + 1;
            height = parse_int(Data, Size, &offset) % 1024 + 1;
            pitch = parse_int(Data, Size, &offset) % (width * 4) + width;
            pixelSize = parse_byte(Data, Size, &offset) % 4 + 1;
            jpegSubsamp = parse_byte(Data, Size, &offset) % 4;
            jpegQual = parse_byte(Data, Size, &offset) % 101;
            flags = parse_int(Data, Size, &offset);
            
            int srcBufSize = pitch * height;
            if (offset + srcBufSize > Size) {
                srcBufSize = Size - offset;
            }
            unsigned char *srcBuf = (unsigned char *)Data + offset;
            offset += srcBufSize;
            
            int dstBufSize = width * height * pixelSize * 2;
            buffer = tjAlloc(dstBufSize);
            if (buffer && srcBufSize > 0) {
                tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                          buffer, &jpegSize, jpegSubsamp, jpegQual, flags);
                tjFree(buffer);
            }
            
            tjDestroy(handle);
            break;
        }
        
        case 4: { // tjAlloc
            bytes = parse_int(Data, Size, &offset) % 65536;
            buffer = tjAlloc(bytes);
            if (buffer) {
                tjFree(buffer);
            }
            break;
        }
        
        case 5: { // tjDecompressToYUV2
            handle = tjInitDecompress();
            if (!handle) return 0;
            
            jpegSize = parse_ulong(Data, Size, &offset) % Size;
            if (jpegSize == 0) jpegSize = Size;
            if (offset + jpegSize > Size) {
                jpegSize = Size - offset;
            }
            const unsigned char *jpegBufData = Data + offset;
            offset += jpegSize;
            
            width = parse_int(Data, Size, &offset) % 1024 + 1;
            height = parse_int(Data, Size, &offset) % 1024 + 1;
            align = parse_int(Data, Size, &offset) % 16 + 1;
            flags = parse_int(Data, Size, &offset);
            
            int dstBufSize = width * height * 3;
            buffer = tjAlloc(dstBufSize);
            if (buffer && jpegSize > 0) {
                tjDecompressToYUV2(handle, jpegBufData, jpegSize,
                                  buffer, width, align, height, flags);
                tjFree(buffer);
            }
            
            tjDestroy(handle);
            break;
        }
    }
    
    return 0;
}