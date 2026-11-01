// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <namedb.h>

static int fuzz_pcap_nametoportrange(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    int port1, port2, proto;
    return pcap_nametoportrange(str, &port1, &port2, &proto);
}

static int fuzz_pcap_nametoeproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    return pcap_nametoeproto(str);
}

static int fuzz_pcap_nametollc(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    return pcap_nametollc(str);
}

static int fuzz_pcap_tstamp_type_name_to_val(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    return pcap_tstamp_type_name_to_val(str);
}

static int fuzz_pcap_nametoproto(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    return pcap_nametoproto(str);
}

static int fuzz_pcap_nametoport(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    char *str = (char *)data;
    int port, proto;
    return pcap_nametoport(str, &port, &proto);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure null termination for string functions
    char *buffer = (char *)malloc(Size + 1);
    if (!buffer) return 0;
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    // Fuzz pcap_nametoportrange
    int p1, p2, proto;
    fuzz_pcap_nametoportrange((const uint8_t *)buffer, Size);

    // Fuzz pcap_nametoeproto
    fuzz_pcap_nametoeproto((const uint8_t *)buffer, Size);

    // Fuzz pcap_nametollc
    fuzz_pcap_nametollc((const uint8_t *)buffer, Size);

    // Fuzz pcap_tstamp_type_name_to_val
    fuzz_pcap_tstamp_type_name_to_val((const uint8_t *)buffer, Size);

    // Fuzz pcap_nametoproto
    fuzz_pcap_nametoproto((const uint8_t *)buffer, Size);

    // Fuzz pcap_nametoport
    fuzz_pcap_nametoport((const uint8_t *)buffer, Size);

    free(buffer);
    return 0;
}