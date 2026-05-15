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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 20.666666, nr_unique_branch: 1
//<Quality> {"density":31,"unique_branches":{"cmsCreateExtendedTransform":[[1201,9,1201,101,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsDeleteContext","cmsSetColorSpace","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPropertyDbl","cmsIT8Free","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsMLUfree","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsPipelineAlloc","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsFreeToneCurve","cmsSetColorSpace","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPropertyDbl","cmsIT8Free","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsMLUfree","cmsReadTag","cmsDupProfileSequenceDescription","cmsFreeProfileSequenceDescription","cmsPipelineAlloc","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsCloseProfile","cmsDeleteContext"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
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
	  // Too small for meaningful processing
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // 1. Open profile from memory using cmsOpenProfileFromMemTHR
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, data, size);
    if (!hProfile) {
        // Try creating a dummy profile from the data
        cmsToneCurve* curve = cmsBuildGamma(ctx, 2.2);
        cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
        hProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, curve);
        cmsFreeToneCurve(curve);
        if (!hProfile) {
            cmsDeleteContext(ctx);
            return 0;
        }
    }
    
    // 2. Set color space using cmsSetColorSpace
    cmsSetColorSpace(hProfile, cmsSigRgbData);
    
    // 3. Open profile from I/O handler using cmsOpenProfileFromIOhandler2THR
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
        if (io) {
            cmsHPROFILE hProfile2 = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
            if (hProfile2) {
                cmsCloseProfile(hProfile2);
            }
            cmsCloseIOhandler(io);
        }
        assert_file_closed(&in_file);;
    }
    
    // 4. Get IT8 property using cmsIT8GetPropertyDbl
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        cmsIT8SetPropertyStr(hIT8, "TestProp", "123.456");
        double propValue = cmsIT8GetPropertyDbl(hIT8, "TestProp");
        (void)propValue; // Use the value to avoid unused warning
        cmsIT8Free(hIT8);
    }
    
    // 5. Get MLU UTF8 string using cmsMLUgetUTF8
    cmsMLU* mlu = cmsMLUalloc(ctx, 1);
    if (mlu) {
        cmsMLUsetASCII(mlu, "en", "US", "Test UTF8 String");
        char buffer[256];
        cmsUInt32Number len = cmsMLUgetUTF8(mlu, "en", "US", buffer, sizeof(buffer));
        (void)len; // Use the length to avoid unused warning
        cmsMLUfree(mlu);
    }
    
    // 6. Duplicate profile sequence description using cmsDupProfileSequenceDescription
    cmsSEQ* seq = (cmsSEQ*)cmsReadTag(hProfile, cmsSigProfileSequenceDescTag);
    if (seq) {
        cmsSEQ* dupSeq = cmsDupProfileSequenceDescription(seq);
        if (dupSeq) {
            cmsFreeProfileSequenceDescription(dupSeq);
        }
    }
    
    // 7. Get pipeline output channels using cmsPipelineOutputChannels
    cmsPipeline* lut = cmsPipelineAlloc(ctx, 3, 3);
    if (lut) {
        cmsUInt32Number outChannels = cmsPipelineOutputChannels(lut);
        (void)outChannels; // Use the value to avoid unused warning
        
        // Create a simple transform to test the pipeline
        cmsHPROFILE hXYZ = cmsCreateXYZProfileTHR(ctx);
        if (hXYZ) {
            cmsHTRANSFORM hTransform = cmsCreateTransformTHR(ctx, hProfile, 
                                                             TYPE_RGB_8, 
                                                             hXYZ, 
                                                             TYPE_XYZ_16, 
                                                             INTENT_PERCEPTUAL, 0);
            if (hTransform) {
                // Test with some sample data
                unsigned char rgb[3] = {128, 128, 128};
                cmsUInt16Number xyz[3];
                cmsDoTransform(hTransform, rgb, xyz, 1);
                cmsDeleteTransform(hTransform);
            }
            cmsCloseProfile(hXYZ);
        }
        
        cmsPipelineFree(lut);
    }
    
    // Clean up
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ctx);
    
    assert_file_closed(&in_file);
	return 0;
}