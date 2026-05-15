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
#include <lcms2.h>
}
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 149.5, nr_unique_branch: 88
//<Quality> {"density":13,"unique_branches":{"OutputValueSampler":[[569,9,569,23,0,0,4,0],[571,13,571,28,0,0,4,0],[571,13,571,28,0,0,4,1],[573,18,573,33,0,0,4,0],[573,18,573,33,0,0,4,1],[573,37,573,52,0,0,4,0],[573,37,573,52,0,0,4,1],[574,18,574,33,0,0,4,0],[574,18,574,33,0,0,4,1],[574,37,574,52,0,0,4,0],[574,37,574,52,0,0,4,1],[580,21,580,86,0,0,4,1],[583,27,583,39,0,0,4,0],[583,27,583,39,0,0,4,1]],"WriteOutputLUT":[[1284,9,1284,21,0,0,4,1],[1294,9,1294,63,0,0,4,1],[1307,9,1307,22,0,0,4,0],[1307,9,1307,22,0,0,4,1],[1315,9,1315,27,0,0,4,1],[1323,9,1323,114,0,0,4,1],[1347,9,1347,47,0,0,4,1],[1353,9,1353,22,0,0,4,0],[1354,13,1354,46,0,0,4,1],[1366,15,1366,28,0,0,4,0],[1366,15,1366,28,0,0,4,1],[1375,9,1375,51,0,0,4,0]],"cmsGetPostScriptCRD":[[1561,9,1561,23,0,0,4,1],[1566,9,1566,13,0,0,4,1]],"_cmsReadDevicelinkLUT":[[721,9,721,61,0,0,4,0],[725,13,725,23,0,0,4,0]],"EmitHeader":[[347,9,347,28,0,0,4,0],[347,9,347,28,0,0,4,1],[348,9,348,26,0,0,4,0],[348,9,348,26,0,0,4,1]],"_cmsReadOutputLUT":[[649,9,649,53,0,0,4,0]],"cmsGetPostScriptColorResource":[[1537,9,1537,32,0,0,4,1],[1542,9,1542,32,0,0,4,0]],"WriteNamedColorCRD":[[1429,9,1429,22,0,0,4,0]],"TrilinearInterp16":[[583,16,583,35,0,0,4,0],[586,16,586,35,0,0,4,0]],"_cmsIOPrintf":[[505,13,505,24,0,0,4,0]],"BuildGrayOutputPipeline":[[418,9,418,24,0,0,4,0]],"cmsMLUgetASCII":[[558,9,558,21,0,0,4,0]],"PatchLUT":[[519,17,519,33,0,0,4,0]],"MemoryWrite":[[215,9,215,18,0,0,4,0]],"EmitPQRStage":[[1168,13,1168,24,0,0,4,1],[1200,13,1200,19,0,0,4,0]],"GenerateCRD":[[1485,9,1485,51,0,0,4,0],[1492,9,1492,61,0,0,4,0],[1492,9,1492,61,0,0,4,1],[1494,13,1494,64,0,0,4,0],[1502,13,1502,60,0,0,4,0],[1502,13,1502,60,0,0,4,1],[1507,9,1507,51,0,0,4,0]],"OptimizeMatrixShaper":[[1691,45,1691,75,0,0,4,0]],"cmsChannelsOfColorSpace":[[881,5,881,24,0,0,4,0],[883,5,883,24,0,0,4,0],[885,5,885,24,0,0,4,0],[886,5,886,26,0,0,4,0],[892,5,892,23,0,0,4,0],[897,5,897,24,0,0,4,0],[898,5,898,26,0,0,4,0],[902,5,902,24,0,0,4,0],[903,5,903,26,0,0,4,0],[905,5,905,24,0,0,4,0],[906,5,906,26,0,0,4,0],[908,5,908,24,0,0,4,0],[909,5,909,26,0,0,4,0],[911,5,911,24,0,0,4,0],[912,5,912,26,0,0,4,0],[914,5,914,24,0,0,4,0],[915,5,915,26,0,0,4,0],[918,5,918,26,0,0,4,0],[921,5,921,27,0,0,4,0],[923,5,923,24,0,0,4,0],[926,5,926,24,0,0,4,0],[927,5,927,27,0,0,4,0],[929,5,929,24,0,0,4,0],[930,5,930,27,0,0,4,0],[938,5,938,12,0,0,4,0]],"IsProperColorSpace":[[1074,9,1074,22,0,0,4,0]],"RemoveCR":[[325,23,325,26,0,0,4,0],[325,23,325,26,0,0,4,1],[326,17,326,28,0,0,4,0],[326,17,326,28,0,0,4,1],[326,32,326,43,0,0,4,1]],"DefaultICCintents":[[576,70,576,86,0,0,4,0]],"cmsFormatterForColorspaceOfProfile":[[4034,9,4034,25,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromStreamTHR","cmsDeleteContext","cmsGetColorSpace","cmsGetHeaderCreator","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSliceSpaceFloat","cmsGetPostScriptCRD","cmsLabEncoded2Float","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromStreamTHR","cmsGetColorSpace","cmsGetHeaderCreator","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineInputChannels","cmsPipelineOutputChannels","cmsSliceSpaceFloat","cmsGetPostScriptCRD","cmsLabEncoded2Float","cmsDeleteTransform","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


