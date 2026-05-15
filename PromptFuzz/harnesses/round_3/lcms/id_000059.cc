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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["cmsCreateContext","cmsDeleteContext","cmsOpenProfileFromFile","cmsAllocProfileSequenceDescription","cmsReadTag","cmsMLUtranslationsCount","cmsBuildGamma","cmsDupToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromFile","cmsAllocProfileSequenceDescription","cmsReadTag","cmsMLUtranslationsCount","cmsBuildGamma","cmsDupToneCurve","cmsFreeToneCurve","cmsFreeToneCurve","cmsOpenIOhandlerFromFile","cmsGetPostScriptColorResource","cmsCloseIOhandler","cmsGBDAlloc","cmsGDBAddPoint","cmsGBDFree","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a context for lcms operations
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) {
	return 0;
	}
	
    
    // Create a temporary file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open a profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("input_file", "r");
    
    // Allocate profile sequence description
    cmsSEQ *seq = cmsAllocProfileSequenceDescription(ctx, 1);
    
    // Get MLU translations count if profile exists
    cmsUInt32Number translationsCount = 0;
    if (hProfile) {
        // Get the MLU from profile description tag
        cmsMLU* mlu = (cmsMLU*)cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
        if (mlu) {
            translationsCount = cmsMLUtranslationsCount(mlu);
        }
        
        // Create and duplicate a tone curve
        cmsToneCurve *gammaCurve = cmsBuildGamma(ctx, 2.2);
        if (gammaCurve) {
            cmsToneCurve *dupCurve = cmsDupToneCurve(gammaCurve);
            if (dupCurve) {
                cmsFreeToneCurve(dupCurve);
            }
            cmsFreeToneCurve(gammaCurve);
        }
        
        // Create output file for PostScript color resource
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Create IO handler for output
            cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, "output_file", "wb");
            if (io) {
                // Get PostScript color resource
                cmsGetPostScriptColorResource(ctx, cmsPS_RESOURCE_CSA, 
                                             hProfile, INTENT_PERCEPTUAL, 0, io);
                cmsCloseIOhandler(io);
            }
            assert_file_closed(&out_file);;
        }
    }
    
    // Allocate and immediately free a GBD handle to use cmsGBDFree
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    if (hGBD) {
        // Add a sample point if needed
        cmsCIELab lab = {50.0, 0.0, 0.0};
        cmsGDBAddPoint(hGBD, &lab);
        cmsGBDFree(hGBD);
    }
    
    // Cleanup
    if (seq) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    assert_file_closed(&in_file);;
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}