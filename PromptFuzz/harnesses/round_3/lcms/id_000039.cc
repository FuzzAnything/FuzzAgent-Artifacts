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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 8
//<Quality> {"density":23,"unique_branches":{"cmsIT8GetPatchName":[[2973,9,2973,14,0,0,4,0]],"TetrahedralInterpFloat":[[648,16,648,39,0,0,4,1],[651,16,651,39,0,0,4,1],[654,16,654,39,0,0,4,1],[656,21,656,39,0,0,4,0],[656,21,656,39,0,0,4,1],[662,13,662,21,0,0,4,0],[662,25,662,33,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsCreateProfilePlaceholder","cmsDeleteContext","cmsSetHeaderProfileID","cmsSetColorSpace","cmsGetHeaderCMM","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataFormat","cmsIT8SetDataRowCol","cmsIT8SetDataRowColDbl","cmsIT8GetPatchName","cmsIT8GetDataRowColDbl","cmsIT8Free","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsStageFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsSetLogErrorHandlerTHR","cmsCreateProfilePlaceholder","cmsSetHeaderProfileID","cmsSetColorSpace","cmsGetHeaderCMM","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8SetDataFormat","cmsIT8SetDataRowCol","cmsIT8SetDataRowColDbl","cmsIT8GetPatchName","cmsIT8GetDataRowColDbl","cmsIT8Free","cmsStageAllocCLutFloat","cmsPipelineAlloc","cmsPipelineInsertStage","cmsPipelineEvalFloat","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


// Simple error handler that does nothing
static void  id_000039_DummyErrorHandler(cmsContext /*ContextID*/, cmsUInt32Number /*ErrorCode*/, 
                              const char* /*Text*/) {
    // Do nothing - just a placeholder
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create context first
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Set error handler
    cmsSetLogErrorHandlerTHR(ctx,  id_000039_DummyErrorHandler);
    
    // Create a profile placeholder
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Set profile ID (first 16 bytes as profile ID)
    cmsUInt8Number profileID[16];
    size_t copySize = size < 16 ? size : 16;
    memcpy(profileID, data, copySize);
    cmsSetHeaderProfileID(hProfile, profileID);
    
    // Set color space (use data to determine which one)
    cmsColorSpaceSignature sig;
    if (data[0] % 4 == 0) {
        sig = cmsSigRgbData;
    } else if (data[0] % 4 == 1) {
        sig = cmsSigGrayData;
    } else if (data[0] % 4 == 2) {
        sig = cmsSigLabData;
    } else {
        sig = cmsSigCmykData;
    }
    cmsSetColorSpace(hProfile, sig);
    
    // Get header CMM
    cmsUInt32Number cmm = cmsGetHeaderCMM(hProfile);
    (void)cmm; // Prevent unused variable warning
    
    // Create IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        // Set some IT8 properties using data
        const char* propName = "TestProperty";
        const char* propValue = "TestValue";
        cmsIT8SetPropertyStr(hIT8, propName, propValue);
        
        // Try to set data format
        cmsIT8SetDataFormat(hIT8, 0, "Sample");
        
        // Set some data rows/columns
        int maxRows = 10;
        int maxCols = 5;
        size_t dataIndex = 16; // Start after profile ID
        
        for (int row = 0; row < maxRows && dataIndex < size; row++) {
            for (int col = 0; col < maxCols && dataIndex < size; col++) {
                // Create a value from data
                char valBuf[32];
                snprintf(valBuf, sizeof(valBuf), "%.2f", (double)data[dataIndex] / 255.0);
                cmsIT8SetDataRowCol(hIT8, row, col, valBuf);
                
                // Also set as double
                cmsIT8SetDataRowColDbl(hIT8, row, col, (cmsFloat64Number)data[dataIndex] / 255.0);
                dataIndex++;
            }
        }
        
        // Get data from IT8 (try first few rows/cols)
        if (maxRows > 0 && maxCols > 0) {
            // Get patch names
            char patchBuf[256];
            for (int i = 0; i < 5 && i < maxRows; i++) {
                const char* patchName = cmsIT8GetPatchName(hIT8, i, patchBuf);
                (void)patchName; // Prevent unused warning
            }
            
            // Get data as double
            cmsFloat64Number dblVal = cmsIT8GetDataRowColDbl(hIT8, 0, 0);
            (void)dblVal; // Prevent unused warning
        }
        
        // Free IT8 handle
        cmsIT8Free(hIT8);
    }
    
    // Create CLUT stage
    // Simple 2x2x2 CLUT for RGB->RGB
    cmsUInt32Number nGridPoints = 2;
    cmsUInt32Number inputChan = 3;
    cmsUInt32Number outputChan = 3;
    
    // Create table data (2^3 * 3 = 24 entries)
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(24 * sizeof(cmsFloat32Number));
    if (table) {
        // Fill table with data from input
        size_t tableDataStart = size > 40 ? 40 : 0;
        for (int i = 0; i < 24 && (tableDataStart + i) < size; i++) {
            table[i] = (cmsFloat32Number)data[tableDataStart + i] / 255.0f;
        }
        
        cmsStage* clutStage = cmsStageAllocCLutFloat(ctx, nGridPoints, inputChan, outputChan, table);
        if (clutStage) {
            // Use the stage in a simple pipeline
            cmsPipeline* lut = cmsPipelineAlloc(ctx, inputChan, outputChan);
            if (lut) {
                cmsPipelineInsertStage(lut, cmsAT_BEGIN, clutStage);
                
                // Test the pipeline
                cmsFloat32Number in[3] = {0.5f, 0.5f, 0.5f};
                cmsFloat32Number out[3];
                cmsPipelineEvalFloat(in, out, lut);
                
                cmsPipelineFree(lut);
            } else {
                cmsStageFree(clutStage);
            }
        }
        
        free(table);
    }
    
    // Note: cmsGBDFree is not available in the provided API list, so we cannot call it
    
    // Clean up
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}