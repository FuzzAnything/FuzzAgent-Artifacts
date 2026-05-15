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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 31, nr_unique_branch: 0
//<Quality> {"density":31,"unique_branches":{},"library_calls":["tjInitDecompress","tjDecompressHeader2","tjGetErrorStr2","tjDestroy","tjDestroy","tjBufSizeYUV","tjDestroy","tj3InitVersion","tjDestroy","tj3Set","tj3Set","tj3Get","tjAlloc","tj3Destroy","tjDestroy","tjDecompress2","tjGetErrorStr2","tjFree","tj3Destroy","tjDestroy","tjAlloc","tjFree","tjFree","tj3Destroy","tjDestroy","tjEncodeYUVPlanes","tjGetErrorStr2","tj3SaveImage8","tjFree","tjFree","tj3Destroy","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader2","tjBufSizeYUV","tj3InitVersion","tj3Set","tj3Set","tj3Get","tjAlloc","tjDecompress2","tjAlloc","tjEncodeYUVPlanes","tjGetErrorStr2","tj3SaveImage8","tjFree","tjFree","tj3Destroy","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<74) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small to be a valid JPEG
    
    tjhandle handle = nullptr;
    unsigned char* jpegBuf = nullptr;
    unsigned char* yuvPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0};
    FILE* out_file = nullptr;
    
    // 1. Create decompressor handle using old API
    handle = tjInitDecompress();
    if (!handle) {
	return 0;
	}
	
    
    // 2. Use input data as JPEG buffer
    jpegBuf = (unsigned char*)data;
    unsigned long jpegSize = (unsigned long)size;
    
    // 3. Get JPEG header information
    int width = 0, height = 0, jpegSubsamp = 0;
    int result = tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    if (result != 0) {
        // Error occurred, get error string
        char* errorStr = tjGetErrorStr2(handle);
        if (errorStr) {
            // Could log error here if needed
        }
        tjDestroy(handle);
        return 0;
    }
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }
    
    // 4. Calculate YUV buffer size using old API
    unsigned long yuvBufSize = tjBufSizeYUV(fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3);
    if (yuvBufSize == 0) {
        tjDestroy(handle);
        return 0;
    }
    
    // 5. Create new API handle for encoding/saving
    tjhandle newHandle = tj3InitVersion(TJINIT_COMPRESS, fuzz_int32_t_4);
    if (!newHandle) {
        tjDestroy(handle);
        return 0;
    }
    
    // 6. Set some parameters on new handle
    tj3Set(newHandle, TJPARAM_QUALITY, fuzz_int32_t_5);
    tj3Set(newHandle, TJPARAM_SUBSAMP, fuzz_int32_t_6);
    
    // 7. Get a parameter back to verify
    int quality = tj3Get(newHandle, TJPARAM_QUALITY);
    
    // 8. Allocate memory for decompressed RGB image
    int pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned long rgbBufSize = pitch * height;
    unsigned char* rgbBuf = (unsigned char*)tjAlloc(rgbBufSize);
    
    if (!rgbBuf) {
        tj3Destroy(newHandle);
        tjDestroy(handle);
        return 0;
    }
    
    // 9. Decompress JPEG to RGB buffer
    result = tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, fuzz_int32_t_7, pitch, fuzz_int32_t_8, pixelFormat, fuzz_int32_t_9);
    
    if (result != 0) {
        char* errorStr = tjGetErrorStr2(handle);
        tjFree(rgbBuf);
        tj3Destroy(newHandle);
        tjDestroy(handle);
        return 0;
    }
    
    // 10. Allocate YUV planes
    for (int i = 0; i < 3; i++) {
        yuvPlanes[i] = (unsigned char*)tjAlloc(yuvBufSize / 3);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) tjFree(yuvPlanes[j]);
            tjFree(rgbBuf);
            tj3Destroy(newHandle);
            tjDestroy(handle);
            return 0;
        }
    }
    
    // 11. Encode RGB to YUV planes
    result = tjEncodeYUVPlanes(handle, rgbBuf, fuzz_int32_t_10, pitch, fuzz_int32_t_11, pixelFormat, 
                               yuvPlanes, strides, fuzz_int32_t_12, fuzz_int32_t_13);
    
    if (result != 0) {
        char* errorStr = tjGetErrorStr2(handle);
        // Clean up and continue to try saving
    }
    
    // 12. Save the RGB image to file using new API
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // Use one of the YUV planes as input for saving (just for demonstration)
        // In practice, we'd save the RGB buffer
        if (yuvPlanes[0]) {
            tj3SaveImage8(newHandle, fuzz_str_14, rgbBuf, fuzz_int32_t_15, pitch, fuzz_int32_t_16, pixelFormat);
        }
        assert_file_closed(&out_file);;
    }
    
    // 13. Clean up
    tjFree(rgbBuf);
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
    }
    tj3Destroy(newHandle);
    tjDestroy(handle);
    
    assert_file_closed(&out_file);
	return 0;
}