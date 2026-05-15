// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void init_lab_from_data(cmsCIELab* lab, const uint8_t** data, size_t* size) {
    if (*size >= sizeof(double) * 3) {
        memcpy(&lab->L, *data, sizeof(double));
        *data += sizeof(double);
        memcpy(&lab->a, *data, sizeof(double));
        *data += sizeof(double);
        memcpy(&lab->b, *data, sizeof(double));
        *data += sizeof(double);
        *size -= sizeof(double) * 3;
    } else {
        lab->L = lab->a = lab->b = 0.0;
    }
}

static double read_double_from_data(const uint8_t** data, size_t* size) {
    double value = 0.0;
    if (*size >= sizeof(double)) {
        memcpy(&value, *data, sizeof(double));
        *data += sizeof(double);
        *size -= sizeof(double);
    }
    return value;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3 * 2) {
        return 0;
    }

    const uint8_t* data_ptr = Data;
    size_t remaining = Size;

    cmsCIELab lab1, lab2;
    init_lab_from_data(&lab1, &data_ptr, &remaining);
    init_lab_from_data(&lab2, &data_ptr, &remaining);

    cmsDesaturateLab(&lab1, 
                     read_double_from_data(&data_ptr, &remaining),
                     read_double_from_data(&data_ptr, &remaining),
                     read_double_from_data(&data_ptr, &remaining),
                     read_double_from_data(&data_ptr, &remaining));

    cmsCIE2000DeltaE(&lab1, &lab2,
                     read_double_from_data(&data_ptr, &remaining),
                     read_double_from_data(&data_ptr, &remaining),
                     read_double_from_data(&data_ptr, &remaining));

    cmsDeltaE(&lab1, &lab2);

    cmsBFDdeltaE(&lab1, &lab2);

    cmsCIE94DeltaE(&lab1, &lab2);

    cmsCMCdeltaE(&lab1, &lab2,
                 read_double_from_data(&data_ptr, &remaining),
                 read_double_from_data(&data_ptr, &remaining));

    return 0;
}