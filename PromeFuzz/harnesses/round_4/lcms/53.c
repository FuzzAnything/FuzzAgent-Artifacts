// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // We need at least enough data for two cmsCIELab structures (6 doubles = 48 bytes)
    if (Size < 48) return 0;
    
    // Create Lab structures from fuzzer input
    cmsCIELab lab1, lab2;
    cmsCIELCh lch1;
    
    // Use the first 24 bytes for lab1
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    
    // Use the next 24 bytes for lab2  
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    // Test cmsDeltaE
    cmsDeltaE(&lab1, &lab2);
    
    // Test cmsBFDdeltaE
    cmsBFDdeltaE(&lab1, &lab2);
    
    // Test cmsCIE94DeltaE
    cmsCIE94DeltaE(&lab1, &lab2);
    
    // Test cmsCMCdeltaE - use additional bytes for l and c parameters if available
    if (Size >= 64) {
        cmsFloat64Number l, c;
        memcpy(&l, Data + 48, sizeof(cmsFloat64Number));
        memcpy(&c, Data + 56, sizeof(cmsFloat64Number));
        cmsCMCdeltaE(&lab1, &lab2, l, c);
    }
    
    // Test conversion functions
    // Lab to LCh
    cmsLab2LCh(&lch1, &lab1);
    
    // LCh to Lab
    cmsLCh2Lab(&lab2, &lch1);
    
    // Only test with additional LCh data if we have enough bytes
    // We need 88 bytes total: 48 for two Lab + 16 for l and c + 24 for lch2
    if (Size >= 88) {
        cmsCIELCh lch2;
        memcpy(&lch2, Data + 64, sizeof(cmsCIELCh));
        cmsLCh2Lab(&lab1, &lch2);
    }
    
    return 0;
}