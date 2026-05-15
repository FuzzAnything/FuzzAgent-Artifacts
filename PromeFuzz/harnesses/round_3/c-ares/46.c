// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_query_dnsrec at ares_query.c:115:15 in ares.h
// ares_query at ares_query.c:133:6 in ares.h
// ares_mkquery at ares_create_query.c:74:5 in ares.h
// ares_create_query at ares_create_query.c:66:5 in ares.h
// ares_send at ares_send.c:247:6 in ares.h
// ares_search at ares_search.c:431:6 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init_options at ares_init.c:238:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    /* Do nothing */
}

static void dummy_callback_dnsrec(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    /* Do nothing */
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create dummy file if needed */
    write_dummy_file(Data, Size);
    
    /* Initialize ares channel */
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return 0;
    }
    
    /* Extract parameters from fuzz data */
    size_t offset = 0;
    
    /* Get name string (null-terminated) */
    char name[256] = {0};
    size_t name_len = (Size - offset > sizeof(name) - 1) ? sizeof(name) - 1 : Size - offset;
    if (name_len > 0) {
        memcpy(name, Data + offset, name_len);
        name[name_len] = '\0';
        offset += name_len;
    } else {
        strcpy(name, "example.com");
    }
    
    /* Get parameters from remaining data */
    int dnsclass = 1; /* Default IN class */
    int type = 1;     /* Default A record */
    unsigned short id = 0;
    int rd = 1;
    int max_udp_size = 512;
    
    if (Size - offset >= sizeof(int)) {
        memcpy(&dnsclass, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (Size - offset >= sizeof(int)) {
        memcpy(&type, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (Size - offset >= sizeof(unsigned short)) {
        memcpy(&id, Data + offset, sizeof(unsigned short));
        offset += sizeof(unsigned short);
    }
    
    if (Size - offset >= sizeof(int)) {
        memcpy(&rd, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (Size - offset >= sizeof(int)) {
        memcpy(&max_udp_size, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    /* Ensure reasonable values */
    dnsclass = dnsclass & 0xFFFF;
    type = type & 0xFFFF;
    id = id & 0xFFFF;
    rd = rd & 1;
    max_udp_size = (max_udp_size < 512) ? 512 : ((max_udp_size > 4096) ? 4096 : max_udp_size);
    
    /* Test ares_query_dnsrec */
    unsigned short qid = 0;
    ares_dns_class_t dnsclass_enum = (ares_dns_class_t)dnsclass;
    ares_dns_rec_type_t type_enum = (ares_dns_rec_type_t)type;
    ares_query_dnsrec(channel, name, dnsclass_enum, type_enum, dummy_callback_dnsrec, NULL, &qid);
    
    /* Test ares_query */
    ares_query(channel, name, dnsclass, type, dummy_callback, NULL);
    
    /* Test ares_mkquery */
    unsigned char *buf1 = NULL;
    int buflen1 = 0;
    ares_mkquery(name, dnsclass, type, id, rd, &buf1, &buflen1);
    if (buf1) {
        free(buf1);
    }
    
    /* Test ares_create_query */
    unsigned char *buf2 = NULL;
    int buflen2 = 0;
    ares_create_query(name, dnsclass, type, id, rd, &buf2, &buflen2, max_udp_size);
    if (buf2) {
        free(buf2);
    }
    
    /* Test ares_send */
    if (Size > 0) {
        ares_send(channel, Data, (Size > 65535) ? 65535 : (int)Size, dummy_callback, NULL);
    }
    
    /* Test ares_search */
    ares_search(channel, name, dnsclass, type, dummy_callback, NULL);
    
    /* Cleanup */
    ares_destroy(channel);
    
    /* Remove dummy file */
    unlink("./dummy_file");
    
    return 0;
}