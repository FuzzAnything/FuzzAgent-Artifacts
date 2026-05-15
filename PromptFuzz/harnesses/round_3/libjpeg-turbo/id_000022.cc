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
//<ID> 279
//<Prompt> []
/*<Combination>: [
*/
//<score> 69.666664, nr_unique_branch: 54
//<Quality> {"density":19,"unique_branches":{"tj3CompressFromYUVPlanes8":[[1347,7,1347,26,0,0,4,1],[1349,7,1349,38,0,0,4,1],[1358,7,1358,22,0,0,4,1],[1364,7,1364,27,0,0,4,1],[1366,15,1366,40,0,0,4,0],[1366,15,1366,40,0,0,4,1],[1376,9,1376,23,0,0,4,0],[1376,9,1376,23,0,0,4,1],[1376,27,1376,38,0,0,4,0],[1379,9,1379,74,0,0,4,1],[1382,19,1382,30,0,0,4,0],[1382,19,1382,30,0,0,4,1],[1384,15,1384,22,0,0,4,0],[1384,26,1384,41,0,0,4,0],[1387,7,1387,16,0,0,4,0],[1388,9,1388,76,0,0,4,1],[1391,17,1391,42,0,0,4,0],[1391,17,1391,42,0,0,4,1],[1392,11,1392,77,0,0,4,1],[1394,21,1394,32,0,0,4,0],[1394,21,1394,32,0,0,4,1],[1403,17,1403,47,0,0,4,0],[1403,17,1403,47,0,0,4,1],[1408,17,1408,42,0,0,4,0],[1408,17,1408,42,0,0,4,1],[1412,11,1412,20,0,0,4,0],[1415,21,1415,28,0,0,4,0],[1415,21,1415,28,0,0,4,1],[1418,27,1418,36,0,0,4,0],[1418,27,1418,36,0,0,4,1],[1422,35,1422,44,0,0,4,0],[1422,35,1422,44,0,0,4,1],[1435,45,1435,57,0,0,4,1],[515,26,515,35,23,0,4,0],[515,26,515,35,23,0,4,1]],"jpeg_finish_compress":[[184,7,184,45,0,0,4,1],[185,7,185,43,0,0,4,0]],"jinit_compress_master":[[42,7,42,26,0,0,4,1]],"tj3CompressFromYUV8":[[1497,7,1497,38,0,0,4,0]],"prepare_for_pass":[[589,9,589,28,0,0,4,1]],"jpeg_start_compress":[[63,26,63,44,0,0,4,0]],"start_pass_main":[[63,7,63,25,0,0,4,0]],"tj3SetScalingFactor":[[2045,43,2045,77,0,0,4,1]],"jpeg_write_raw_data":[[158,7,158,47,0,0,4,1],[161,7,161,30,0,0,4,1],[164,7,164,43,0,0,4,1],[166,7,166,50,0,0,4,1],[172,7,172,30,0,0,4,1],[183,7,183,39,0,0,4,0],[183,7,183,39,0,0,4,1],[188,7,188,37,0,0,4,1],[192,7,192,42,0,0,4,1],[194,7,194,52,0,0,4,1]],"jinit_c_main_controller":[[167,7,167,25,0,0,4,0]]},"library_calls":["tj3InitVersion","tj3LoadImage16","tj3Free","tj3Destroy","tj3Set","tj3Set","tj3SetScalingFactor","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tj3YUVBufSize","tj3Alloc","tj3Alloc","tj3EncodeYUV8","tj3Free","tj3CompressFromYUV8","tjInitCompress","tjCompressFromYUV","tjFree","tjDestroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3LoadImage16","tj3Set","tj3Set","tj3SetScalingFactor","tj3SetCroppingRegion","tj3Alloc","tj3Decompress8","tj3YUVBufSize","tj3Alloc","tj3Alloc","tj3EncodeYUV8","tj3Free","tj3CompressFromYUV8","tjInitCompress","tjCompressFromYUV","tjFree","tjDestroy","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tj3Alloc","tj3DecodeYUVPlanes8","tj3Free","tj3Free","tj3Free","tj3Free","tj3Free","tj3Destroy"],"visited":2}
/*Here's a C++ fuzz driver that uses all the specified libjpeg-turbo APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<120) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_27, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_28, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_29, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_30, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // 1. First, write input data to a file for loading with tj3LoadImage16
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Write to a temporary file since tj3LoadImage16 needs a filename
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data to temp file
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    assert_file_closed(&in_file);;
    
    tjhandle handle = nullptr;
    int ret = 0;
    int width = 0, height = 0, pixelFormat = 0;
    int align = 4;  // Default alignment
    
    // 2. Load image using tj3LoadImage16 (requires file input)
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    unsigned short *loadedImage = tj3LoadImage16(handle, "input_file", &width, fuzz_int32_t_1, &height, &pixelFormat);
    
    if (!loadedImage || width <= 0 || height <= 0) {
        if (loadedImage) tj3Free(loadedImage);
        tj3Destroy(handle);
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Set some parameters using tj3Set
    ret = tj3Set(handle, TJPARAM_FASTUPSAMPLE, fuzz_int32_t_2);
    ret = tj3Set(handle, TJPARAM_FASTDCT, fuzz_int32_t_3);
    
    // 4. Set scaling factor
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2;  // Scale to half size
    
    ret = tj3SetScalingFactor(handle, scalingFactor);
    
    // 5. Set cropping region (crop to center 50% of image)
    tjregion croppingRegion;
    croppingRegion.x = width / 4;
    croppingRegion.y = height / 4;
    croppingRegion.w = width / 2;
    croppingRegion.h = height / 2;
    
    ret = tj3SetCroppingRegion(handle, croppingRegion);
    
    // Calculate pitch for the loaded image
    int pitch = width * tjPixelSize[pixelFormat];
    
    // 6. Decompress the input JPEG data (if it's JPEG)
    unsigned char *decompressedBuffer = nullptr;
    if (size > 2 && data[0] == 0xFF && data[1] == 0xD8) {  // JPEG header check
        // Allocate buffer for decompressed image
        decompressedBuffer = (unsigned char *)tj3Alloc(pitch * height);
        if (decompressedBuffer) {
            ret = tj3Decompress8(handle, data, size, decompressedBuffer, pitch, fuzz_int32_t_4);
        }
    }
    
    // 7. Convert loaded 16-bit image to YUV format first
    size_t yuvBufSize = tj3YUVBufSize(fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, TJSAMP_420);
    unsigned char *yuvBuffer = nullptr;
    
    if (yuvBufSize > 0) {
        yuvBuffer = (unsigned char *)tj3Alloc(yuvBufSize);
        if (yuvBuffer) {
            // We need to convert 16-bit to 8-bit first for YUV encoding
            unsigned char *eightBitBuffer = (unsigned char *)tj3Alloc(width * height * tjPixelSize[pixelFormat]);
            if (eightBitBuffer) {
                // Simple conversion: take lower 8 bits
                for (size_t i = 0; i < width * height * tjPixelSize[pixelFormat]; i++) {
                    eightBitBuffer[i] = (unsigned char)(loadedImage[i] & 0xFF);
                }
                
                // Encode to YUV
                ret = tj3EncodeYUV8(handle, eightBitBuffer, fuzz_int32_t_8, pitch, fuzz_int32_t_9, 
                                   fuzz_int32_t_10, yuvBuffer, fuzz_int32_t_11);
                
                tj3Free(eightBitBuffer);
            }
        }
    }
    
    // 8. Compress from YUV using tj3CompressFromYUV8
    unsigned char *jpegBuf1 = nullptr;
    size_t jpegSize1 = 0;
    
    if (yuvBuffer) {
        ret = tj3CompressFromYUV8(handle, yuvBuffer, fuzz_int32_t_12, fuzz_int32_t_13, fuzz_int32_t_14, 
                                 &jpegBuf1, &jpegSize1);
    }
    
    // 9. For tjCompressFromYUV (legacy API), we need to use legacy handle
    tjhandle legacyHandle = tjInitCompress();
    if (legacyHandle) {
        unsigned char *jpegBuf2 = nullptr;
        unsigned long jpegSize2 = 0;
        
        if (yuvBuffer) {
            ret = tjCompressFromYUV(legacyHandle, yuvBuffer, fuzz_int32_t_15, fuzz_int32_t_16, fuzz_int32_t_17,
                                   TJSAMP_420, &jpegBuf2, &jpegSize2, fuzz_int32_t_18, fuzz_int32_t_19);
        }
        
        if (jpegBuf2) {
            tjFree(jpegBuf2);
        }
        tjDestroy(legacyHandle);
    }
    
    // 10. Use tj3DecodeYUVPlanes8 - need YUV planes
    if (yuvBuffer) {
        // Prepare YUV planes
        const unsigned char *srcPlanes[3];
        int strides[3];
        
        // For 4:2:0 subsampling
        int yWidth = tj3YUVPlaneWidth(fuzz_int32_t_20, fuzz_int32_t_21, TJSAMP_420);
        int yHeight = tj3YUVPlaneHeight(fuzz_int32_t_22, fuzz_int32_t_23, TJSAMP_420);
        int uvWidth = tj3YUVPlaneWidth(fuzz_int32_t_24, fuzz_int32_t_25, TJSAMP_420);
        int uvHeight = tj3YUVPlaneHeight(fuzz_int32_t_26, fuzz_int32_t_27, TJSAMP_420);
        
        // Calculate plane pointers (simplified - actual implementation depends on YUV layout)
        srcPlanes[0] = yuvBuffer;  // Y plane
        strides[0] = yWidth;
        
        // U and V planes (simplified calculation)
        size_t yPlaneSize = yWidth * yHeight;
        srcPlanes[1] = yuvBuffer + yPlaneSize;  // U plane
        strides[1] = uvWidth;
        srcPlanes[2] = yuvBuffer + yPlaneSize + (uvWidth * uvHeight);  // V plane
        strides[2] = uvWidth;
        
        unsigned char *decodedBuffer = (unsigned char *)tj3Alloc(width * height * 3);
        if (decodedBuffer) {
            ret = tj3DecodeYUVPlanes8(handle, srcPlanes, strides, decodedBuffer,
                                     fuzz_int32_t_28, pitch, fuzz_int32_t_29, fuzz_int32_t_30);
            tj3Free(decodedBuffer);
        }
    }
    
    // 11. Clean up all allocated resources
    if (loadedImage) tj3Free(loadedImage);
    if (decompressedBuffer) tj3Free(decompressedBuffer);
    if (yuvBuffer) tj3Free(yuvBuffer);
    if (jpegBuf1) tj3Free(jpegBuf1);
    
    tj3Destroy(handle);
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}