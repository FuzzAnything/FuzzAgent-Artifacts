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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 10
//<Quality> {"density":12,"unique_branches":{"_cmsLinkProfiles":[[1129,51,1129,85,0,0,4,0]],"cmsAllocProfileSequenceDescription":[[991,9,991,15,0,0,4,0]],"cmsCreateTransformTHR":[[1351,65,1351,79,0,0,4,0]],"cmsChannelsOfColorSpace":[[920,5,920,24,0,0,4,0]],"cmsFormatterForPCSOfProfile":[[4051,29,4051,37,0,0,4,0],[4051,29,4051,37,0,0,4,1],[4054,9,4054,25,0,0,4,0],[4054,9,4054,25,0,0,4,1]],"Type_ProfileSequenceDesc_Read":[[3572,13,3572,48,0,0,4,0],[3576,13,3576,48,0,0,4,0]]},"library_calls":["cmsOpenProfileFromStreamTHR","cmsIsIntentSupported","cmsFormatterForPCSOfProfile","cmsReadTag","cmsMLUtranslationsCodes","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineOutputChannels","cmsGBDAlloc","cmsGDBAddPoint","cmsCMCdeltaE","cmsGBDFree","cmsDeleteTransform","cmsReadTag","cmsFreeProfileSequenceDescription","cmsCloseProfile"],"critical_calls":["cmsOpenProfileFromStreamTHR","cmsIsIntentSupported","cmsFormatterForPCSOfProfile","cmsReadTag","cmsMLUtranslationsCodes","cmsCreateTransform","cmsGetTransformPipeline","cmsPipelineOutputChannels","cmsGBDAlloc","cmsGDBAddPoint","cmsCMCdeltaE","cmsGBDFree","cmsDeleteTransform","cmsReadTag","cmsFreeProfileSequenceDescription","cmsCloseProfile"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to process color profile data:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for processing
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open profile from the input data stream
    cmsHPROFILE hProfile = cmsOpenProfileFromStreamTHR(NULL, in_file, "r");
    if (!hProfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Check if intent is supported
    cmsUInt32Number intent = *data % 4; // Random intent from 0-3
    cmsUInt32Number usedDirection = *data % 2; // 0 or 1
    cmsIsIntentSupported(hProfile, intent, usedDirection);
    
    // 2. Get formatter for PCS
    cmsUInt32Number nBytes = 4; // Example: 4 bytes per component
    cmsBool lIsFloat = (*data % 2) ? TRUE : FALSE;
    cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    
    // 3. Get MLU translations count (need to get MLU from profile first)
    cmsMLU* mlu = (cmsMLU*)cmsReadTag(hProfile, cmsSigProfileDescriptionTag);
    if (mlu) {
        // Note: cmsMLUtranslationsCount is not in the provided list, but we'll use
        // the provided cmsMLUtranslationsCodes to get count
        cmsUInt32Number count = 0;
        char lang[3], country[3];
        while (cmsMLUtranslationsCodes(mlu, count, lang, country)) {
            count++;
        }
    }
    
    // 4. Create a transform to get pipeline
    cmsUInt32Number inputFormat = TYPE_RGB_8;
    cmsUInt32Number outputFormat = TYPE_RGB_8;
    cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, inputFormat, 
                                                  NULL, outputFormat, 
                                                  intent, dwFlags);
    
    if (hTransform) {
        // 5. Get pipeline output channels
        cmsPipeline* pipeline = cmsGetTransformPipeline(hTransform);
        if (pipeline) {
            cmsUInt32Number outputChannels = cmsPipelineOutputChannels(pipeline);
            
            // 6. Create GDB (Gamut Boundary Descriptor) and add points
            cmsHANDLE hGBD = cmsGBDAlloc(NULL);
            if (hGBD) {
                // Create some sample Lab colors
                cmsCIELab lab1, lab2;
                lab1.L = 50.0;
                lab1.a = 0.0;
                lab1.b = 0.0;
                
                // Use data to vary Lab values
                size_t dataIndex = 0;
                for (int i = 0; i < 3 && dataIndex < size; i++) {
                    lab2.L = (data[dataIndex++] % 100) + 0.0;
                    lab2.a = (data[dataIndex++] % 128) - 64.0;
                    lab2.b = (data[dataIndex++] % 128) - 64.0;
                    
                    // 7. Add point to GDB
                    cmsGDBAddPoint(hGBD, &lab2);
                    
                    // 8. Calculate CMC deltaE between lab1 and lab2
                    cmsFloat64Number l = 2.0; // lightness weighting
                    cmsFloat64Number c = 1.0; // chroma weighting
                    cmsCMCdeltaE(&lab1, &lab2, l, c);
                    
                    // Update lab1 for next comparison
                    lab1 = lab2;
                }
                
                // 9. Free GDB
                cmsGBDFree(hGBD);
            }
        }
        
        cmsDeleteTransform(hTransform);
    }
    
    // 10. Get and free sequence description if available
    cmsSEQ* seq = (cmsSEQ*)cmsReadTag(hProfile, cmsSigProfileSequenceDescTag);
    if (seq) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    // Cleanup
    cmsCloseProfile(hProfile);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}