// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametoportrange at nametoaddr.c:437:1 in namedb.h
// pcap_nametoeproto at nametoaddr.c:585:1 in namedb.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_nametollc at nametoaddr.c:609:1 in namedb.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
// pcap_nametoproto at nametoaddr.c:475:1 in namedb.h
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

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Fuzz pcap_nametoportrange
    int start_port, end_port, proto_range;
    pcap_nametoportrange(input_str, &start_port, &end_port, &proto_range);

    // 2. Fuzz pcap_nametoeproto
    int eproto_val = pcap_nametoeproto(input_str);
    (void)eproto_val;

    // 3. Fuzz pcap_tstamp_type_name_to_val
    int tstamp_val = pcap_tstamp_type_name_to_val(input_str);
    (void)tstamp_val;

    // 4. Fuzz pcap_nametollc
    int llc_val = pcap_nametollc(input_str);
    (void)llc_val;

    // 5. Fuzz pcap_nametoport
    int port, proto_port;
    pcap_nametoport(input_str, &port, &proto_port);

    // 6. Fuzz pcap_nametoproto
    int proto_val = pcap_nametoproto(input_str);
    (void)proto_val;

    free(input_str);
    return 0;
}