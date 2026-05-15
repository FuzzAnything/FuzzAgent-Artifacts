#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <turbojpeg.h>
}
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"tjCompress2":[[1274,27,1274,42,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3Destroy","tjDecompressHeader2","tj3Alloc","tj3Destroy","tjCompress2","tj3Get","tjPlaneHeight","tjPlaneWidth","tjPlaneSizeYUV","tj3Alloc","tjEncodeYUVPlanes","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Alloc","tjBufSizeYUV2","tjEncodeYUV2","tj3Free","tj3Alloc","tj3SaveImage12","tj3Free","tj3Free","tjGetErrorCode","tjGetErrorStr2","tjFree","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tjDecompressHeader2","tj3Alloc","tjCompress2","tj3Get","tjPlaneHeight","tjPlaneWidth","tjPlaneSizeYUV","tj3Alloc","tjEncodeYUVPlanes","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Alloc","tjBufSizeYUV2","tjEncodeYUV2","tj3Free","tj3Alloc","tj3SaveImage12","tj3Free","tj3Free","tjGetErrorCode","tjGetErrorStr2","tjFree","tj3Free","tj3Destroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<84) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful JPEG data
    
    tjhandle handle = nullptr;
    int width = 0, height = 0;
    int pixelFormat = TJPF_RGB;
    int subsamp = TJSAMP_420;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    
    // Initialize compressor handle
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
	return 0;
	}
	
    
    // Create a temporary input file with the JPEG data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read JPEG header to get dimensions
    fseek(in_file, 0, SEEK_SET);
    if (tjDecompressHeader2(handle, (unsigned char *)data, (unsigned long)size, 
                           &width, &height, &subsamp) != 0) {
        // Try alternative approach if header reading fails
        width = 64;
        height = 64;
    }
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 64;
    if (height <= 0) height = 64;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Allocate source buffer
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned char *srcBuf = (unsigned char *)tj3Alloc(pitch * height);
    if (!srcBuf) {
        assert_file_closed(&in_file);;
        tj3Destroy(handle);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Fill source buffer with pattern
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < pitch; j++) {
            srcBuf[i * pitch + j] = (i + j) % 256;
        }
    }
    
    // 1. Use tjCompress2 to compress image
    if (tjCompress2(handle, srcBuf, fuzz_int32_t_1, pitch, fuzz_int32_t_2, pixelFormat,
                   &jpegBuf, &jpegSize, subsamp, fuzz_int32_t_3, fuzz_int32_t_4) == 0) {
        
        // 2. Use tj3Get to retrieve a parameter
        int quality = tj3Get(handle, TJPARAM_QUALITY);
        
        // 3. Use tjPlaneHeight to calculate plane height
        int planeHeight = tjPlaneHeight(fuzz_int32_t_5, fuzz_int32_t_6, subsamp);
        
        // Allocate YUV planes
        int strides[3];
        unsigned char *yuvPlanes[3];
        for (int i = 0; i < 3; i++) {
            strides[i] = tjPlaneWidth(fuzz_int32_t_7, fuzz_int32_t_8, subsamp);
            int planeSize = tjPlaneSizeYUV(fuzz_int32_t_9, fuzz_int32_t_10, strides[i], fuzz_int32_t_11, subsamp);
            yuvPlanes[i] = (unsigned char *)tj3Alloc(planeSize);
            if (yuvPlanes[i]) {
                memset(yuvPlanes[i], 128, planeSize);  // Initialize with gray
            }
        }
        
        // 4. Use tjEncodeYUVPlanes to encode to YUV planes
        if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
            tjEncodeYUVPlanes(handle, srcBuf, fuzz_int32_t_12, pitch, fuzz_int32_t_13, pixelFormat,
                            yuvPlanes, strides, subsamp, fuzz_int32_t_14);
            
            // 5. Use tj3DecodeYUVPlanes8 to decode back to RGB
            unsigned char *dstBuf = (unsigned char *)tj3Alloc(pitch * height);
            if (dstBuf) {
                tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)yuvPlanes, 
                                   strides, dstBuf, fuzz_int32_t_15, pitch, fuzz_int32_t_16, pixelFormat);
                tj3Free(dstBuf);
            }
            
            // 6. Use tjEncodeYUV2 to encode RGB to YUV buffer
            unsigned char *yuvBuf = (unsigned char *)tj3Alloc(
                tjBufSizeYUV2(width, 4, height, subsamp));
            if (yuvBuf) {
                tjEncodeYUV2(handle, srcBuf, fuzz_int32_t_17, pitch, fuzz_int32_t_18, pixelFormat,
                           yuvBuf, subsamp, fuzz_int32_t_19);
                tj3Free(yuvBuf);
            }
        }
        
        // 7. Use tj3SaveImage12 to save 12-bit image (convert 8-bit to 12-bit)
        short *buffer12 = (short *)tj3Alloc(width * height * 3 * sizeof(short));
        if (buffer12) {
            // Convert 8-bit to 12-bit by shifting
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width * 3; j++) {
                    buffer12[i * width * 3 + j] = srcBuf[i * pitch + j] << 4;
                }
            }
            tj3SaveImage12(handle, "output_file", buffer12, fuzz_int32_t_20, width * 3 * sizeof(short), 
                          fuzz_int32_t_21, TJPF_RGB);
            tj3Free(buffer12);
        }
        
        // Clean up YUV planes
        for (int i = 0; i < 3; i++) {
            if (yuvPlanes[i]) tj3Free(yuvPlanes[i]);
        }
        
        // 8. Use tjGetErrorStr2 to check for errors
        if (tjGetErrorCode(handle) != 0) {
            const char *errorStr = tjGetErrorStr2(handle);
            // Error string available for debugging
        }
        
        if (jpegBuf) tjFree(jpegBuf);
    }
    
    // Cleanup
    tj3Free(srcBuf);
    assert_file_closed(&in_file);;
    tj3Destroy(handle);
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}