// Dummy sampler function for cmsSliceSpaceFloat
static int  id_000007_DummySampler(const cmsFloat32Number* in, cmsFloat32Number* out, void* cargo) {
    // Just copy input to output (simplified example)
    for (int i = 0; i < 3; i++) {
        out[i] = in[i];
    }
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegralArray(uint32_t, fuzz_uint32_t_array_4, fuzz_uint32_t_array_size_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_8, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
	
		const cmsUInt32Number * fuzzer_var_cmsSliceSpaceFloat_10_1 = fuzz_uint32_t_array_4;
		cmsUInt32Number fuzzer_size_cmsSliceSpaceFloat_10_1 = static_cast<cmsUInt32Number>(fuzz_uint32_t_array_size_4);
		cmsUInt16Number fuzzer_var_cmsLabEncoded2Float_12_1[3] = {fuzz_uint16_t_7,fuzz_uint16_t_8,fuzz_uint16_t_9};//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Create memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open profile from stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(ctx, in_file, fuzz_str_1);
    if (!hProfile) {
        assert_file_closed(&in_file);;
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get profile information using required APIs
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    
    // Create a simple transform to get a pipeline
    cmsHTRANSFORM hTransform = cmsCreateTransform(
        hProfile, 
        TYPE_Lab_16, 
        hProfile, 
        TYPE_Lab_16, 
        fuzz_uint32_t_2, 
        fuzz_uint32_t_3
    );
    
    cmsPipeline* pipeline = NULL;
    if (hTransform) {
        pipeline = cmsGetTransformPipeline(hTransform);
        
        if (pipeline) {
            // Use pipeline channel APIs
            cmsUInt32Number inputChannels = cmsPipelineInputChannels(pipeline);
            cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
            
            // Prepare for cmsSliceSpaceFloat
            cmsUInt32Number clutPoints[] = {5, 5, 5}; // 3D grid
            if (inputChannels >= 3) {
                // Use cmsSliceSpaceFloat with dummy sampler
                cmsSliceSpaceFloat(fuzzer_size_cmsSliceSpaceFloat_10_1, fuzzer_var_cmsSliceSpaceFloat_10_1,  id_000007_DummySampler, NULL);
            }
        }
    }
    
    // Get PostScript CRD
    cmsUInt32Number bufferSize = 4096;
    void* buffer = malloc(bufferSize);
    if (buffer) {
        cmsUInt32Number crdSize = cmsGetPostScriptCRD(
            ctx,
            hProfile,
            fuzz_uint32_t_5,
            fuzz_uint32_t_6,
            buffer,
            bufferSize
        );
        free(buffer);
    }
    
    // Use cmsLabEncoded2Float with sample data
    cmsUInt16Number encodedLab[3] = {32768, 32768, 32768}; // Midpoint values
    cmsCIELab lab;
    cmsLabEncoded2Float(&lab, fuzzer_var_cmsLabEncoded2Float_12_1);
    
    // Clean up
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}