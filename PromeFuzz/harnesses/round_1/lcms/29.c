// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMAT3per at cmsmtrx.c:114:16 in lcms2_plugin.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// _cmsMAT3solve at cmsmtrx.c:156:20 in lcms2_plugin.h
// _cmsMAT3identity at cmsmtrx.c:84:16 in lcms2_plugin.h
// _cmsMAT3isIdentity at cmsmtrx.c:98:19 in lcms2_plugin.h
// _cmsMAT3inverse at cmsmtrx.c:129:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

static void init_matrices_from_data(cmsMAT3* a, cmsMAT3* b, cmsVEC3* v, const uint8_t* data, size_t size) {
    size_t offset = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (offset + sizeof(double) <= size) {
                memcpy(&a->v[i].n[j], data + offset, sizeof(double));
                offset += sizeof(double);
            } else {
                a->v[i].n[j] = 0.0;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (offset + sizeof(double) <= size) {
                memcpy(&b->v[i].n[j], data + offset, sizeof(double));
                offset += sizeof(double);
            } else {
                b->v[i].n[j] = 0.0;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (offset + sizeof(double) <= size) {
            memcpy(&v->n[i], data + offset, sizeof(double));
            offset += sizeof(double);
        } else {
            v->n[i] = 0.0;
        }
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) {
        return 0;
    }

    cmsMAT3 a, b, r, inv;
    cmsVEC3 v, x;
    
    init_matrices_from_data(&a, &b, &v, Data, Size);

    _cmsMAT3per(&r, &a, &b);
    _cmsMAT3eval(&x, &a, &v);
    _cmsMAT3solve(&x, &a, &v);
    _cmsMAT3identity(&a);
    _cmsMAT3isIdentity(&a);
    _cmsMAT3inverse(&a, &inv);

    return 0;
}