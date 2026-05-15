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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 18
//<Quality> {"density":24,"unique_branches":{"cmsGetTagSignature":[[653,9,653,26,0,0,4,1],[654,9,654,27,0,0,4,1]],"IsMyFile":[[2545,8,2545,11,0,0,4,0],[2545,8,2545,11,0,0,4,1],[2552,8,2552,23,0,0,4,1]],"cmsPluginTHR":[[551,10,551,24,0,0,4,0],[554,17,554,56,0,0,4,0],[554,17,554,56,0,0,4,1],[559,17,559,56,0,0,4,0]],"cmsIT8LoadFromFile":[[2620,10,2620,19,0,0,4,0],[2620,10,2620,19,0,0,4,1],[2624,10,2624,15,0,0,4,1],[2629,10,2629,37,0,0,4,0],[2629,10,2629,37,0,0,4,1],[2638,9,2638,31,0,0,4,0],[2638,9,2638,31,0,0,4,1],[2648,9,2648,47,0,0,4,1]],"IsMyBlock":[[2504,9,2504,15,0,0,4,0]]},"library_calls":["cmsOpenProfileFromMem","cmsGetTagSignature","cmsCloseProfile","cmsIT8LoadFromFile","cmsGetProfileContextID","cmsIT8EnumDataFormat","cmsD50_XYZ","cmsBuildGamma","cmsGetProfileContextID","cmsDupToneCurve","cmsAllocNamedColorList","cmsGetProfileContextID","cmsAppendNamedColor","cmsNamedColorCount","cmsPipelineAlloc","cmsGetProfileContextID","cmsStageAllocIdentity","cmsGetProfileContextID","cmsPipelineInsertStage","cmsPipelineDup","cmsPluginTHR","cmsGetProfileContextID","cmsPipelineFree","cmsPipelineFree","cmsFreeNamedColorList","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromMem","cmsGetTagSignature","cmsIT8LoadFromFile","cmsGetProfileContextID","cmsIT8EnumDataFormat","cmsD50_XYZ","cmsBuildGamma","cmsGetProfileContextID","cmsDupToneCurve","cmsAllocNamedColorList","cmsGetProfileContextID","cmsAppendNamedColor","cmsNamedColorCount","cmsPipelineAlloc","cmsGetProfileContextID","cmsStageAllocIdentity","cmsGetProfileContextID","cmsPipelineInsertStage","cmsPipelineDup","cmsPluginTHR","cmsGetProfileContextID","cmsPipelineFree","cmsPipelineFree","cmsFreeNamedColorList","cmsFreeToneCurve","cmsFreeToneCurve","cmsIT8Free","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // 1. Create a profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (!hProfile) {
	return 0;
	}
	
    
    // 2. Get tag signature (using cmsGetTagSignature)
    cmsTagSignature tagSig = cmsGetTagSignature(hProfile, fuzz_uint32_t_1);
    
    // 3. Create IT8 handle from file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsCloseProfile(hProfile);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Write to temp file for IT8 loading
    FILE *temp_file = fopen("temp_it8_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(cmsGetProfileContextID(hProfile), fuzz_str_2);
    
    // 4. Use cmsIT8EnumDataFormat
    char **sampleNames = nullptr;
    if (hIT8) {
        int formatCount = cmsIT8EnumDataFormat(hIT8, &sampleNames);
        // sampleNames is allocated by lcms, we don't need to free it
    }
    
    // 5. Get D50 white point (using cmsD50_XYZ)
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    
    // 6. Create and duplicate tone curve
    cmsToneCurve *curve = cmsBuildGamma(cmsGetProfileContextID(hProfile), 2.2);
    cmsToneCurve *dupCurve = nullptr;
    if (curve) {
        dupCurve = cmsDupToneCurve(curve);
    }
    
    // 7. Create named color list and get count
    cmsNAMEDCOLORLIST *namedColorList = cmsAllocNamedColorList(
        cmsGetProfileContextID(hProfile), 
        fuzz_uint32_t_5,  // n
        3,   // ColorantCount
        fuzz_str_3,
        fuzz_str_4
    );
    
    cmsUInt32Number colorCount = 0;
    if (namedColorList) {
        // Add some colors using data
        for (int i = 0; i < 3 && (i * 6 + 6) <= (int)size; i++) {
            cmsUInt16Number PCS[3] = {
                (cmsUInt16Number)(data[i*6] << 8 | data[i*6+1]),
                (cmsUInt16Number)(data[i*6+2] << 8 | data[i*6+3]),
                (cmsUInt16Number)(data[i*6+4] << 8 | data[i*6+5])
            };
            cmsUInt16Number Colorant[3] = {0, 0, 0};
            char name[20];
            snprintf(name, sizeof(name), "Color%d", i);
            cmsAppendNamedColor(namedColorList, name, PCS, Colorant);
        }
        colorCount = cmsNamedColorCount(namedColorList);
    }
    
    // 8. Create and duplicate pipeline
    cmsPipeline *pipeline = cmsPipelineAlloc(cmsGetProfileContextID(hProfile), fuzz_uint32_t_6, fuzz_uint32_t_7);
    cmsPipeline *dupPipeline = nullptr;
    if (pipeline) {
        // Add a simple identity stage
        cmsStage *stage = cmsStageAllocIdentity(cmsGetProfileContextID(hProfile), fuzz_uint32_t_8);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, stage);
        }
        dupPipeline = cmsPipelineDup(pipeline);
    }
    
    // 9. Install plugin (dummy plugin structure)
    struct DummyPlugin {
        cmsUInt32Number Magic;
        cmsUInt32Number ExpectedVersion;
        cmsUInt32Number Type;
        cmsUInt32Number Flags;
        cmsUInt32Number Reserved[4];
    };
    
    DummyPlugin dummyPlugin = {
        0x6163706C,  // 'acpl' magic
        0x02100000,  // LCMS_VERSION
        0,           // Type
        0,           // Flags
        {0, 0, 0, 0}
    };
    
    // Use some data for plugin parameters
    if (size >= sizeof(DummyPlugin)) {
        memcpy(&dummyPlugin, data, sizeof(DummyPlugin) > size ? size : sizeof(DummyPlugin));
    }
    
    cmsBool pluginResult = cmsPluginTHR(cmsGetProfileContextID(hProfile), &dummyPlugin);
    
    // Cleanup
    if (dupPipeline) cmsPipelineFree(dupPipeline);
    if (pipeline) cmsPipelineFree(pipeline);
    
    if (namedColorList) cmsFreeNamedColorList(namedColorList);
    
    if (dupCurve) cmsFreeToneCurve(dupCurve);
    if (curve) cmsFreeToneCurve(curve);
    
    if (hIT8) cmsIT8Free(hIT8);
    
    assert_file_closed(&in_file);;
    remove("temp_it8_file");
    
    cmsCloseProfile(hProfile);
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	return 0;
}