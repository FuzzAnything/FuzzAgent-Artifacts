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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 232, nr_unique_branch: 20
//<Quality> {"density":29,"unique_branches":{"decompress_smooth_data":[[569,51,569,69,0,0,4,1]],"save_marker":[[279,38,279,43,9,0,4,0]],"jpeg_core_output_dimensions":[[166,16,166,68,0,0,4,1],[174,16,174,68,0,0,4,1],[182,16,182,69,0,0,4,1],[190,16,190,69,0,0,4,1],[198,16,198,69,0,0,4,1],[206,16,206,69,0,0,4,1],[214,16,214,69,0,0,4,1],[222,16,222,69,0,0,4,1]],"jpeg_idct_16x16":[[2430,17,2430,24,0,0,4,0],[2430,17,2430,24,0,0,4,1],[2529,17,2529,25,0,0,4,0],[2529,17,2529,25,0,0,4,1]],"tjDecompress2":[[2142,35,2142,76,0,0,4,0],[2155,7,2155,41,0,0,4,0],[2156,7,2156,25,0,0,4,1]],"first_marker":[[279,38,279,43,8,0,4,0]],"start_pass":[[154,5,154,17,0,0,4,1],[223,5,223,12,0,0,4,0]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tjDestroy","tjDecompressHeader2","tjDestroy","tjDestroy","tj3GetScalingFactors","tjDestroy","tjBufSizeYUV","tjDestroy","tjAlloc","tjDestroy","tjDecompress","tjFree","tjDestroy","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjFree","tjFree","tjDestroy","tjEncodeYUVPlanes","tjFree","tjFree","tjDestroy","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader","tjDecompressHeader2","tj3GetScalingFactors","tjBufSizeYUV","tjAlloc","tjDecompress","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjEncodeYUVPlanes","tjFree","tjFree","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<88) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
	//fuzzer shim end}




    // Initialize decompressor
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Read JPEG header using both header functions
    int width = 0, height = 0, jpegSubsamp = 0;
    
    // First header function
    if (tjDecompressHeader(handle, (unsigned char*)data, (unsigned long)size, &width, &height) != 0) {
        tjDestroy(handle);
        return 0;
    }
    
    // Second header function (for subsampling info)
    if (tjDecompressHeader2(handle, (unsigned char*)data, (unsigned long)size, &width, &height, &jpegSubsamp) != 0) {
        tjDestroy(handle);
        return 0;
    }

    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }

    // Get scaling factors
    int numScalingFactors = 0;
    tjscalingfactor* scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    if (!scalingFactors || numScalingFactors <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Use first scaling factor
    tjscalingfactor scale = scalingFactors[0];
    int scaledWidth = (width * scale.num + scale.denom - 1) / scale.denom;
    int scaledHeight = (height * scale.num + scale.denom - 1) / scale.denom;
    
    if (scaledWidth <= 0) scaledWidth = 1;
    if (scaledHeight <= 0) scaledHeight = 1;

    // Calculate buffer sizes
    unsigned long yuvBufSize = tjBufSizeYUV(fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3);
    if (yuvBufSize == 0 || yuvBufSize > 100 * 1024 * 1024) { // Limit to 100MB
        tjDestroy(handle);
        return 0;
    }

    // Allocate destination buffer for decompression (RGB format)
    int pixelSize = 3; // RGB
    int pitch = scaledWidth * pixelSize;
    unsigned char* dstBuf = (unsigned char*)tjAlloc(pitch * scaledHeight);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }

    // Decompress JPEG to RGB
    if (tjDecompress(handle, (unsigned char*)data, (unsigned long)size, 
                     dstBuf, scaledWidth, pitch, scaledHeight, fuzz_int32_t_4, fuzz_int32_t_5) != 0) {
        tjFree(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    // Calculate plane widths for YUV encoding
    int yPlaneWidth = tjPlaneWidth(fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8);
    int uPlaneWidth = tjPlaneWidth(fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11);
    int vPlaneWidth = tjPlaneWidth(fuzz_int32_t_12, fuzz_int32_t_13, fuzz_int32_t_14);
    
    // Allocate and prepare YUV planes
    unsigned char* yuvPlanes[3];
    int strides[3];
    
    // Calculate plane sizes and allocate
    for (int i = 0; i < 3; i++) {
        int planeW = tjPlaneWidth(fuzz_int32_t_15, width, fuzz_int32_t_16);
        int planeH = tjPlaneHeight(fuzz_int32_t_17, height, fuzz_int32_t_18);
        if (planeW <= 0) planeW = 1;
        if (planeH <= 0) planeH = 1;
        
        strides[i] = planeW;
        yuvPlanes[i] = (unsigned char*)tjAlloc(planeW * planeH);
        if (!yuvPlanes[i]) {
            for (int j = 0; j < i; j++) {
                tjFree(yuvPlanes[j]);
            }
            tjFree(dstBuf);
            tjDestroy(handle);
            return 0;
        }
    }

    // Encode RGB to YUV planes
    if (tjEncodeYUVPlanes(handle, dstBuf, fuzz_int32_t_19, pitch, fuzz_int32_t_20, TJPF_RGB, 
                          yuvPlanes, strides, fuzz_int32_t_21, fuzz_int32_t_22) != 0) {
        for (int i = 0; i < 3; i++) {
            tjFree(yuvPlanes[i]);
        }
        tjFree(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    // Cleanup
    for (int i = 0; i < 3; i++) {
        tjFree(yuvPlanes[i]);
    }
    tjFree(dstBuf);
    tjDestroy(handle);
    
    return 0;
}