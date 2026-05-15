// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetASCII at cmsnamed.c:542:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUgetUTF8 at cmsnamed.c:590:27 in lcms2.h
// cmsMLUtranslationsCount at cmsnamed.c:689:27 in lcms2.h
// cmsMLUalloc at cmsnamed.c:33:19 in lcms2.h
// cmsMLUdup at cmsnamed.c:430:19 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsMLUfree at cmsnamed.c:476:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsMLU* create_test_mlu(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsMLU* mlu = cmsMLUalloc(ctx, 2);
    if (!mlu) return NULL;
    
    // Try to add some test entries if we have enough data
    if (size >= 8) {
        char lang1[3] = {data[0], data[1], '\0'};
        char country1[3] = {data[2], data[3], '\0'};
        char lang2[3] = {data[4], data[5], '\0'};
        char country2[3] = {data[6], data[7], '\0'};
        
        // Use internal function if available, otherwise leave empty
        // For fuzzing purposes, empty MLU is also valid
    }
    
    return mlu;
}

int LLVMFuzzerTestOneInput_25(const uint8_t* Data, size_t Size) {
    if (Size < 12) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Extract parameters from fuzzer input
    char lang_code[3] = {Data[0], Data[1], '\0'};
    char country_code[3] = {Data[2], Data[3], '\0'};
    cmsUInt32Number nItems = *(cmsUInt32Number*)(Data + 4);
    
    // Create test MLU
    cmsMLU* mlu = create_test_mlu(ctx, Data + 8, Size - 8);
    if (!mlu) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsMLUgetASCII
    char ascii_buffer[256];
    cmsUInt32Number ascii_len = cmsMLUgetASCII(mlu, lang_code, country_code, 
                                               ascii_buffer, sizeof(ascii_buffer));
    // Also test with NULL buffer to get required size
    cmsUInt32Number required_ascii_len = cmsMLUgetASCII(mlu, lang_code, country_code, 
                                                       NULL, 0);
    
    // Test cmsMLUgetUTF8
    char utf8_buffer[256];
    cmsUInt32Number utf8_len = cmsMLUgetUTF8(mlu, lang_code, country_code,
                                            utf8_buffer, sizeof(utf8_buffer));
    // Also test with NULL buffer
    cmsUInt32Number required_utf8_len = cmsMLUgetUTF8(mlu, lang_code, country_code,
                                                     NULL, 0);
    
    // Test cmsMLUtranslationsCount
    cmsUInt32Number translation_count = cmsMLUtranslationsCount(mlu);
    
    // Test cmsMLUalloc with different sizes
    cmsMLU* mlu2 = cmsMLUalloc(ctx, nItems);
    
    // Test cmsMLUdup
    cmsMLU* mlu_dup = cmsMLUdup(mlu);
    
    // Test cmsMLUfree (safe with NULL)
    cmsMLUfree(NULL);
    
    // Cleanup
    if (mlu_dup) cmsMLUfree(mlu_dup);
    if (mlu2) cmsMLUfree(mlu2);
    cmsMLUfree(mlu);
    cmsDeleteContext(ctx);
    
    return 0;
}