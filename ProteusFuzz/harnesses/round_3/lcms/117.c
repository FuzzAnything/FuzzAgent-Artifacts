// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsIsToneCurveDescending at cmsgamma.c:1393:20 in lcms2.h
// cmsIsToneCurveMultisegment at cmsgamma.c:1402:20 in lcms2.h
// cmsIsToneCurveLinear at cmsgamma.c:1329:19 in lcms2.h
// cmsIsToneCurveMonotonic at cmsgamma.c:1347:20 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsToneCurve* create_curve_from_data(const uint8_t *Data, size_t Size) {
    cmsToneCurve *Curve = NULL;
    cmsUInt16Number *Table = NULL;
    cmsUInt32Number nEntries = 0;

    if (Size < 2) return NULL;

    // Interpret the first two bytes as the number of entries (capped at 256 for safety)
    nEntries = (Data[0] << 8) | Data[1];
    if (nEntries == 0 || nEntries > 256) {
        nEntries = 256;
    }

    // Ensure we have enough data for the table
    if (Size < 2 + (nEntries * 2)) {
        // If data is insufficient, generate a random linear curve
        Table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
        if (!Table) return NULL;
        for (cmsUInt32Number i = 0; i < nEntries; i++) {
            Table[i] = (cmsUInt16Number)(i * 65535 / (nEntries - 1));
        }
    } else {
        // Use provided data
        Table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
        if (!Table) return NULL;
        memcpy(Table, Data + 2, nEntries * sizeof(cmsUInt16Number));
    }

    // Use cmsBuildTabulatedToneCurve16 instead of cmsBuildTableToneCurve
    Curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, Table);
    free(Table);
    return Curve;
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t *Data, size_t Size) {
    cmsToneCurve *Curve = NULL;
    cmsFloat64Number Params[10] = {0};
    cmsInt32Number Type = 0;

    if (Size > 0) {
        Type = (Data[0] % 5) + 1; // Types 1-5
        if (Size > 1) {
            // Generate some parametric values based on input
            for (int i = 0; i < 10 && (i + 1) * sizeof(cmsFloat64Number) < Size; i++) {
                uint64_t val = 0;
                memcpy(&val, Data + 1 + i * sizeof(uint64_t), sizeof(uint64_t));
                Params[i] = (cmsFloat64Number)val;
            }
        }
    }

    Curve = cmsBuildParametricToneCurve(NULL, Type, Params);
    return Curve;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    cmsToneCurve *Curve1 = NULL;
    cmsToneCurve *Curve2 = NULL;
    cmsFloat64Number lambda = 0.0;
    cmsBool result;
    cmsInt32Number paramType;

    if (Size == 0) return 0;

    // Extract lambda from first byte if available
    lambda = ((cmsFloat64Number)Data[0]) / 255.0;

    // Strategy 1: Create a table-based curve
    Curve1 = create_curve_from_data(Data, Size);
    
    if (Curve1) {
        // Test cmsIsToneCurveDescending
        cmsIsToneCurveDescending(Curve1);

        // Test cmsIsToneCurveMultisegment
        cmsIsToneCurveMultisegment(Curve1);

        // Test cmsIsToneCurveLinear
        cmsIsToneCurveLinear(Curve1);

        // Test cmsIsToneCurveMonotonic
        cmsIsToneCurveMonotonic(Curve1);

        // Test cmsGetToneCurveParametricType (should return 0 for table-based)
        paramType = cmsGetToneCurveParametricType(Curve1);
        (void)paramType;

        // Test cmsSmoothToneCurve
        // Note: This modifies the curve, so we do it last for this curve instance
        result = cmsSmoothToneCurve(Curve1, lambda);
        (void)result;
    }

    // Strategy 2: Create a parametric curve
    Curve2 = create_parametric_curve_from_data(Data, Size);

    if (Curve2) {
        // Test cmsIsToneCurveDescending
        cmsIsToneCurveDescending(Curve2);

        // Test cmsIsToneCurveMultisegment
        cmsIsToneCurveMultisegment(Curve2);

        // Test cmsIsToneCurveLinear
        cmsIsToneCurveLinear(Curve2);

        // Test cmsIsToneCurveMonotonic
        cmsIsToneCurveMonotonic(Curve2);

        // Test cmsGetToneCurveParametricType
        paramType = cmsGetToneCurveParametricType(Curve2);
        (void)paramType;

        // Test cmsSmoothToneCurve
        result = cmsSmoothToneCurve(Curve2, lambda);
        (void)result;
    }

    // Cleanup
    if (Curve1) cmsFreeToneCurve(Curve1);
    if (Curve2) cmsFreeToneCurve(Curve2);

    return 0;
}