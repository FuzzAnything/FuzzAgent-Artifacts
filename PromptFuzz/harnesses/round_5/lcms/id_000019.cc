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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"cmsSetAdaptationStateTHR":[[66,9,66,17,0,0,4,0]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsDeleteContext","cmsGetHeaderAttributes","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsIT8Free","cmsBFDdeltaE","cmsLCh2Lab","cmsSetAdaptationStateTHR","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetHeaderAttributes","cmsMLUalloc","cmsMLUgetTranslation","cmsMLUfree","cmsIT8Alloc","cmsIT8SetPropertyUncooked","cmsIT8Free","cmsBFDdeltaE","cmsLCh2Lab","cmsSetAdaptationStateTHR","cmsFreeProfileSequenceDescription","cmsCloseProfile","cmsDeleteContext"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for operations

    // Create a context for lcms operations
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	

    // 1. Open profile from memory (consumes input data)
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!profile) {
        cmsDeleteContext(context);
        return 0;
    }

    // 2. Get header attributes
    cmsUInt64Number flags = 0;
    cmsGetHeaderAttributes(profile, &flags);

    // 3. Create MLU and try to get translation
    cmsMLU* mlu = cmsMLUalloc(context, 1);
    if (mlu) {
        char obtainedLang[3] = {0};
        char obtainedCountry[3] = {0};
        // Try to get translation (may fail if no translations exist)
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
        cmsMLUfree(mlu);
    }

    // 4. Create IT8 handle and set property
    cmsHANDLE it8 = cmsIT8Alloc(context);
    if (it8) {
        // Use a portion of input data as property value
        size_t propLen = size > 50 ? 50 : size;
        char* propValue = (char*)malloc(propLen + 1);
        if (propValue) {
            memcpy(propValue, data, propLen);
            propValue[propLen] = '\0';
            cmsIT8SetPropertyUncooked(it8, "TEST_KEY", propValue);
            free(propValue);
        }
        cmsIT8Free(it8);
    }

    // 5. Create Lab colors and compute BFD deltaE
    cmsCIELab lab1 = {50.0, 20.0, 30.0};  // Example values
    cmsCIELab lab2 = {55.0, 25.0, 35.0};  // Example values
    cmsFloat64Number deltaE = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaE;  // Use variable to avoid unused warning

    // 6. Convert LCh to Lab
    cmsCIELCh lch = {60.0, 40.0, 45.0};  // Example values
    cmsCIELab convertedLab;
    cmsLCh2Lab(&convertedLab, &lch);

    // 7. Set adaptation state
    cmsFloat64Number adaptationState = 0.5;  // Example value
    cmsSetAdaptationStateTHR(context, adaptationState);

    // 8. Create and free profile sequence description
    cmsSEQ* seq = (cmsSEQ*)calloc(1, sizeof(cmsSEQ));
    if (seq) {
        seq->ContextID = context;
        seq->n = 0;
        seq->seq = nullptr;
        cmsFreeProfileSequenceDescription(seq);
    }

    // Cleanup
    cmsCloseProfile(profile);
    cmsDeleteContext(context);

    return 0;
}