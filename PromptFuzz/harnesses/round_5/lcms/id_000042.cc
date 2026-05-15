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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cmsDictGetEntryList":[[1194,9,1194,21,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsPipelineSetSaveAs8bitsFlag","cmsGetPostScriptCRD","cmsPipelineFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsCIE2000DeltaE","cmsGetTagSignature","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDeleteTransform","cmsDictGetEntryList","cmsDictNextEntry","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPlugins","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFileTHR","cmsOpenProfileFromStreamTHR","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsStageAllocMatrix","cmsStageFree","cmsPipelineSetSaveAs8bitsFlag","cmsGetPostScriptCRD","cmsPipelineFree","cmsSetHeaderRenderingIntent","cmsSetEncodedICCversion","cmsMLUalloc","cmsMLUdup","cmsMLUfree","cmsMLUfree","cmsCIE2000DeltaE","cmsGetTagSignature","cmsCreateLab4Profile","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDeleteTransform","cmsDictGetEntryList","cmsDictNextEntry","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsUnregisterPlugins","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file.icc", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // Write input data to temporary file
    FILE *tmp_file = fopen("input_file.icc", "wb");
    if (!tmp_file) {
        cmsDeleteContext(context);
        assert_file_closed(&tmp_file);
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;

    // Open profile from file
    cmsHPROFILE hProfileFile = cmsOpenProfileFromFileTHR(context, "input_file.icc", "r");
    
    // Also open from stream using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    cmsHPROFILE hProfileStream = nullptr;
    if (in_file) {
        hProfileStream = cmsOpenProfileFromStreamTHR(context, in_file, "r");
    }

    // Use the file-based profile for most operations
    cmsHPROFILE hProfile = hProfileFile ? hProfileFile : hProfileStream;
    
    if (hProfile) {
        // Detect destination black point
        cmsCIEXYZ blackPoint;
        if (cmsDetectDestinationBlackPoint(&blackPoint, hProfile, INTENT_PERCEPTUAL, 0)) {
            // Create pipeline with 3 input, 3 output channels (e.g., for RGB)
            cmsPipeline *pipeline = cmsPipelineAlloc(context, 3, 3);
            if (pipeline) {
                // Create identity matrix
                cmsFloat64Number matrix[9] = {1,0,0, 0,1,0, 0,0,1};
                cmsFloat64Number offset[3] = {0,0,0};
                
                // Add matrix stage
                cmsStage *matrixStage = cmsStageAllocMatrix(context, 3, 3, matrix, offset);
                if (matrixStage) {
                    // Pipeline operations would go here
                    cmsStageFree(matrixStage);
                }
                
                // Set pipeline flag
                cmsPipelineSetSaveAs8bitsFlag(pipeline, TRUE);
                
                // Get PostScript CRD
                cmsUInt32Number bufferSize = 4096;
                char *buffer = (char*)malloc(bufferSize);
                if (buffer) {
                    cmsUInt32Number needed = cmsGetPostScriptCRD(context, hProfile, 
                                                                 INTENT_PERCEPTUAL, 0, 
                                                                 buffer, bufferSize);
                    free(buffer);
                }
                
                cmsPipelineFree(pipeline);
            }
            
            // Set profile header properties
            cmsSetHeaderRenderingIntent(hProfile, INTENT_PERCEPTUAL);
            cmsSetEncodedICCversion(hProfile, 0x02400000); // Version 2.4
            
            // MLU operations
            cmsMLU *mlu = cmsMLUalloc(context, 1);
            if (mlu) {
                cmsMLU *mluDup = cmsMLUdup(mlu);
                if (mluDup) {
                    cmsMLUfree(mluDup);
                }
                cmsMLUfree(mlu);
            }
            
            // CIE2000 delta E calculation
            cmsCIELab lab1 = {50.0, 2.0, -3.0};
            cmsCIELab lab2 = {52.0, 3.0, -2.0};
            cmsFloat64Number deltaE = cmsCIE2000DeltaE(&lab1, &lab2, 1.0, 1.0, 1.0);
            
            // Get tag signature
            cmsTagSignature sig = cmsGetTagSignature(hProfile, 0);
            
            // Create other profiles
            cmsCIExyY whitePoint = {0.3457, 0.3585, 1.0}; // D50
            cmsHPROFILE labProfile = cmsCreateLab4Profile(&whitePoint);
            cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
            
            // Create and delete transform
            if (labProfile && xyzProfile) {
                cmsHTRANSFORM transform = cmsCreateTransformTHR(context, 
                    xyzProfile, TYPE_XYZ_DBL,
                    labProfile, TYPE_Lab_DBL,
                    INTENT_PERCEPTUAL, 0);
                if (transform) {
                    cmsDeleteTransform(transform);
                }
            }
            
            // Dictionary iteration
            const cmsDICTentry *dictEntry = cmsDictGetEntryList(nullptr);
            if (dictEntry) {
                const cmsDICTentry *nextEntry = cmsDictNextEntry(dictEntry);
                // Iteration would continue here
            }
            
            // Cleanup profiles
            if (labProfile) cmsCloseProfile(labProfile);
            if (xyzProfile) cmsCloseProfile(xyzProfile);
        }
        
        // Close opened profiles
        if (hProfileFile) cmsCloseProfile(hProfileFile);
        if (hProfileStream) cmsCloseProfile(hProfileStream);
    }
    
    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove("input_file.icc");
    
    // Unregister plugins (global operation)
    cmsUnregisterPlugins();
    
    // Delete context
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file.icc");
	assert_file_name_closed("input_file.icc");
	return 0;
}