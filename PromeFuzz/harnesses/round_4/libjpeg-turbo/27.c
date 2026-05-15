// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_handles(tjhandle *handles, int count) {
    for (int i = 0; i < count; i++) {
        if (handles[i]) {
            tjDestroy(handles[i]);
            handles[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Initialize handles
    tjhandle handles[3] = {0};
    handles[0] = tjInitCompress();
    handles[1] = tjInitDecompress();
    handles[2] = tj3Init(TJINIT_DECOMPRESS);
    
    // Test tjGetErrorCode (old API)
    if (handles[0]) {
        tjGetErrorCode(handles[0]);
    }
    
    // Test tj3GetErrorCode (new API)
    if (handles[2]) {
        tj3GetErrorCode(handles[2]);
    }
    
    // Test tjDecompressHeader2
    if (handles[1] && Size > 20) {
        int width = 0, height = 0, jpegSubsamp = 0;
        unsigned long jpegSize = Size > 1000 ? 1000 : Size;
        tjDecompressHeader2(handles[1], (unsigned char*)Data, jpegSize, 
                           &width, &height, &jpegSubsamp);
        tjGetErrorCode(handles[1]);
    }
    
    // Test tj3DecompressToYUV8
    if (handles[2] && Size > 100) {
        unsigned long yuvSize = Size > 5000 ? 5000 : Size;
        unsigned char *yuvBuf = (unsigned char*)malloc(yuvSize);
        if (yuvBuf) {
            int align = 1 << (Data[0] % 4); // 1, 2, 4, or 8
            unsigned long jpegSize = Size > 2000 ? 2000 : Size;
            tj3DecompressToYUV8(handles[2], Data, jpegSize, yuvBuf, align);
            tj3GetErrorCode(handles[2]);
            free(yuvBuf);
        }
    }
    
    // Test tjCompress2
    if (handles[0] && Size > 100) {
        int width = 16 + (Data[0] % 64);
        int height = 16 + (Data[1] % 64);
        int pitch = width * 3;
        int pixelFormat = TJPF_RGB;
        int jpegSubsamp = Data[2] % 4;
        int jpegQual = 50 + (Data[3] % 50);
        int flags = Data[4] % 8;
        
        unsigned char *srcBuf = (unsigned char*)malloc(width * height * 3);
        if (srcBuf) {
            // Fill with pattern
            for (int i = 0; i < width * height * 3 && i < (int)Size; i++) {
                srcBuf[i] = Data[i % Size];
            }
            
            unsigned char *jpegBuf = NULL;
            unsigned long jpegSize = 0;
            tjCompress2(handles[0], srcBuf, width, pitch, height, pixelFormat,
                       &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
            
            if (jpegBuf) {
                tjFree(jpegBuf);
            }
            free(srcBuf);
        }
        tjGetErrorCode(handles[0]);
    }
    
    // Test tjDecompress2
    if (handles[1] && Size > 100) {
        write_dummy_file(Data, Size > 2000 ? 2000 : Size);
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long fileSize = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            if (fileSize > 0) {
                unsigned char *jpegBuf = (unsigned char*)malloc(fileSize);
                if (jpegBuf) {
                    fread(jpegBuf, 1, fileSize, f);
                    
                    int width = 32 + (Data[0] % 128);
                    int height = 32 + (Data[1] % 128);
                    int pitch = width * 3;
                    int pixelFormat = TJPF_RGB;
                    int flags = Data[2] % 4;
                    
                    unsigned char *dstBuf = (unsigned char*)malloc(width * height * 3);
                    if (dstBuf) {
                        tjDecompress2(handles[1], jpegBuf, fileSize, dstBuf,
                                     width, pitch, height, pixelFormat, flags);
                        free(dstBuf);
                    }
                    free(jpegBuf);
                }
            }
            fclose(f);
        }
        tjGetErrorCode(handles[1]);
    }
    
    // Cleanup
    cleanup_handles(handles, 3);
    
    return 0;
}