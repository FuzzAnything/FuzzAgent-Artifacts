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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 172.5, nr_unique_branch: 133
//<Quality> {"density":15,"unique_branches":{"jpeg_calc_output_dimensions":[[284,7,284,31,0,0,4,1]],"jinit_d_post_controller":[[271,7,271,30,0,0,4,0],[273,9,273,48,0,0,4,1],[273,52,273,77,0,0,4,1]],"tjDecompressToYUVPlanes":[[2346,7,2346,37,0,0,4,1],[2349,7,2349,22,0,0,4,1],[2349,26,2349,39,0,0,4,1],[2349,43,2349,52,0,0,4,1],[2349,56,2349,66,0,0,4,1],[2376,7,2376,41,0,0,4,0],[2377,7,2377,25,0,0,4,0],[2377,7,2377,25,0,0,4,1],[329,7,329,12,1,0,4,1]],"jinit_color_deconverter":[[630,7,630,30,0,0,4,0],[632,9,632,48,0,0,4,1],[632,52,632,77,0,0,4,1],[653,3,653,21,0,0,4,1],[658,3,658,15,0,0,4,0],[659,3,659,17,0,0,4,1],[660,9,660,35,0,0,4,1],[664,3,664,16,0,0,4,1],[665,3,665,16,0,0,4,1],[670,3,670,10,0,0,4,1],[684,3,684,21,0,0,4,1],[704,3,704,15,0,0,4,1],[705,3,705,19,0,0,4,0],[706,3,706,20,0,0,4,1],[707,3,707,19,0,0,4,1],[708,3,708,20,0,0,4,1],[709,3,709,20,0,0,4,1],[710,3,710,20,0,0,4,1],[711,3,711,20,0,0,4,1],[712,3,712,20,0,0,4,1],[713,3,713,20,0,0,4,1],[714,3,714,20,0,0,4,1],[716,9,716,32,0,0,4,0],[716,36,716,70,0,0,4,1],[720,9,720,45,0,0,4,1],[755,16,755,56,0,0,4,1],[782,16,782,50,0,0,4,0],[783,11,783,47,0,0,4,0],[784,11,784,49,0,0,4,0],[785,11,785,48,0,0,4,0],[786,11,786,53,0,0,4,0],[820,3,820,18,0,0,4,1],[857,3,857,16,0,0,4,1],[873,3,873,10,0,0,4,1],[883,7,883,29,0,0,4,1]],"master_selection":[[528,7,528,30,0,0,4,0],[611,9,611,38,0,0,4,1],[623,11,623,37,0,0,4,0],[650,7,650,30,0,0,4,0],[655,9,655,35,0,0,4,0],[662,9,662,26,0,0,4,0],[662,9,662,26,0,0,4,1],[669,20,669,55,0,0,4,0],[669,20,669,55,0,0,4,1],[670,20,670,41,0,0,4,1],[671,9,671,35,0,0,4,0],[671,9,671,35,0,0,4,1],[673,14,673,41,0,0,4,1],[691,9,691,26,0,0,4,0],[720,7,720,27,0,0,4,1]],"tj3DecodeYUVPlanes8":[[2583,7,2583,17,0,0,4,0],[2673,7,2673,41,0,0,4,0]],"jinit_lossless_decompressor":[[285,7,285,46,0,0,4,1],[285,50,285,75,0,0,4,1]],"tj3Decompress16":[[165,7,165,37,0,0,4,1],[168,7,168,22,0,0,4,1],[168,26,168,39,0,0,4,1],[168,43,168,57,0,0,4,1],[168,61,168,70,0,0,4,1],[169,7,169,22,0,0,4,1],[169,26,169,49,0,0,4,1],[172,7,172,22,0,0,4,1],[184,7,184,45,0,0,4,0],[184,7,184,45,0,0,4,1],[284,7,284,41,0,0,4,0],[286,7,286,25,0,0,4,0],[286,7,286,25,0,0,4,1],[329,7,329,12,3,0,4,1]],"tjPlaneWidth":[[1144,10,1144,23,0,0,4,1]],"setDecodeDefaults":[[2528,9,2528,46,0,0,4,1]],"jinit_d_main_controller":[[435,7,435,30,0,0,4,0],[437,9,437,48,0,0,4,1],[437,52,437,77,0,0,4,1]],"use_merged_upsample":[[42,7,42,30,0,0,4,0]],"jinit_upsampler":[[446,7,446,30,0,0,4,0],[448,9,448,48,0,0,4,1],[448,52,448,77,0,0,4,1],[461,7,461,47,0,0,4,0],[472,7,472,30,0,0,4,1],[478,14,478,40,0,0,4,1],[483,44,483,70,0,0,4,0],[483,44,483,70,0,0,4,1],[496,9,496,35,0,0,4,1],[500,16,500,41,0,0,4,0],[500,16,500,41,0,0,4,1],[500,45,500,70,0,0,4,0],[504,16,504,45,0,0,4,0],[504,49,504,74,0,0,4,1],[521,16,521,41,0,0,4,1],[531,16,531,45,0,0,4,0],[532,16,532,45,0,0,4,0],[534,11,534,19,0,0,4,1],[544,13,544,43,0,0,4,0],[558,9,558,20,0,0,4,0],[558,9,558,20,0,0,4,1],[558,24,558,64,0,0,4,0]],"jpeg_make_d_derived_tbl":[[201,9,201,44,0,0,4,0],[257,22,257,63,0,0,4,0]],"jinit_arith_decoder":[[762,15,762,33,0,0,4,0],[762,15,762,33,0,0,4,1],[770,7,770,30,0,0,4,1]],"start_pass_lhuff_decoder":[[89,16,89,41,0,0,4,0],[89,16,89,41,0,0,4,1],[93,9,93,18,0,0,4,1],[93,22,93,44,0,0,4,1],[94,9,94,47,0,0,4,0],[103,29,103,57,0,0,4,1]],"jpeg_core_output_dimensions":[[108,7,108,31,0,0,4,1]],"jinit_lhuff_decoder":[[297,15,297,32,0,0,4,0],[297,15,297,32,0,0,4,1]],"default_decompress_parms":[[172,13,172,36,0,0,4,0]],"jinit_d_diff_controller":[[354,7,354,46,0,0,4,1],[354,50,354,75,0,0,4,1],[369,44,369,70,0,0,4,0],[369,44,369,70,0,0,4,1],[383,7,383,23,0,0,4,1]],"read_markers":[[1020,5,1020,17,0,0,4,0],[1021,11,1021,45,0,0,4,1]],"jsimd_set_h2v2_upsample":[[516,7,516,23,0,0,4,1],[520,7,520,47,0,0,4,0]]},"library_calls":["tjLoadImage","tjInitDecompress","tjInitCompress","tjGetScalingFactors","tjGetErrorStr2","tjBufSizeYUV","tjAlloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjPlaneSizeYUV","tjPlaneWidth","tjDecompressToYUVPlanes","tjDecodeYUVPlanes","tj3InitVersion","tj3Decompress16","tj3JPEGBufSize","tjCompress2","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"critical_calls":["tjLoadImage","tjInitDecompress","tjInitCompress","tjGetScalingFactors","tjGetErrorStr2","tjBufSizeYUV","tjAlloc","tj3YUVPlaneWidth","tj3YUVPlaneHeight","tjPlaneSizeYUV","tjPlaneWidth","tjDecompressToYUVPlanes","tjDecodeYUVPlanes","tj3InitVersion","tj3Decompress16","tj3JPEGBufSize","tjCompress2","tjFree","tjFree","tjFree","tjFree","tjDestroy","tjDestroy","tj3Destroy"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified libjpeg-turbo APIs to achieve the event of loading a JPEG image, decompressing it to YUV planes, then recompressing it and saving the result:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a file for tjLoadImage
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Variables for image parameters
    int width = 0, height = 0, pixelFormat = 0;
    int align = 4; // Default alignment
    int flags = 0;
    tjhandle decompHandle = nullptr;
    tjhandle compressHandle = nullptr;
    tjhandle legacyDecompHandle = nullptr;
    tjhandle legacyCompressHandle = nullptr;
    unsigned char *loadedImage = nullptr;
    unsigned short *decompressed16bit = nullptr;
    unsigned char *yuvBuffer = nullptr;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    int subsamp = TJSAMP_420;
    int jpegQual = 85;
    
    // 1. Load image using tjLoadImage
    loadedImage = tjLoadImage("input_file", &width, align, &height, &pixelFormat, flags);
    if (!loadedImage) {
        // Clean up and continue with other APIs
        remove("input_file");
        width = 64; height = 64; // Default values for other APIs
    }
    
    // 2. Initialize decompressor handles
    legacyDecompHandle = tjInitDecompress();
    legacyCompressHandle = tjInitCompress();
    
    // 3. Use scaling factors API
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tjGetScalingFactors(&numScalingFactors);
    
    // 4. Use error string API
    char *errorStr = nullptr;
    if (legacyDecompHandle) {
        errorStr = tjGetErrorStr2(legacyDecompHandle);
    }
    
    // 5. Allocate memory for YUV planes using tjAlloc
    int yuvSize = tjBufSizeYUV(width, height, subsamp);
    unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    
    if (yuvSize > 0) {
        yuvBuffer = tjAlloc(yuvSize);
        
        // Calculate plane dimensions using new API functions
        int planeWidth = tj3YUVPlaneWidth(0, width, subsamp);
        int planeHeight = tj3YUVPlaneHeight(0, height, subsamp);
        
        // Allocate individual planes
        for (int i = 0; i < 3; i++) {
            yuvPlanes[i] = (unsigned char *)malloc(tjPlaneSizeYUV(i, width, 0, height, subsamp));
            strides[i] = tjPlaneWidth(i, width, subsamp);
        }
    }
    
    // 6. Decompress input to YUV planes using legacy API
    if (legacyDecompHandle && yuvBuffer) {
        int result = tjDecompressToYUVPlanes(legacyDecompHandle, data, size, 
                                            yuvPlanes, width, strides, height, flags);
        // Result intentionally ignored for fuzzing
    }
    
    // 7. Decode YUV planes back to RGB using legacy API
    unsigned char *decodedImage = nullptr;
    if (legacyDecompHandle && yuvPlanes[0]) {
        decodedImage = (unsigned char *)malloc(width * height * 3);
        if (decodedImage) {
            tjDecodeYUVPlanes(legacyDecompHandle, (const unsigned char **)yuvPlanes, 
                             strides, subsamp, decodedImage, width, 0, height, 
                             TJPF_RGB, flags);
        }
    }
    
    // 8. Create a handle for new API (using tj3Init)
    tjhandle newDecompHandle = tj3Init(TJINIT_DECOMPRESS);
    
    // 9. Use tj3Decompress16 to decompress to 16-bit pixels
    if (newDecompHandle && width > 0 && height > 0) {
        size_t bufSize = width * height * sizeof(unsigned short) * 3;
        decompressed16bit = (unsigned short *)malloc(bufSize);
        if (decompressed16bit) {
            tj3Decompress16(newDecompHandle, data, size, 
                           decompressed16bit, 0, TJPF_RGB);
        }
    }
    
    // 10. Calculate JPEG buffer size using new API
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    
    // 11. Compress the loaded image (if available) using legacy API
    if (legacyCompressHandle && loadedImage && width > 0 && height > 0) {
        unsigned char *compressedBuf = nullptr;
        unsigned long compressedSize = 0;
        
        int result = tjCompress2(legacyCompressHandle, loadedImage, width, 0, 
                                height, pixelFormat, &compressedBuf, &compressedSize, 
                                subsamp, jpegQual, flags);
        
        if (result == 0 && compressedBuf) {
            // Save compressed image to file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                fwrite(compressedBuf, 1, compressedSize, out_file);
                assert_file_closed(&out_file);;
                tjFree(compressedBuf);
            }
        }
    }
    
    // 12. Clean up all allocated resources
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    // Free allocated memory
    if (loadedImage) {
        tjFree(loadedImage);
    }
    
    if (decompressed16bit) {
        free(decompressed16bit);
    }
    
    if (yuvBuffer) {
        tjFree(yuvBuffer);
    }
    
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) {
            free(yuvPlanes[i]);
        }
    }
    
    if (decodedImage) {
        free(decodedImage);
    }
    
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    
    // Destroy handles
    if (legacyDecompHandle) {
        tjDestroy(legacyDecompHandle);
    }
    
    if (legacyCompressHandle) {
        tjDestroy(legacyCompressHandle);
    }
    
    if (newDecompHandle) {
        tj3Destroy(newDecompHandle);
    }
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}