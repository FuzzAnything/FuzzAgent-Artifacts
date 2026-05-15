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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 2
//<Quality> {"density":23,"unique_branches":{"ParseFloatNumber":[[752,9,752,16,0,0,4,0],[752,20,752,43,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsSetColorSpace","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPropertyDbl","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsReadTag","cmsReadTag","cmsDupProfileSequenceDescription","cmsPipelineAlloc","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsReadTag","cmsMLUgetUTF8","cmsPipelineOutputChannels","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsFreeProfileSequenceDescription","cmsMLUfree","cmsIT8Free","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsSetColorSpace","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseProfile","cmsCloseIOhandler","cmsIT8Alloc","cmsIT8SetPropertyStr","cmsIT8GetPropertyDbl","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsReadTag","cmsReadTag","cmsDupProfileSequenceDescription","cmsPipelineAlloc","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsReadTag","cmsMLUgetUTF8","cmsPipelineOutputChannels","cmsDeleteTransform","cmsCloseProfile","cmsPipelineFree","cmsFreeProfileSequenceDescription","cmsMLUfree","cmsIT8Free","cmsCloseProfile","cmsFreeToneCurve","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
	return 0;
	}
	
    
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile1 = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsHPROFILE hGrayProfile = NULL;
    cmsHPROFILE hXYZProfile = NULL;
    cmsToneCurve *gammaCurve = NULL;
    cmsIOHANDLER *io = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsMLU *mlu = NULL;
    cmsSEQ *seq = NULL;
    cmsPipeline *pipeline = NULL;
    cmsHTRANSFORM transform = NULL;
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    
    // Create context
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
	return 0;
	}
	
    
    // Open profile from memory (first profile)
    hProfile1 = cmsOpenProfileFromMemTHR(ctx, data, size);
    
    // Build gamma curve
    double gamma_val = 2.2;
    gammaCurve = cmsBuildGamma(ctx, gamma_val);
    
    // Create gray profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    if (gammaCurve) {
        hGrayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, gammaCurve);
    }
    
    // Set color space of gray profile
    if (hGrayProfile) {
        cmsSetColorSpace(hGrayProfile, cmsSigGrayData);
    }
    
    // Create file from input data
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Open I/O handler from file
        io = cmsOpenIOhandlerFromFile(ctx, "input_file", "rb");
        // Open profile from I/O handler (write mode false)
        if (io) {
            hProfile2 = cmsOpenProfileFromIOhandler2THR(ctx, io, FALSE);
        }
    }
    
    // Create output file
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // For demonstration, we could write something to out_file
        assert_file_closed(&out_file);;
        out_file = NULL;
    }
    
    // Clean up profiles and I/O handlers
    if (hProfile2) cmsCloseProfile(hProfile2);
    if (io) cmsCloseIOhandler(io);
    
    // IT8 operations
    hIT8 = cmsIT8Alloc(ctx);
    if (hIT8) {
        cmsIT8SetPropertyStr(hIT8, "TestProperty", "TestValue");
        double prop = cmsIT8GetPropertyDbl(hIT8, "TestProperty");
        (void)prop; // Prevent unused variable warning
    }
    
    // MLU operations
    mlu = cmsMLUalloc(ctx, 10);
    if (mlu) {
        char lang[3] = {'e', 'n', '\0'};
        char country[3] = {'U', 'S', '\0'};
        cmsMLUsetASCII(mlu, lang, country, "Test String");
        
        char buffer[100];
        cmsUInt32Number bufferSize = sizeof(buffer);
        cmsMLUgetUTF8(mlu, lang, country, buffer, bufferSize);
    }
    
    // Read tag from first profile
    if (hProfile1) {
        void *tagData = cmsReadTag(hProfile1, cmsSigProfileDescriptionTag);
        if (tagData) {
            // Could process tagData here
        }
        
        // Try to get sequence description
        void *seqData = cmsReadTag(hProfile1, cmsSigProfileSequenceDescTag);
        if (seqData) {
            seq = cmsDupProfileSequenceDescription((const cmsSEQ*)seqData);
        }
    }
    
    // Pipeline operations
    pipeline = cmsPipelineAlloc(ctx, 3, 3);
    if (pipeline) {
        cmsUInt32Number outChannels = cmsPipelineOutputChannels(pipeline);
        (void)outChannels; // Prevent unused variable warning
    }
    
    // Create XYZ profile
    hXYZProfile = cmsCreateXYZProfileTHR(ctx);
    
    // Create transform if we have input and output profiles
    if (hProfile1 && hXYZProfile) {
        transform = cmsCreateTransformTHR(ctx, 
            hProfile1, 
            TYPE_RGB_8, 
            hXYZProfile, 
            TYPE_XYZ_16, 
            INTENT_PERCEPTUAL, 
            0);
    }
    
    // Read tag again (second call as required)
    if (hProfile1) {
        void *tagData2 = cmsReadTag(hProfile1, cmsSigCopyrightTag);
        if (tagData2) {
            // Could process tagData2 here
        }
        
        // Second MLU getUTF8 call
        if (mlu) {
            char buffer2[100];
            cmsUInt32Number bufferSize2 = sizeof(buffer2);
            char lang2[3] = {'f', 'r', '\0'};
            char country2[3] = {'F', 'R', '\0'};
            cmsMLUgetUTF8(mlu, lang2, country2, buffer2, bufferSize2);
        }
    }
    
    // Get pipeline output channels again
    if (pipeline) {
        cmsUInt32Number outChannels2 = cmsPipelineOutputChannels(pipeline);
        (void)outChannels2; // Prevent unused variable warning
    }
    
    // Cleanup in reverse order
    if (transform) cmsDeleteTransform(transform);
    if (hXYZProfile) cmsCloseProfile(hXYZProfile);
    if (pipeline) cmsPipelineFree(pipeline);
    if (seq) cmsFreeProfileSequenceDescription(seq);
    if (mlu) cmsMLUfree(mlu);
    if (hIT8) cmsIT8Free(hIT8);
    if (hGrayProfile) cmsCloseProfile(hGrayProfile);
    if (gammaCurve) cmsFreeToneCurve(gammaCurve);
    if (hProfile1) cmsCloseProfile(hProfile1);
    if (ctx) cmsDeleteContext(ctx);
    
    if (in_file) assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}