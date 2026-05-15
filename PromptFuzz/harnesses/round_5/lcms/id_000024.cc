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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"cmsBFDdeltaE":[[517,9,517,51,0,0,4,1]],"ComputeLBFD":[[486,7,486,24,0,0,4,1]]},"library_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetHeaderAttributes","cmsMLUalloc","cmsGetEncodedICCversion","cmsMLUgetTranslation","cmsGetHeaderFlags","cmsIT8LoadFromFile","cmsIT8SetTable","cmsIT8Free","cmsLCh2Lab","cmsBFDdeltaE","cmsFreeProfileSequenceDescription","cmsMLUfree","cmsCloseProfile","cmsDeleteContext"],"critical_calls":["cmsCreateContext","cmsOpenProfileFromMemTHR","cmsGetHeaderAttributes","cmsMLUalloc","cmsGetEncodedICCversion","cmsMLUgetTranslation","cmsGetHeaderFlags","cmsIT8LoadFromFile","cmsIT8SetTable","cmsIT8Free","cmsLCh2Lab","cmsBFDdeltaE","cmsFreeProfileSequenceDescription","cmsMLUfree","cmsCloseProfile","cmsDeleteContext"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
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
	  // Need minimum data for meaningful operations
    
    // 1. Create context
    cmsContext context = cmsCreateContext(nullptr, nullptr);
    if (!context) {
	return 0;
	}
	
    
    // 2. Open profile from memory (consume input data)
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!profile) {
        // Even if profile fails, continue with other operations
        // using dummy data where needed
    }
    
    // 3. Get header attributes
    cmsUInt64Number flags = 0;
    if (profile) {
        cmsGetHeaderAttributes(profile, &flags);
    }
    
    // 4. Allocate MLU object
    cmsMLU *mlu = cmsMLUalloc(context, 10);
    
    // 5. Get encoded ICC version
    cmsUInt32Number version = 0;
    if (profile) {
        version = cmsGetEncodedICCversion(profile);
    }
    
    // 6. Try MLU translation (even if MLU is empty)
    char obtainedLang[3] = {0};
    char obtainedCountry[3] = {0};
    if (mlu) {
        cmsMLUgetTranslation(mlu, "en", "US", obtainedLang, obtainedCountry);
    }
    
    // 7. Get header flags
    cmsUInt32Number headerFlags = 0;
    if (profile) {
        headerFlags = cmsGetHeaderFlags(profile);
    }
    
    // 8. Create IT8 handle and set table (using file input)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        cmsHANDLE it8 = cmsIT8LoadFromFile(context, "input_file");
        if (it8) {
            // Try to set table (may fail if no valid table index)
            cmsInt32Number tableResult = cmsIT8SetTable(it8, 0);
            (void)tableResult; // Result intentionally ignored for fuzzing
            
            // Clean up IT8
            cmsIT8Free(it8);
        }
        assert_file_closed(&in_file);;
    }
    
    // 9. Perform color space calculations
    // Create LCh and Lab structures from input data
    if (size >= sizeof(cmsCIELCh) + sizeof(cmsCIELab)) {
        cmsCIELCh lch;
        cmsCIELab lab1, lab2;
        
        // Use parts of input data for color values
        size_t offset = 0;
        if (size - offset >= sizeof(lch)) {
            memcpy(&lch, data + offset, sizeof(lch));
            offset += sizeof(lch);
        } else {
            // Use default values if not enough data
            lch.L = 50.0;
            lch.C = 20.0;
            lch.h = 180.0;
        }
        
        // Convert LCh to Lab
        cmsLCh2Lab(&lab1, &lch);
        
        // Create second Lab from input data
        if (size - offset >= sizeof(lab2)) {
            memcpy(&lab2, data + offset, sizeof(lab2));
        } else {
            // Use modified values for second Lab
            lab2.L = 60.0;
            lab2.a = 10.0;
            lab2.b = 20.0;
        }
        
        // Calculate BFD deltaE
        cmsFloat64Number deltaE = cmsBFDdeltaE(&lab1, &lab2);
        (void)deltaE; // Result intentionally ignored for fuzzing
    }
    
    // 10. Create and free sequence description
    cmsSEQ *seq = (cmsSEQ *)malloc(sizeof(cmsSEQ));
    if (seq) {
        seq->n = 0;
        seq->ContextID = context;
        seq->seq = nullptr;
        cmsFreeProfileSequenceDescription(seq);
    }
    
    // 11. Clean up resources
    if (mlu) {
        cmsMLUfree(mlu);
    }
    
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    cmsDeleteContext(context);
    
    assert_file_closed(&in_file);
	return 0;
}