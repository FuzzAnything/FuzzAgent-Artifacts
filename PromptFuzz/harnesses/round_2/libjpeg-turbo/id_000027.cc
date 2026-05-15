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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"tj3Compress8":[[95,26,95,45,0,0,4,0],[140,45,140,57,0,0,4,0]]},"library_calls":["tjInitDecompress","tj3LoadImage8","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tjDestroy","tjFree","tjFree","tj3SetScalingFactor","tjAlloc","tjEncodeYUV2","tjGetErrorCode","tjGetErrorStr2","tj3Compress8","tj3GetErrorStr","tjDestroy","tjDestroy","tjFree","tjFree","tjFree","tjFree"],"critical_calls":["tjInitDecompress","tj3LoadImage8","tj3LoadImage16","TJBUFSIZEYUV","tjBufSizeYUV","tjInitCompress","tj3SetScalingFactor","tjAlloc","tjEncodeYUV2","tjGetErrorCode","tjGetErrorStr2","tj3Compress8","tj3GetErrorStr","tjDestroy","tjDestroy","tjFree","tjFree","tjFree","tjFree"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;
    }

    // Step 1: Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // Step 2: Initialize decompressor
    tjhandle decompressor = tjInitDecompress();
    if (!decompressor) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Load image in 8-bit format
    int width8, height8, pixelFormat8;
    unsigned char *img8 = tj3LoadImage8(decompressor, "input_file", &width8, 1, &height8, &pixelFormat8);
    
    // Step 4: Load image in 16-bit format (if 8-bit failed, use default values)
    int width16 = 100, height16 = 100, pixelFormat16 = TJPF_RGB;
    unsigned short *img16 = nullptr;
    if (img8) {
        // Re-open file for 16-bit loading
        img16 = tj3LoadImage16(decompressor, "input_file", &width16, 1, &height16, &pixelFormat16);
    }

    // Step 5: Calculate YUV buffer sizes using both functions
    unsigned long bufsize_yuv1 = 0, bufsize_yuv2 = 0;
    if (img8 && width8 > 0 && height8 > 0) {
        bufsize_yuv1 = TJBUFSIZEYUV(width8, height8, TJSAMP_420);
        bufsize_yuv2 = tjBufSizeYUV(width8, height8, TJSAMP_420);
    }

    // Step 6: Initialize compressor
    tjhandle compressor = tjInitCompress();
    if (!compressor) {
        tjDestroy(decompressor);
        if (img8) tjFree(img8);
        if (img16) tjFree((unsigned char*)img16);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // Step 7: Set scaling factor (1/2 scaling)
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2;
    tj3SetScalingFactor(decompressor, scalingFactor);

    // Step 8: Encode 8-bit image to YUV (if loaded successfully)
    unsigned char *yuvBuf = nullptr;
    if (img8 && width8 > 0 && height8 > 0 && bufsize_yuv1 > 0) {
        yuvBuf = (unsigned char*)tjAlloc(bufsize_yuv1);
        if (yuvBuf) {
            int pitch = width8 * tjPixelSize[pixelFormat8];
            int flags = 0;
            int result = tjEncodeYUV2(compressor, img8, width8, pitch, height8, 
                                     pixelFormat8, yuvBuf, TJSAMP_420, flags);
            // Error checking
            if (result != 0) {
                tjGetErrorCode(compressor);
                char *errStr = tjGetErrorStr2(compressor);
                (void)errStr; // Use error string to avoid unused variable warning
            }
        }
    }

    // Step 9: Compress 8-bit image to JPEG
    unsigned char *jpegBuf = nullptr;
    size_t jpegSize = 0;
    if (img8 && width8 > 0 && height8 > 0) {
        int pitch = width8 * tjPixelSize[pixelFormat8];
        int result = tj3Compress8(compressor, img8, width8, pitch, height8,
                                 pixelFormat8, &jpegBuf, &jpegSize);
        if (result != 0) {
            char *errStr = tj3GetErrorStr(compressor);
            (void)errStr;
        }
    }

    // Step 10: Write compressed JPEG to output file (for debugging)
    if (jpegBuf && jpegSize > 0) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(jpegBuf, 1, jpegSize, out_file);
            assert_file_closed(&out_file);;
        }
    }

    // Step 11: Clean up
    tjDestroy(decompressor);
    tjDestroy(compressor);
    
    if (img8) tjFree(img8);
    if (img16) tjFree((unsigned char*)img16);
    if (yuvBuf) tjFree(yuvBuf);
    if (jpegBuf) tjFree(jpegBuf);
    
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}