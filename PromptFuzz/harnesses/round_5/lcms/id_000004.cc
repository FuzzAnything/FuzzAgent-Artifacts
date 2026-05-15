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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsSetColorSpace","cmsCloseProfile","cmsDeleteContext","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsReadTag","cmsMLUgetUTF8","cmsReadTag","cmsPipelineOutputChannels","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsIT8LoadFromMem","cmsIT8GetPropertyDbl","cmsIT8Free","cmsCloseProfile","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsSetColorSpace","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsReadTag","cmsMLUgetUTF8","cmsReadTag","cmsPipelineOutputChannels","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsIT8LoadFromMem","cmsIT8GetPropertyDbl","cmsIT8Free","cmsCloseProfile","cmsCloseIOhandler","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified lcms APIs to process profile data from memory:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
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
	 // Need minimum data for processing

    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	

    // 1. Open profile from memory using the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 2. Set color space of the profile
    cmsSetColorSpace(hProfile, cmsSigRgbData);

    // 3. Create I/O handler from memory and open profile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        cmsCloseProfile(hProfile);
        cmsDeleteContext(ctx);
        assert_file_closed(&in_file);
	return 0;
    }

    cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
    if (io) {
        cmsHPROFILE hProfile2 = cmsOpenProfileFromIOhandler2THR(ctx, io, false);
        if (hProfile2) {
            // Get MLU from profile for language info
            cmsMLU *mlu = (cmsMLU*)cmsReadTag(hProfile2, cmsSigProfileDescriptionTag);
            if (mlu) {
                char buffer[256];
                // 5. Get UTF8 string from MLU
                cmsMLUgetUTF8(mlu, "en", "US", buffer, sizeof(buffer));
            }

            // Get pipeline if available
            cmsPipeline *lut = (cmsPipeline*)cmsReadTag(hProfile2, cmsSigAToB0Tag);
            if (lut) {
                // 7. Get output channels from pipeline
                cmsUInt32Number outputChannels = cmsPipelineOutputChannels(lut);
                (void)outputChannels; // Use variable to avoid unused warning
            }

            // Get sequence description if available
            cmsSEQ *seq = (cmsSEQ*)cmsReadTag(hProfile2, cmsSigProfileSequenceDescTag);
            if (seq) {
                // 6. Duplicate profile sequence description
                cmsSEQ *dupSeq = cmsDupProfileSequenceDescription(seq);
                if (dupSeq) {
                    cmsFreeProfileSequenceDescription(dupSeq);
                }
            }

            // Try to get IT8 property if IT8 data exists
            cmsHANDLE hIT8 = cmsIT8LoadFromMem(ctx, data, size);
            if (hIT8) {
                // 4. Get property as double from IT8
                cmsFloat64Number propValue = cmsIT8GetPropertyDbl(hIT8, "SAMPLE_ID");
                (void)propValue; // Use variable to avoid unused warning
                cmsIT8Free(hIT8);
            }

            cmsCloseProfile(hProfile2);
        }
        cmsCloseIOhandler(io);
    }

    assert_file_closed(&in_file);;
    
    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}