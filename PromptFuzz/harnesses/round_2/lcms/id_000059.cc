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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"SaveTags":[[1449,13,1449,34,0,0,4,0],[1451,17,1451,62,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsSetAlarmCodes","cmsOpenProfileFromStream","cmsCreateProfilePlaceholder","cmsDeleteContext","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorCount","cmsFreeNamedColorList","cmsBuildTabulatedToneCurve16","cmsSmoothToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineOutputChannels","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsOpenIOhandlerFromStream","cmsSaveProfileToFile","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetAlarmCodes","cmsOpenProfileFromStream","cmsCreateProfilePlaceholder","cmsAllocNamedColorList","cmsAppendNamedColor","cmsNamedColorCount","cmsFreeNamedColorList","cmsBuildTabulatedToneCurve16","cmsSmoothToneCurve","cmsFreeToneCurve","cmsWriteRawTag","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineOutputChannels","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsOpenIOhandlerFromStream","cmsSaveProfileToFile","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 128) {
	return 0;
	}
	 // Need minimum data
    
    // Create context
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set alarm codes (first 16 uint16s from data)
    cmsUInt16Number alarmCodes[16];
    size_t alarmBytes = sizeof(cmsUInt16Number) * 16;
    if (size >= alarmBytes) {
        memcpy(alarmCodes, data, alarmBytes);
        cmsSetAlarmCodes(alarmCodes);
    }
    
    // Create a profile from input stream
    cmsHPROFILE profile = cmsOpenProfileFromStream(in_file, "r");
    if (!profile) {
        // Create fallback profile if input isn't valid
        profile = cmsCreateProfilePlaceholder(ctx);
        if (!profile) {
            assert_file_closed(&in_file);;
            cmsDeleteContext(ctx);
            assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // 2. Create and use named color list
    cmsNAMEDCOLORLIST* namedColors = cmsAllocNamedColorList(ctx, 10, 3, "Prefix", "Suffix");
    if (namedColors) {
        // Add some colors using data
        for (int i = 0; i < 5 && (i * 12 + 50) < size; i++) {
            char name[20];
            snprintf(name, sizeof(name), "Color%d", i);
            cmsUInt16Number pcs[3] = {0, 0, 0};
            cmsUInt16Number colorant[3] = {0, 0, 0};
            
            if (i * 12 + 62 < size) {
                memcpy(pcs, data + 50 + i * 12, 6);
                memcpy(colorant, data + 56 + i * 12, 6);
            }
            
            // Add named color (we ignore return for fuzzing)
            cmsAppendNamedColor(namedColors, name, pcs, colorant);
        }
        
        // Get named color count (API 2)
        cmsUInt32Number colorCount = cmsNamedColorCount(namedColors);
        (void)colorCount; // Use to avoid unused warning
        
        cmsFreeNamedColorList(namedColors);
    }
    
    // 3. Create and smooth tone curve
    cmsUInt16Number curveValues[256];
    size_t curveDataSize = sizeof(cmsUInt16Number) * 256;
    if (100 + curveDataSize < size) {
        memcpy(curveValues, data + 100, curveDataSize);
        cmsToneCurve* toneCurve = cmsBuildTabulatedToneCurve16(ctx, 256, curveValues);
        if (toneCurve) {
            // Smooth tone curve (API 3)
            cmsSmoothToneCurve(toneCurve, 1.0);
            cmsFreeToneCurve(toneCurve);
        }
    }
    
    // 4. Write raw tag to profile (API 8)
    if (size > 200) {
        cmsWriteRawTag(profile, cmsSigMediaBlackPointTag, data + 200, 
                      size - 200 < 100 ? size - 200 : 100);
    }
    
    // 5. Create pipeline and CLUT stage
    cmsPipeline* pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        // Create CLUT points
        cmsUInt32Number clutPoints[3] = {2, 2, 2};
        
        // Prepare CLUT table from fuzzer data
        size_t tableSize = 2 * 2 * 2 * 3 * sizeof(cmsFloat32Number);
        cmsFloat32Number* table = nullptr;
        if (300 + tableSize < size) {
            table = (cmsFloat32Number*)(data + 300);
            
            // Allocate CLUT stage (API 5)
            cmsStage* clutStage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, 3, 3, table);
            if (clutStage) {
                // Add stage to pipeline
                cmsPipelineInsertStage(pipeline, cmsAT_END, clutStage);
                
                // 6. Get output channels (API 6)
                cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
                (void)outputChannels;
                
                // 7. Unlink stage (API 7)
                cmsPipelineUnlinkStage(pipeline, cmsAT_END, &clutStage);
                
                // Free the unlinked stage
                if (clutStage) {
                    cmsStageFree(clutStage);
                }
            }
        }
        
        cmsPipelineFree(pipeline);
    }
    
    // 4. Save profile to file (API 4)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Create a file handler for saving
        cmsIOHANDLER* ioHandler = cmsOpenIOhandlerFromStream(ctx, out_file);
        if (ioHandler) {
            // Use the save function - note: cmsSaveProfileToFileTHR would be better but we have cmsSaveProfileToFile
            cmsSaveProfileToFile(profile, "output_file");
            cmsCloseIOhandler(ioHandler);
        }
        assert_file_closed(&out_file);;
    }
    
    // Cleanup
    if (profile) cmsCloseProfile(profile);
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}