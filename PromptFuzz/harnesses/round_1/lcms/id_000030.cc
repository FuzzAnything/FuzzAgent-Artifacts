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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 67
//<Quality> {"density":6,"unique_branches":{"cmsDetectDestinationBlackPoint":[[433,9,433,57,0,0,4,1],[450,9,450,59,0,0,4,1],[451,10,451,38,0,0,4,0],[452,10,452,37,0,0,4,0],[453,10,453,38,0,0,4,1],[463,9,463,47,0,0,4,1],[489,9,489,27,0,0,4,0],[489,9,489,27,0,0,4,1],[493,15,493,22,0,0,4,0],[493,15,493,22,0,0,4,1],[506,19,506,24,0,0,4,0],[506,19,506,24,0,0,4,1],[511,9,511,38,0,0,4,0],[511,9,511,38,0,0,4,1],[522,9,522,47,0,0,4,1],[546,15,546,22,0,0,4,0],[546,15,546,22,0,0,4,1],[552,9,552,47,0,0,4,1],[565,15,565,22,0,0,4,0],[565,15,565,22,0,0,4,1],[569,13,569,21,0,0,4,0],[569,13,569,21,0,0,4,1],[569,25,569,32,0,0,4,0],[569,25,569,32,0,0,4,1],[578,9,578,14,0,0,4,0],[578,9,578,14,0,0,4,1],[588,9,588,20,0,0,4,1],[30,23,30,34,18,0,4,1],[30,31,30,32,18,0,4,1],[30,44,30,45,18,0,4,1],[30,23,30,34,19,0,4,1],[30,31,30,32,19,0,4,1],[30,44,30,45,19,0,4,1],[30,23,30,34,20,0,4,0],[30,23,30,34,20,0,4,1]],"RootOfLeastSquaresFitQuadraticCurve":[[340,9,340,14,0,0,4,0],[340,9,340,14,0,0,4,1],[342,15,342,20,0,0,4,0],[342,15,342,20,0,0,4,1],[363,9,363,37,0,0,4,1],[370,9,370,26,0,0,4,0],[370,9,370,26,0,0,4,1],[372,13,372,30,0,0,4,0],[372,13,372,30,0,0,4,1],[378,14,378,20,0,0,4,0],[378,14,378,20,0,0,4,1],[385,18,385,35,0,0,4,1],[31,23,31,34,1,0,4,0],[31,23,31,34,1,0,4,1],[31,31,31,32,1,0,4,0],[31,31,31,32,1,0,4,1],[31,44,31,45,1,0,4,0],[31,44,31,45,1,0,4,1],[31,23,31,34,2,0,4,0],[31,23,31,34,2,0,4,1],[31,31,31,32,2,0,4,0],[31,31,31,32,2,0,4,1],[31,44,31,45,2,0,4,0],[31,44,31,45,2,0,4,1]],"cmsPipelineUnlinkStage":[[1563,9,1563,31,0,0,4,1],[1571,9,1571,25,0,0,4,0],[1582,9,1582,23,0,0,4,1],[1599,9,1599,16,0,0,4,1],[1602,9,1602,12,0,0,4,0]],"Type_LUT8_Read":[[2023,9,2023,27,0,0,4,0],[2068,13,2068,50,0,0,4,0],[2096,9,2096,23,0,0,4,0]]},"library_calls":["cmsOpenProfileFromFile","cmsGetHeaderModel","cmsSetHeaderAttributes","cmsDetectDestinationBlackPoint","cmsSetDeviceClass","cmsCIECAM02Init","cmsIT8Alloc","cmsIT8FindDataFormat","cmsIT8SetData","cmsIT8SetData","cmsMLUalloc","cmsMLUgetASCII","cmsMLUfree","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromFile","cmsGetHeaderModel","cmsSetHeaderAttributes","cmsDetectDestinationBlackPoint","cmsSetDeviceClass","cmsCIECAM02Init","cmsIT8Alloc","cmsIT8FindDataFormat","cmsIT8SetData","cmsIT8SetData","cmsMLUalloc","cmsMLUgetASCII","cmsMLUfree","cmsPipelineAlloc","cmsStageAllocCLutFloatGranular","cmsPipelineInsertStage","cmsPipelineUnlinkStage","cmsStageFree","cmsPipelineFree","cmsIT8Free","cmsCIECAM02Done","cmsCloseProfile"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data
    
    // 1. Create profile from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open profile from file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 2. Get header model
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);
    
    // 3. Set header attributes
    cmsSetHeaderAttributes(hProfile, 0x12345678);
    
    // 4. Detect destination black point
    cmsCIEXYZ blackPoint;
    cmsBool blackPointResult = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, 0, 0);
    
    // 5. Set device class
    cmsSetDeviceClass(hProfile, cmsSigDisplayClass);
    
    // 6. Create viewing conditions for CIECAM02
    cmsViewingConditions vc = {
        {0.9642, 1.0, 0.8249}, // D50 white point
        20.0,   // Yb
        100.0,  // La
        1,      // surround
        1.0     // D_value
    };
    
    // 7. Initialize CIECAM02
    cmsHANDLE hCam = cmsCIECAM02Init(0, &vc);
    
    // 8. Create IT8 handle and use data format APIs
    cmsHANDLE hIT8 = cmsIT8Alloc(0);
    if (hIT8) {
        // Find data format
        int formatIndex = cmsIT8FindDataFormat(hIT8, "Sample");
        
        // Set some data in IT8
        cmsIT8SetData(hIT8, "Patch1", "Sample1", "Value1");
        cmsIT8SetData(hIT8, "Patch2", "Sample2", "Value2");
        
        // Get ASCII from MLU (need to create MLU first)
        cmsMLU *mlu = cmsMLUalloc(0, 5);
        if (mlu) {
            char buffer[256];
            cmsUInt32Number bytes = cmsMLUgetASCII(mlu, "en", "US", buffer, sizeof(buffer));
            cmsMLUfree(mlu);
        }
    }
    
    // 9. Create a simple pipeline and unlink a stage
    cmsPipeline *pipeline = cmsPipelineAlloc(0, 3, 3);
    if (pipeline) {
        // Create a CLUT stage
        cmsUInt32Number gridPoints[] = {5, 5, 5};
        cmsStage *clutStage = cmsStageAllocCLutFloatGranular(0, gridPoints, 3, 3, NULL);
        
        if (clutStage) {
            // Insert the stage
            cmsPipelineInsertStage(pipeline, cmsAT_BEGIN, clutStage);
            
            // Now unlink it
            cmsStage *unlinkedStage = NULL;
            cmsPipelineUnlinkStage(pipeline, cmsAT_BEGIN, &unlinkedStage);
            
            // Free the unlinked stage
            if (unlinkedStage) {
                cmsStageFree(unlinkedStage);
            }
        }
        
        // Free pipeline
        cmsPipelineFree(pipeline);
    }
    
    // Clean up
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    if (hCam) {
        cmsCIECAM02Done(hCam);
    }
    
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}