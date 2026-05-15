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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 1071, nr_unique_branch: 252
//<Quality> {"density":21,"unique_branches":{"j12init_d_diff_controller":[[356,7,356,46,0,0,4,1],[357,7,357,50,0,0,4,1],[369,44,369,70,0,0,4,0],[369,44,369,70,0,0,4,1],[383,7,383,23,0,0,4,1]],"decompress_smooth_data":[[754,43,754,60,0,0,4,1],[769,43,769,61,0,0,4,1],[784,43,784,61,0,0,4,1]],"use_merged_upsample":[[42,7,42,30,0,0,4,0]],"j12init_d_post_controller":[[271,7,271,30,0,0,4,0],[275,9,275,48,0,0,4,1],[276,9,276,52,0,0,4,1]],"jpeg_undifference6":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"save_marker":[[772,9,772,20,0,0,4,1],[821,7,821,25,0,0,4,1]],"master_selection":[[528,7,528,30,0,0,4,0],[626,18,626,45,0,0,4,1],[650,7,650,30,0,0,4,0],[655,9,655,35,0,0,4,0],[655,9,655,35,0,0,4,1],[657,14,657,41,0,0,4,0],[657,14,657,41,0,0,4,1],[662,9,662,26,0,0,4,1],[669,20,669,55,0,0,4,1],[670,20,670,41,0,0,4,1],[671,9,671,35,0,0,4,0],[671,9,671,35,0,0,4,1],[673,14,673,41,0,0,4,0],[673,14,673,41,0,0,4,1],[723,14,723,41,0,0,4,1]],"jsimd_set_idct_islow":[[1159,7,1159,19,0,0,4,1],[1163,7,1163,49,0,0,4,0],[46,30,46,50,8,0,4,0]],"j12init_d_main_controller":[[435,7,435,30,0,0,4,0],[439,9,439,48,0,0,4,1],[440,9,440,52,0,0,4,1],[463,7,463,41,0,0,4,1]],"jpeg_undifference5":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"jpeg_undifference1":[[64,10,64,17,1,0,4,0],[64,10,64,17,1,0,4,1]],"jpeg_undifference2":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"realize_virt_arrays":[[707,24,707,43,0,0,4,0]],"jinit_lossless_decompressor":[[285,7,285,46,0,0,4,1],[285,50,285,75,0,0,4,1]],"jinit_lhuff_decoder":[[297,15,297,32,0,0,4,0],[297,15,297,32,0,0,4,1]],"simple_upscale":[[224,12,224,19,0,0,4,0],[224,12,224,19,0,0,4,1]],"j16init_upsampler":[[446,7,446,30,0,0,4,0],[450,9,450,48,0,0,4,1],[451,9,451,52,0,0,4,1],[461,7,461,47,0,0,4,0],[472,7,472,30,0,0,4,1],[478,14,478,40,0,0,4,0],[478,44,478,75,0,0,4,1],[483,44,483,70,0,0,4,0],[483,44,483,70,0,0,4,1],[496,9,496,35,0,0,4,1],[500,16,500,41,0,0,4,0],[500,45,500,70,0,0,4,0],[558,9,558,20,0,0,4,1]],"jpeg_undifference_first_row":[[188,3,188,9,0,0,4,0],[188,3,188,9,0,0,4,1],[191,3,191,9,0,0,4,0],[191,3,191,9,0,0,4,1],[194,3,194,9,0,0,4,0],[194,3,194,9,0,0,4,1],[197,3,197,9,0,0,4,0],[197,3,197,9,0,0,4,1],[200,3,200,9,0,0,4,0],[200,3,200,9,0,0,4,1],[203,3,203,9,0,0,4,0],[203,3,203,9,0,0,4,1],[206,3,206,9,0,0,4,0],[206,3,206,9,0,0,4,1],[209,3,209,10,0,0,4,1],[64,10,64,17,1,0,4,0],[64,10,64,17,1,0,4,1]],"jpeg_make_d_derived_tbl":[[257,29,257,52,0,0,4,0]],"jinit_d_diff_controller":[[354,7,354,46,0,0,4,1],[354,50,354,75,0,0,4,1],[369,44,369,70,0,0,4,0],[369,44,369,70,0,0,4,1],[383,7,383,23,0,0,4,1]],"decode_mcu_AC_first":[[415,15,415,22,0,0,4,0]],"j16init_lossless_decompressor":[[287,7,287,46,0,0,4,1],[288,7,288,50,0,0,4,1]],"prepare_range_limit_table":[[448,14,448,41,0,0,4,1],[484,17,484,34,0,0,4,0],[484,17,484,34,0,0,4,1],[488,31,488,57,0,0,4,0],[488,31,488,57,0,0,4,1]],"j16init_d_diff_controller":[[356,7,356,46,0,0,4,1],[357,7,357,50,0,0,4,1],[369,44,369,70,0,0,4,0],[369,44,369,70,0,0,4,1],[383,7,383,23,0,0,4,1]],"jpeg_read_scanlines":[[329,7,329,30,0,0,4,0],[331,9,331,48,0,0,4,0],[331,9,331,48,0,0,4,1],[331,52,331,77,0,0,4,1]],"decode_mcu_fast":[[401,9,401,16,26,0,4,0],[401,9,401,16,27,0,4,0]],"jpeg_undifference4":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"jsimd_set_h2v1_upsample":[[458,7,458,23,0,0,4,1],[462,7,462,47,0,0,4,0]],"jinit_upsampler":[[446,7,446,30,0,0,4,0],[448,9,448,48,0,0,4,1],[448,52,448,77,0,0,4,1],[478,44,478,75,0,0,4,1],[506,23,506,53,0,0,4,1],[515,13,515,43,0,0,4,0]],"jpeg_core_output_dimensions":[[108,7,108,31,0,0,4,1],[166,16,166,68,0,0,4,0]],"j16init_d_main_controller":[[435,7,435,30,0,0,4,0],[439,9,439,48,0,0,4,1],[440,9,440,52,0,0,4,1],[457,7,457,23,0,0,4,1],[463,7,463,41,0,0,4,1],[472,44,472,70,0,0,4,0],[472,44,472,70,0,0,4,1]],"alloc_sarray":[[453,24,453,43,0,0,4,0],[499,14,499,34,0,0,4,1],[527,12,527,28,0,0,4,0],[527,12,527,28,0,0,4,1],[531,30,531,35,0,0,4,0],[531,30,531,35,0,0,4,1],[515,26,515,35,8,0,4,1]],"default_decompress_parms":[[177,32,177,42,0,0,4,0],[177,46,177,56,0,0,4,0]],"jpeg_undifference7":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"j12init_lossless_decompressor":[[287,7,287,46,0,0,4,1],[288,7,288,50,0,0,4,1]],"tj3SetScalingFactor":[[2045,9,2045,39,0,0,4,1]],"j12init_upsampler":[[446,7,446,30,0,0,4,0],[450,9,450,48,0,0,4,1],[451,9,451,52,0,0,4,1],[478,44,478,75,0,0,4,1],[534,11,534,19,0,0,4,1]],"j16init_d_post_controller":[[271,7,271,30,0,0,4,0],[275,9,275,48,0,0,4,1],[276,9,276,52,0,0,4,1],[295,7,295,29,0,0,4,1]],"jinit_d_post_controller":[[271,7,271,30,0,0,4,0],[273,9,273,48,0,0,4,1],[273,52,273,77,0,0,4,1]],"start_pass_lhuff_decoder":[[89,16,89,41,0,0,4,0],[89,16,89,41,0,0,4,1],[93,9,93,18,0,0,4,1],[93,22,93,44,0,0,4,1],[94,9,94,47,0,0,4,1],[103,29,103,57,0,0,4,0],[103,29,103,57,0,0,4,1],[106,23,106,52,0,0,4,0],[106,23,106,52,0,0,4,1],[111,25,111,53,0,0,4,0],[111,25,111,53,0,0,4,1]],"decode_mcu_AC_refine":[[553,15,553,21,0,0,4,0],[593,18,593,25,0,0,4,1]],"decode_mcus":[[216,18,216,49,0,0,4,0],[216,18,216,49,0,0,4,1],[231,7,231,37,0,0,4,0],[231,7,231,37,0,0,4,1],[232,20,232,51,0,0,4,0],[232,20,232,51,0,0,4,1],[246,23,246,37,0,0,4,0],[246,23,246,37,0,0,4,1],[249,23,249,51,0,0,4,0],[249,23,249,51,0,0,4,1],[255,13,255,14,0,0,4,0],[255,13,255,14,0,0,4,1],[256,15,256,22,0,0,4,1],[202,7,202,33,3,0,4,0],[202,7,202,33,3,0,4,1],[203,9,203,64,3,0,4,1],[206,9,206,35,3,0,4,0],[206,9,206,35,3,0,4,1],[211,7,211,70,3,0,4,0],[211,7,211,70,3,0,4,1],[216,9,217,72,3,0,4,1],[161,7,161,26,4,0,4,0],[161,7,161,26,4,0,4,1],[162,9,162,70,4,0,4,1]],"tjDecompress2":[[2137,9,2137,25,0,0,4,1],[2142,35,2142,76,0,0,4,1],[2143,35,2143,77,0,0,4,1]],"jinit_color_deconverter":[[630,7,630,30,0,0,4,0],[632,9,632,48,0,0,4,1],[632,52,632,77,0,0,4,1],[716,9,716,32,0,0,4,0],[716,36,716,70,0,0,4,0],[716,36,716,70,0,0,4,1]],"jpeg_calc_output_dimensions":[[284,7,284,31,0,0,4,1]],"jpeg_undifference3":[[92,10,92,17,1,0,4,0],[92,10,92,17,1,0,4,1]],"j12init_color_deconverter":[[630,7,630,30,0,0,4,0],[634,9,634,48,0,0,4,1],[635,9,635,52,0,0,4,1],[716,9,716,32,0,0,4,0],[716,36,716,70,0,0,4,0],[716,36,716,70,0,0,4,1],[782,16,782,50,0,0,4,1]],"jinit_d_main_controller":[[435,7,435,30,0,0,4,0],[437,9,437,48,0,0,4,1],[437,52,437,77,0,0,4,1]],"start_pass":[[154,5,154,17,0,0,4,0],[157,7,157,22,0,0,4,0],[159,13,159,40,0,0,4,0],[168,7,168,22,0,0,4,1],[179,7,179,22,0,0,4,1],[189,7,189,14,0,0,4,1],[223,5,223,12,0,0,4,1]],"j16init_color_deconverter":[[630,7,630,30,0,0,4,0],[634,9,634,48,0,0,4,1],[635,9,635,52,0,0,4,1],[653,3,653,21,0,0,4,1],[658,3,658,15,0,0,4,0],[659,3,659,17,0,0,4,1],[660,9,660,35,0,0,4,1],[664,3,664,16,0,0,4,1],[665,3,665,16,0,0,4,1],[670,3,670,10,0,0,4,1],[684,3,684,21,0,0,4,1],[704,3,704,15,0,0,4,1],[705,3,705,19,0,0,4,0],[706,3,706,20,0,0,4,1],[707,3,707,19,0,0,4,1],[708,3,708,20,0,0,4,1],[709,3,709,20,0,0,4,1],[710,3,710,20,0,0,4,1],[711,3,711,20,0,0,4,1],[712,3,712,20,0,0,4,1],[713,3,713,20,0,0,4,1],[714,3,714,20,0,0,4,1],[716,9,716,32,0,0,4,0],[716,36,716,70,0,0,4,1],[720,9,720,45,0,0,4,1],[755,16,755,56,0,0,4,1],[782,16,782,50,0,0,4,0],[783,11,783,47,0,0,4,0],[784,11,784,49,0,0,4,0],[785,11,785,48,0,0,4,0],[786,11,786,53,0,0,4,0],[820,3,820,18,0,0,4,1],[857,3,857,16,0,0,4,1],[873,3,873,10,0,0,4,1],[883,7,883,29,0,0,4,1]]},"library_calls":["tjInitDecompress","tjDecompressHeader","tjDestroy","tjDecompressHeader2","tjDestroy","tjDestroy","tjBufSizeYUV","tjDestroy","tjPlaneWidth","tjPlaneHeight","tjAlloc","tjFree","tjDestroy","tj3GetScalingFactors","tjAlloc","tjFree","tjDestroy","tjDecompress","tjFree","tjFree","tjDestroy","tjEncodeYUVPlanes","tjFree","tjFree","tjDestroy"],"critical_calls":["tjInitDecompress","tjDecompressHeader","tjDecompressHeader2","tjBufSizeYUV","tjPlaneWidth","tjPlaneHeight","tjAlloc","tj3GetScalingFactors","tjAlloc","tjDecompress","tjEncodeYUVPlanes","tjFree","tjFree","tjDestroy"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
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
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Too small for meaningful JPEG data
    
    tjhandle handle = nullptr;
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned char *jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    unsigned char *yuvPlanes[3] = {nullptr, nullptr, nullptr};
    int strides[3] = {0, 0, 0};
    unsigned char *rgbBuf = nullptr;
    int pixelSize = 3;  // RGB
    int flags = 0;
    
    // 1. Initialize decompressor
    handle = tjInitDecompress();
    if (!handle) {
	return 0;
	}
	
    
    // 2. Get JPEG header information using two different header functions
    int width1 = 0, height1 = 0;
    if (tjDecompressHeader(handle, (unsigned char*)data, (unsigned long)size, &width1, &height1) != 0) {
        tjDestroy(handle);
        return 0;
    }
    
    int width2 = 0, height2 = 0, subsamp = 0;
    if (tjDecompressHeader2(handle, (unsigned char*)data, (unsigned long)size, &width2, &height2, &subsamp) != 0) {
        tjDestroy(handle);
        return 0;
    }
    
    // Use the dimensions from the second header call
    width = width2;
    height = height2;
    jpegSubsamp = subsamp;
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        tjDestroy(handle);
        return 0;
    }
    
    // 3. Calculate YUV buffer size
    unsigned long yuvBufSize = tjBufSizeYUV(fuzz_int32_t_1, fuzz_int32_t_2, fuzz_int32_t_3);
    if (yuvBufSize == 0 || yuvBufSize > 100 * 1024 * 1024) {  // Limit to 100MB
        tjDestroy(handle);
        return 0;
    }
    
    // 4. Allocate YUV planes
    for (int i = 0; i < 3; i++) {
        int planeWidth = tjPlaneWidth(fuzz_int32_t_4, width, fuzz_int32_t_5);
        int planeHeight = tjPlaneHeight(fuzz_int32_t_6, height, fuzz_int32_t_7);
        if (planeWidth > 0 && planeHeight > 0) {
            strides[i] = planeWidth;
            yuvPlanes[i] = (unsigned char*)tjAlloc(planeWidth * planeHeight);
            if (!yuvPlanes[i]) {
                for (int j = 0; j < i; j++) tjFree(yuvPlanes[j]);
                tjDestroy(handle);
                return 0;
            }
        }
    }
    
    // 5. Get scaling factors (not used directly but API is called)
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    (void)scalingFactors;  // Suppress unused variable warning
    (void)numScalingFactors;
    
    // 6. Decompress JPEG to RGB
    int pitch = width * pixelSize;
    rgbBuf = (unsigned char*)tjAlloc(pitch * height);
    if (!rgbBuf) {
        for (int i = 0; i < 3; i++) tjFree(yuvPlanes[i]);
        tjDestroy(handle);
        return 0;
    }
    
    if (tjDecompress(handle, (unsigned char*)data, (unsigned long)size, 
                     rgbBuf, fuzz_int32_t_8, pitch, fuzz_int32_t_9, fuzz_int32_t_10, fuzz_int32_t_11) != 0) {
        tjFree(rgbBuf);
        for (int i = 0; i < 3; i++) tjFree(yuvPlanes[i]);
        tjDestroy(handle);
        return 0;
    }
    
    // 7. Encode RGB to YUV planes
    if (tjEncodeYUVPlanes(handle, rgbBuf, fuzz_int32_t_12, pitch, fuzz_int32_t_13, 
                          TJPF_RGB, yuvPlanes, strides, fuzz_int32_t_14, fuzz_int32_t_15) != 0) {
        // Clean up and continue (don't fail on encode error)
    }
    
    // 8. Clean up
    tjFree(rgbBuf);
    for (int i = 0; i < 3; i++) {
        if (yuvPlanes[i]) tjFree(yuvPlanes[i]);
    }
    tjDestroy(handle);
    
    return 0;
}