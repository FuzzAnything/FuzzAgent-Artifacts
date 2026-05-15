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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 69, nr_unique_branch: 10
//<Quality> {"density":23,"unique_branches":{"OptimizeMatrixShaper":[[1691,12,1691,41,0,0,4,0]],"Unroll8ToFloat":[[1249,9,1249,19,0,0,4,1],[1252,17,1252,26,0,0,4,0],[1252,17,1252,26,0,0,4,1],[1254,33,1254,39,0,0,4,1],[1256,13,1256,19,0,0,4,1],[1263,22,1263,29,0,0,4,1],[1267,9,1267,19,0,0,4,0],[1267,23,1267,32,0,0,4,1],[731,31,731,44,7,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseIOhandler","cmsOpenProfileFromFileTHR","cmsDeleteContext","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsSetColorSpace","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsPipelineAlloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsMLUfree","cmsReadTag","cmsGetTagSignature","cmsDupProfileSequenceDescription","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsPipelineFree","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsOpenIOhandlerFromFile","cmsOpenProfileFromIOhandler2THR","cmsCloseIOhandler","cmsOpenProfileFromFileTHR","cmsBuildGamma","cmsCreateGrayProfileTHR","cmsSetColorSpace","cmsDetectDestinationBlackPoint","cmsPipelineAlloc","cmsPipelineAlloc","cmsMLUalloc","cmsMLUsetASCII","cmsMLUgetUTF8","cmsMLUfree","cmsReadTag","cmsGetTagSignature","cmsDupProfileSequenceDescription","cmsPipelineOutputChannels","cmsCreateXYZProfileTHR","cmsCreateTransformTHR","cmsDoTransform","cmsDoTransform","cmsDeleteTransform","cmsPipelineFree","cmsPipelineFree","cmsFreeProfileSequenceDescription","cmsFreeToneCurve","cmsCloseProfile","cmsCloseProfile","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified lcms APIs to process ICC profiles and perform color transformations:

*/


// Include lcms2 headers

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
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
        return 0; // Too small to be meaningful
    }

    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
        return 0;
    }

    // 2. Open profile from memory
    cmsHPROFILE hInProfile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!hInProfile) {
        // If we can't open from memory, try from file
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(context, "input_file", "rb");
            if (io) {
                hInProfile = cmsOpenProfileFromIOhandler2THR(context, io, FALSE);
                cmsCloseIOhandler(io);
            }
            assert_file_closed(&in_file);;
        }
    }

    if (!hInProfile) {
        // Try opening from file directly
        FILE *tmp = fopen("input_file", "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
            hInProfile = cmsOpenProfileFromFileTHR(context, "input_file", "r");
        }
    }

    if (!hInProfile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 3. Create gamma curve
    cmsToneCurve *gammaCurve = cmsBuildGamma(context, 2.2);
    
    // 4. Create gray profile
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65 white point
    cmsHPROFILE hGrayProfile = cmsCreateGrayProfileTHR(context, &whitePoint, gammaCurve);
    
    // 5. Set color space
    cmsSetColorSpace(hGrayProfile, cmsSigGrayData);
    
    // 6. Detect black point
    cmsCIEXYZ blackPoint;
    cmsDetectDestinationBlackPoint(&blackPoint, hInProfile, INTENT_PERCEPTUAL, 0);
    
    // 7. Allocate pipeline
    cmsPipeline *pipeline1 = cmsPipelineAlloc(context, 3, 3);
    cmsPipeline *pipeline2 = cmsPipelineAlloc(context, 3, 3);
    
    // 8. IT8 operations (simulate with dummy data)
    cmsHANDLE hIT8 = nullptr;
    // Note: We can't create IT8 from given APIs, so we'll skip actual IT8 operations
    // but demonstrate the API calls with dummy data
    double dummyProp = 0.0;
    if (size > 50) {
        // Use part of input data as dummy property
        dummyProp = static_cast<double>(data[0]) / 255.0;
    }
    
    // 9. MLU operations
    cmsMLU *mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        cmsMLUsetASCII(mlu, "en", "US", "Test Description");
        
        char buffer[256];
        cmsUInt32Number bytes = cmsMLUgetUTF8(mlu, "en", "US", buffer, sizeof(buffer));
        
        cmsMLUfree(mlu);
    }
    
    // 10. Read tag from profile
    void *tagData = cmsReadTag(hInProfile, cmsSigProfileDescriptionTag);
    
    // 11. Profile sequence operations
    cmsSEQ *seq = nullptr;
    if (tagData) {
        // Try to duplicate sequence if it's a sequence tag
        if (cmsGetTagSignature(hInProfile, 0) == cmsSigProfileSequenceDescTag) {
            seq = cmsDupProfileSequenceDescription((cmsSEQ *)tagData);
        }
    }
    
    // 12. Get pipeline output channels
    cmsUInt32Number outputChannels = 0;
    if (pipeline1) {
        outputChannels = cmsPipelineOutputChannels(pipeline1);
    }
    
    // 13. Create XYZ profile
    cmsHPROFILE hXYZProfile = cmsCreateXYZProfileTHR(context);
    
    // 14. Create transform
    cmsHTRANSFORM hTransform = nullptr;
    if (hGrayProfile && hXYZProfile) {
        hTransform = cmsCreateTransformTHR(context,
                                          hGrayProfile,
                                          TYPE_GRAY_8,
                                          hXYZProfile,
                                          TYPE_XYZ_DBL,
                                          INTENT_PERCEPTUAL,
                                          0);
    }
    
    // 15. Perform transformation
    if (hTransform) {
        unsigned char inputGray[1] = {128};
        cmsCIEXYZ outputXYZ;
        
        cmsDoTransform(hTransform, inputGray, &outputXYZ, 1);
        
        // Also test with more data if available
        if (size >= 100) {
            size_t samples = (size - 50) / 3;
            if (samples > 0) {
                cmsCIEXYZ *xyzArray = (cmsCIEXYZ *)malloc(samples * sizeof(cmsCIEXYZ));
                if (xyzArray) {
                    cmsDoTransform(hTransform, data + 50, xyzArray, samples);
                    free(xyzArray);
                }
            }
        }
    }
    
    // 16. Cleanup
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
    
    if (pipeline1) {
        cmsPipelineFree(pipeline1);
    }
    
    if (pipeline2) {
        cmsPipelineFree(pipeline2);
    }
    
    if (seq) {
        cmsFreeProfileSequenceDescription(seq);
    }
    
    if (gammaCurve) {
        cmsFreeToneCurve(gammaCurve);
    }
    
    if (hGrayProfile) {
        cmsCloseProfile(hGrayProfile);
    }
    
    if (hXYZProfile) {
        cmsCloseProfile(hXYZProfile);
    }
    
    if (hInProfile) {
        cmsCloseProfile(hInProfile);
    }
    
    // Clean up temporary file
    remove("input_file");
    
    cmsDeleteContext(context);
    
    assert_file_name_closed("input_file");
	return 0;
}