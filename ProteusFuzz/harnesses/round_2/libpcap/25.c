// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static int is_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    if (data[size - 1] == '\0') return 1;
    return 0;
}

static const char *get_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    if (data[size - 1] == '\0') return (const char *)data;
    
    // Create a temporary null-terminated string if input isn't
    static char buffer[4096];
    size_t copy_len = (size < sizeof(buffer) - 1) ? size : sizeof(buffer) - 1;
    memcpy(buffer, data, copy_len);
    buffer[copy_len] = '\0';
    return buffer;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *input_str = get_safe_string(Data, Size);
    if (input_str == NULL) return 0;

    // 1. pcap_nametollc
    int llc_id = pcap_nametollc(input_str);
    (void)llc_id; // Suppress unused variable warning, result is checked by logic

    // 2. pcap_nametoportrange
    int start_port, end_port, proto_id;
    int range_result = pcap_nametoportrange(input_str, &start_port, &end_port, &proto_id);
    (void)range_result;

    // 3. pcap_tstamp_type_name_to_val
    int tstamp_val = pcap_tstamp_type_name_to_val(input_str);
    (void)tstamp_val;

    // 4. pcap_nametoproto
    int proto_num = pcap_nametoproto(input_str);
    (void)proto_num;

    // 5. pcap_nametoeproto
    int eproto_num = pcap_nametoeproto(input_str);
    (void)eproto_num;

    // 6. pcap_nametoport
    int port_num, port_proto;
    int port_result = pcap_nametoport(input_str, &port_num, &port_proto);
    (void)port_result;

    return 0;
}