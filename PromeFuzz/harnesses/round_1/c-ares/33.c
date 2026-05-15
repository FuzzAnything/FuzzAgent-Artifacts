// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query at ares_query.c:133:6 in ares.h
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ares.h"

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, 
                                  unsigned long qid, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)qid;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    ares_channel_t *channel = NULL;
    int status = ares_init(&channel);
    if (status != ARES_SUCCESS) {
        return 0;
    }

    unsigned char *buf = NULL;
    int buflen = 0;
    unsigned short qid = 0;
    ares_status_t dnsrec_status;
    unsigned short id_val;
    
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    char name_buffer[256];
    const char *name = "example.com";
    int dnsclass = ARES_CLASS_IN;
    int type = ARES_REC_TYPE_A;
    unsigned short id = 12345;
    int rd = 1;
    int max_udp_size = 512;
    
    if (remaining_size > 0) {
        size_t name_len = remaining_size > 255 ? 255 : remaining_size;
        memcpy(name_buffer, remaining_data, name_len);
        name_buffer[name_len] = '\0';
        name = name_buffer;
        
        size_t offset = name_len;
        if (offset < remaining_size) {
            dnsclass = remaining_data[offset] % 65536;
            offset++;
            if (offset < remaining_size) {
                type = remaining_data[offset] % 65536;
                offset++;
                if (offset + 1 < remaining_size) {
                    id_val = (remaining_data[offset] << 8) | remaining_data[offset + 1];
                    id = id_val;
                }
            }
        }
    }

    switch (selector % 6) {
        case 0:
            ares_mkquery(name, dnsclass, type, id, rd, &buf, &buflen);
            if (buf) {
                free(buf);
                buf = NULL;
            }
            break;
            
        case 1:
            if (remaining_size > 0) {
                int qlen = (remaining_size < INT_MAX) ? (int)remaining_size : INT_MAX;
                ares_send(channel, remaining_data, qlen, dummy_callback, NULL);
            }
            break;
            
        case 2:
            ares_search(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
            
        case 3:
            ares_query(channel, name, dnsclass, type, dummy_callback, NULL);
            break;
            
        case 4:
            dnsrec_status = ares_query_dnsrec(channel, name, 
                                             (ares_dns_class_t)dnsclass,
                                             (ares_dns_rec_type_t)type,
                                             dummy_callback_dnsrec, NULL, &qid);
            (void)dnsrec_status;
            break;
            
        case 5:
            ares_create_query(name, dnsclass, type, id, rd, &buf, &buflen, max_udp_size);
            if (buf) {
                free(buf);
                buf = NULL;
            }
            break;
    }

    ares_destroy(channel);
    return 0;
}