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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 126.5, nr_unique_branch: 173
//<Quality> {"density":23,"unique_branches":{"encode_mcu_gather":[[876,7,876,30,0,0,4,1],[887,18,887,45,0,0,4,0],[887,18,887,45,0,0,4,1]],"compress_first_pass":[[274,44,274,70,0,0,4,0],[274,44,274,70,0,0,4,1],[282,9,282,43,0,0,4,0],[282,9,282,43,0,0,4,1],[287,11,287,26,0,0,4,0],[293,9,293,19,0,0,4,1],[298,25,298,47,0,0,4,0],[298,25,298,47,0,0,4,1],[304,11,304,21,0,0,4,1],[319,9,319,44,0,0,4,0],[319,9,319,44,0,0,4,1],[322,36,322,70,0,0,4,1]],"start_pass_huff":[[143,7,143,24,0,0,4,0],[163,9,163,26,0,0,4,0],[167,11,167,20,0,0,4,1],[167,24,167,46,0,0,4,1],[169,11,169,20,0,0,4,1],[169,24,169,46,0,0,4,1],[173,11,173,48,0,0,4,0],[173,11,173,48,0,0,4,1],[178,11,178,48,0,0,4,0],[178,11,178,48,0,0,4,1]],"jpeg_calc_output_dimensions":[[338,3,338,21,0,0,4,0],[342,3,342,19,0,0,4,1]],"jpeg_write_scanlines":[[124,7,124,39,0,0,4,1]],"compress_output":[[373,16,373,41,0,0,4,0],[373,16,373,41,0,0,4,1],[382,41,382,78,0,0,4,0],[382,41,382,78,0,0,4,1],[384,39,384,72,0,0,4,0],[384,39,384,72,0,0,4,1],[388,20,388,45,0,0,4,0],[388,20,388,45,0,0,4,1],[391,26,391,54,0,0,4,0],[391,26,391,54,0,0,4,1],[393,28,393,55,0,0,4,0],[393,28,393,55,0,0,4,1],[402,11,402,67,0,0,4,1]],"jinit_compress_master":[[127,48,127,70,0,0,4,0]],"tj3SaveImage8":[[539,7,539,10,0,0,4,0],[539,14,539,38,0,0,4,1],[546,14,546,17,0,0,4,0],[546,21,546,45,0,0,4,1]],"j12init_write_ppm":[[324,7,324,46,0,0,4,1],[325,7,325,50,0,0,4,1],[339,7,339,46,0,0,4,1],[340,7,340,47,0,0,4,1],[366,9,366,32,0,0,4,0],[369,16,369,50,0,0,4,1],[78,4,78,17,11,0,4,1],[78,22,78,39,11,0,4,1]],"start_pass_coef":[[111,3,111,22,0,0,4,1],[117,3,117,26,0,0,4,0],[118,9,118,37,0,0,4,1],[122,3,122,23,0,0,4,0],[123,9,123,37,0,0,4,1]],"jinit_c_master_control":[[802,7,802,29,0,0,4,0]],"jpeg_gen_optimal_table":[[951,15,951,22,0,0,4,0],[951,15,951,22,0,0,4,1],[964,15,964,22,0,0,4,0],[964,15,964,22,0,0,4,1],[965,9,965,16,0,0,4,0],[965,9,965,16,0,0,4,1],[984,17,984,35,0,0,4,0],[984,17,984,35,0,0,4,1],[985,11,985,24,0,0,4,0],[985,11,985,24,0,0,4,1],[986,13,986,25,0,0,4,0],[986,13,986,25,0,0,4,1],[999,9,999,15,0,0,4,0],[999,9,999,15,0,0,4,1],[1011,12,1011,27,0,0,4,0],[1011,12,1011,27,0,0,4,1],[1020,12,1020,27,0,0,4,0],[1020,12,1020,27,0,0,4,1],[1027,15,1027,33,0,0,4,0],[1027,15,1027,33,0,0,4,1],[1030,9,1030,31,0,0,4,1],[1042,15,1042,28,0,0,4,0],[1042,15,1042,28,0,0,4,1],[1058,22,1058,28,0,0,4,0],[1058,22,1058,28,0,0,4,1],[1059,12,1059,23,0,0,4,1],[1072,10,1072,22,0,0,4,0],[1072,10,1072,22,0,0,4,1],[1084,15,1084,37,0,0,4,0],[1084,15,1084,37,0,0,4,1]],"jinit_c_coef_controller":[[451,7,451,23,0,0,4,0],[458,46,458,72,0,0,4,0],[458,46,458,72,0,0,4,1]],"prepare_for_pass":[[597,34,597,58,0,0,4,0],[600,9,600,31,0,0,4,0],[630,9,630,32,0,0,4,1]],"access_virt_barray":[[1072,9,1072,22,0,0,4,1],[1081,11,1081,20,0,0,4,1]],"tj3SetCroppingRegion":[[2065,7,2065,37,0,0,4,0],[340,7,340,12,1,0,4,1]],"finish_pass_master":[[697,9,697,32,0,0,4,1],[706,9,706,31,0,0,4,0]],"tj3Set":[[742,3,742,24,0,0,4,1],[750,3,750,23,0,0,4,0],[751,9,751,33,0,0,4,1],[755,3,755,23,0,0,4,0],[781,3,781,23,0,0,4,0],[784,3,784,24,0,0,4,0],[785,9,785,33,0,0,4,1],[789,3,789,27,0,0,4,0],[790,9,790,33,0,0,4,1],[799,3,799,26,0,0,4,0],[800,9,800,33,0,0,4,1],[719,7,719,23,7,0,4,1],[719,44,719,60,7,0,4,1],[719,7,719,23,8,0,4,1],[719,28,719,40,8,0,4,0],[719,44,719,60,8,0,4,1],[725,7,725,16,18,0,4,1],[725,20,725,29,18,0,4,1],[725,7,725,16,20,0,4,1],[725,20,725,29,20,0,4,1],[725,7,725,16,22,0,4,1],[725,20,725,29,22,0,4,1],[725,7,725,16,26,0,4,1],[725,20,725,29,26,0,4,1]],"htest_one_block":[[806,7,806,15,0,0,4,0],[806,7,806,15,0,0,4,1],[811,10,811,14,0,0,4,0],[811,10,811,14,0,0,4,1],[818,7,818,32,0,0,4,1],[828,15,828,27,0,0,4,0],[828,15,828,27,0,0,4,1],[829,9,829,51,0,0,4,0],[829,9,829,51,0,0,4,1],[833,14,833,20,0,0,4,0],[833,14,833,20,0,0,4,1],[839,11,839,19,0,0,4,0],[839,11,839,19,0,0,4,1],[844,14,844,26,0,0,4,0],[844,14,844,26,0,0,4,1],[847,11,847,32,0,0,4,1],[858,7,858,12,0,0,4,0],[858,7,858,12,0,0,4,1]],"finish_pass_gather":[[1114,16,1114,41,0,0,4,0],[1114,16,1114,41,0,0,4,1],[1118,9,1118,23,0,0,4,0],[1118,9,1118,23,0,0,4,1],[1120,11,1120,27,0,0,4,1],[1125,9,1125,23,0,0,4,0],[1125,9,1125,23,0,0,4,1],[1127,11,1127,27,0,0,4,1]],"tj3SaveImage12":[[511,7,511,16,0,0,4,1],[511,20,511,27,0,0,4,1],[511,31,511,40,0,0,4,1],[511,44,511,53,0,0,4,1],[511,57,511,67,0,0,4,1],[512,7,512,22,0,0,4,1],[512,26,512,49,0,0,4,1],[518,7,518,53,0,0,4,1],[526,7,526,45,0,0,4,1],[539,7,539,10,0,0,4,0],[539,14,539,38,0,0,4,1],[546,14,546,17,0,0,4,0],[546,21,546,45,0,0,4,1],[572,9,572,47,0,0,4,1],[576,9,576,48,0,0,4,1],[587,7,587,17,0,0,4,1],[589,10,589,55,0,0,4,0],[589,10,589,55,0,0,4,1],[592,9,592,15,0,0,4,1],[607,7,607,11,0,0,4,0],[340,7,340,12,2,0,4,1]]},"library_calls":["tj3InitVersion","tj3Set","tj3Set","tj3Set","tj3Set","tj3Set","tj3Set","tj3Alloc","tj3Destroy","tj3Compress8","tj3SetCroppingRegion","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tj3SaveImage8","tj3Alloc","tj3SaveImage12","tj3Free","tjFree","tjInitCompress","tjCompress2","tjFree","tjDestroy","tj3GetErrorStr","tj3Free","tj3Free","tj3Destroy"],"critical_calls":["tj3InitVersion","tj3Set","tj3Set","tj3Set","tj3Set","tj3Set","tj3Set","tj3Alloc","tj3Compress8","tj3SetCroppingRegion","tjBufSizeYUV2","tjAlloc","tjEncodeYUV2","tj3SaveImage8","tj3Alloc","tj3SaveImage12","tj3Free","tjFree","tjInitCompress","tjCompress2","tjFree","tjDestroy","tj3GetErrorStr","tj3Free","tj3Free","tj3Destroy"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create a handle for compression operations
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
	return 0;
	}
	

    // Set various parameters using tj3Set
    tj3Set(handle, TJPARAM_QUALITY, 85);
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    tj3Set(handle, TJPARAM_ARITHMETIC, 0);
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);
    tj3Set(handle, TJPARAM_OPTIMIZE, 1);

    // Define image dimensions
    const int width = 64;
    const int height = 64;
    const int pixelFormat = TJPF_RGB;
    const int pitch = width * tjPixelSize[pixelFormat];

    // Allocate source image buffer
    unsigned char* srcBuf = (unsigned char*)tj3Alloc(pitch * height);
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Fill source buffer with fuzz data (with bounds checking)
    size_t srcSize = pitch * height;
    size_t copySize = size < srcSize ? size : srcSize;
    memcpy(srcBuf, data, copySize);

    // Compress using tj3Compress8
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = 0;
    int result = tj3Compress8(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
    
    // Check for compression success
    if (result == 0 && jpegBuf && jpegSize > 0) {
        // Set cropping region
        tjregion cropRegion;
        cropRegion.x = 10;
        cropRegion.y = 10;
        cropRegion.w = width - 20;
        cropRegion.h = height - 20;
        tj3SetCroppingRegion(handle, cropRegion);

        // Create YUV buffer and encode using tjEncodeYUV2
        int yuvSize = tjBufSizeYUV2(width, 4, height, TJSAMP_420);
        unsigned char* yuvBuf = (unsigned char*)tjAlloc(yuvSize);
        if (yuvBuf) {
            // Use the old API tjEncodeYUV2
            result = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, 
                                  yuvBuf, TJSAMP_420, 0);
            
            // Save the YUV data as an image file (note: this will create actual files)
            if (result == 0) {
                // For 8-bit image
                tj3SaveImage8(handle, "output_8bit.ppm", srcBuf, width, pitch, height, pixelFormat);
                
                // For 12-bit image (create dummy 12-bit data)
                short* srcBuf12 = (short*)tj3Alloc(pitch * height * sizeof(short));
                if (srcBuf12) {
                    // Convert 8-bit to 12-bit by scaling
                    for (size_t i = 0; i < pitch * height; i++) {
                        srcBuf12[i] = (short)(srcBuf[i] << 4);
                    }
                    tj3SaveImage12(handle, "output_12bit.pgm", srcBuf12, width, pitch, height, TJPF_GRAY);
                    tj3Free(srcBuf12);
                }
            }
            tjFree(yuvBuf);
        }

        // Also compress using the old API tjCompress2
        unsigned char* jpegBuf2 = nullptr;
        unsigned long jpegSize2 = 0;
        tjhandle handle2 = tjInitCompress();
        if (handle2) {
            result = tjCompress2(handle2, srcBuf, width, pitch, height, pixelFormat,
                                &jpegBuf2, &jpegSize2, TJSAMP_420, 85, 0);
            if (jpegBuf2) {
                tjFree(jpegBuf2);
            }
            tjDestroy(handle2);
        }

        // Get error string (even if no error, to demonstrate usage)
        const char* errorStr = tj3GetErrorStr(handle);
        // Can check errorStr if needed

        // Free JPEG buffer
        tj3Free(jpegBuf);
    }

    // Cleanup
    tj3Free(srcBuf);
    tj3Destroy(handle);

    return 0;
}