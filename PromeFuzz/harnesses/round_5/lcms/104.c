// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for at least two Lab structures (each 3 doubles = 24 bytes)
    if (Size < 48) return 0;
    
    // Parse input data into Lab and LCh structures
    cmsCIELab lab1, lab2;
    cmsCIELCh lch1, lch2;
    
    // Use first 24 bytes for lab1
    memcpy(&lab1, Data, sizeof(lab1));
    Data += sizeof(lab1);
    Size -= sizeof(lab1);
    
    // Use next 24 bytes for lab2
    if (Size < sizeof(lab2)) return 0;
    memcpy(&lab2, Data, sizeof(lab2));
    Data += sizeof(lab2);
    Size -= sizeof(lab2);
    
    // Use next 24 bytes for lch1 if available
    if (Size >= sizeof(lch1)) {
        memcpy(&lch1, Data, sizeof(lch1));
        Data += sizeof(lch1);
        Size -= sizeof(lch1);
    }
    
    // Use next 24 bytes for lch2 if available
    if (Size >= sizeof(lch2)) {
        memcpy(&lch2, Data, sizeof(lch2));
        Data += sizeof(lch2);
        Size -= sizeof(lch2);
    }
    
    // Parse l and c parameters for cmsCMCdeltaE if available
    cmsFloat64Number l = 1.0, c = 1.0;  // Default values
    if (Size >= sizeof(cmsFloat64Number) * 2) {
        memcpy(&l, Data, sizeof(l));
        Data += sizeof(l);
        memcpy(&c, Data, sizeof(c));
    }
    
    // Call target API functions
    
    // 1. cmsCIE94DeltaE
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);
    (void)deltaE94;  // Use result to avoid unused variable warning
    
    // 2. cmsLab2LCh
    cmsCIELCh lch_out1;
    cmsLab2LCh(&lch_out1, &lab1);
    
    // 3. cmsLCh2Lab
    cmsCIELab lab_out1;
    if (Size >= sizeof(lch1)) {
        cmsLCh2Lab(&lab_out1, &lch1);
    }
    
    // 4. cmsCMCdeltaE
    cmsFloat64Number deltaECMC = cmsCMCdeltaE(&lab1, &lab2, l, c);
    (void)deltaECMC;
    
    // 5. cmsDeltaE
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    (void)deltaE;
    
    // 6. cmsBFDdeltaE
    cmsFloat64Number deltaEBFD = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaEBFD;
    
    // Additional conversions for more coverage
    if (Size >= sizeof(lch1) + sizeof(lch2)) {
        cmsCIELab lab_from_lch1, lab_from_lch2;
        cmsCIELCh lch_from_lab1, lch_from_lab2;
        
        cmsLCh2Lab(&lab_from_lch1, &lch1);
        cmsLCh2Lab(&lab_from_lch2, &lch2);
        cmsLab2LCh(&lch_from_lab1, &lab1);
        cmsLab2LCh(&lch_from_lab2, &lab2);
        
        // Compute deltaE between converted values
        cmsDeltaE(&lab_from_lch1, &lab_from_lch2);
        cmsCIE94DeltaE(&lab_from_lch1, &lab_from_lch2);
        cmsBFDdeltaE(&lab_from_lch1, &lab_from_lch2);
        cmsCMCdeltaE(&lab_from_lch1, &lab_from_lch2, l, c);
    }
    
    return 0;
}