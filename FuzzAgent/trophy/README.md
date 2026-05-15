# FuzzAgent Trophy

We followed a responsible-disclosure policy for all bugs uncovered in this study. For each potential bug reported by FuzzAgent, we spent averagely two hours on confirming this validity, assessing the impact and suggesting the fixes.

We have reported all confirmed genuine bugs to the community. Since many developers are complaining Github security advisories are overwhelmed by noise reports, we reported bugs to this channel after assessing the impact seriously and reading the security policies carefully. For high-impact vulnerabilities, we adopted 90 days embargo period with the community to mitigate the potential negative effects.

To facilitate your assessment of FuzzAgent's bug detection effectiveness, we provided the detailed information for those already confirmed and public available bugs in this directory. Several security-related bugs are not ready to be disclosed at current time.

| ID | Library | Bug Type | Track Link |
|:---|:--------|:---------|:-----------|
| 1 | zlib | Integer Overflow | [#1222](https://github.com/madler/zlib/issues/1222) |
| 2 | c-ares | Integer Overflow | [#1144](https://github.com/c-ares/c-ares/issues/1144) |
| 3 | liblouis | SEGV | [#1976](https://github.com/liblouis/liblouis/issues/1976) |
| 4 | liblouis | Buffer Overflow | Embargo |
| 5 | liblouis | SEGV | [#1978](https://github.com/liblouis/liblouis/issues/1978) |
| 6 | liblouis | Buffer Overflow | [#1980](https://github.com/liblouis/liblouis/issues/1980) |
| 7 | liblouis | Buffer Overflow | [#1982](https://github.com/liblouis/liblouis/issues/1982) |
| 8 | liblouis | Memory Leak | [#1986](https://github.com/liblouis/liblouis/issues/1986) |
| 9 | liblouis | Integer Overflow | [#1984](https://github.com/liblouis/liblouis/issues/1984) |
| 10 | liblouis | SEGV | [#1987](https://github.com/liblouis/liblouis/issues/1987) |
| 11 | libpcap | Address Misalignment | [#1634](https://github.com/the-tcpdump-group/libpcap/issues/1634) |
| 12 | libpcap | Assertion Failure | Embargo |
| 13 | lcms | Documentation | [#546](https://github.com/mm2/Little-CMS/issues/546) |
| 14 | lcms | Documentation | [#545](https://github.com/mm2/Little-CMS/issues/545) |
| 15 | lcms | NULL Pointer | [#544](https://github.com/mm2/Little-CMS/issues/544) |
| 16 | libvpx | Assertion Failure | [#506017558](https://issues.webmproject.org/u/1/issues/506017558) |
| 17 | libvpx | Integer Overflow | [#505947426](https://issues.webmproject.org/u/1/issues/505947426) |
| 18 | libvpx | Buffer Overflow | [#505947421](https://issues.webmproject.org/u/1/issues/505947421) (Embargo) |
| 19 | libvpx | Assertion Failure | [#505947417](https://issues.webmproject.org/u/1/issues/505947417) (Embargo) |
| 20 | libvpx | SEGV | [#505902439](https://issues.webmproject.org/u/1/issues/505902439) |
| 21 | libvpx | Integer Overflow | [#505902436](https://issues.webmproject.org/u/1/issues/505902436) |
| 22 | libvpx | SEGV | [#505830937](https://issues.webmproject.org/u/1/issues/505830937) (Embargo) |
| 23 | libvpx | Buffer Overflow | [#505902433](https://issues.webmproject.org/u/1/issues/505902433) (Embargo) |
| 24 | libvpx | SEGV | [#505665625](https://issues.webmproject.org/u/1/issues/505665625) (Embargo) |
| 25 | libvpx | Assertion Failure | [#505665619](https://issues.webmproject.org/u/1/issues/505665619) |
| 26 | libvpx | Buffer Overflow | [#505665613](https://issues.webmproject.org/u/1/issues/505665613) (Embargo) |
| 27 | libvpx | Integer Overflow | [#505631147](https://issues.webmproject.org/u/1/issues/505631147) |
| 28 | libvpx | Integer Overflow | [#505623919](https://issues.webmproject.org/u/1/issues/505623919) |
| 29 | libvpx | SEGV | [#505286909](https://issues.webmproject.org/u/1/issues/505286909) (Embargo) |
| 30 | libvpx | Integer Overflow | [#505286908](https://issues.webmproject.org/u/1/issues/505286908) |
| 31 | libvpx | SEGV | [#505286906](https://issues.webmproject.org/u/1/issues/505286906) |
| 32 | libvpx | SEGV | [#505286904](https://issues.webmproject.org/u/1/issues/505286904) |
| 33 | libvpx | Integer Overflow | [#505286902](https://issues.webmproject.org/u/1/issues/505286902) |
| 34 | libvpx | Buffer Overflow | [#505286900](https://issues.webmproject.org/u/1/issues/505286900) |
| 35 | libvpx | Integer Overflow | [#505215075](https://issues.webmproject.org/u/1/issues/505215075) |
| 36 | libvpx | Integer Overflow | [#505208797](https://issues.webmproject.org/u/1/issues/505208797) |
| 37 | libvpx | NULL Pointer | [#496881245](https://issues.webmproject.org/u/1/issues/496881245) |
| 38 | libvpx | Integer Overflow | [#496881243](https://issues.webmproject.org/u/1/issues/496881243) |
| 39 | libvpx | Integer Overflow | [#496807676](https://issues.webmproject.org/u/1/issues/496807676) |
| 40 | libvpx | Buffer Overflow | [#441668134](https://issues.webmproject.org/issues/441668134) (Embargo) |
| 41 | libvpx | Buffer Overflow | [#442161280](https://issues.webmproject.org/issues/442161280) (Embargo) |
| 42 | libvpx | SEGV | [#442165459](https://issues.webmproject.org/issues/442165459) (Embargo) |
| 43 | libaom | Assertion Failure | [#503987489](https://aomedia.issues.chromium.org/u/1/issues/503987489) |
| 44 | libaom | Assertion Failure | [#503993976](https://aomedia.issues.chromium.org/u/1/issues/503993976) |
| 45 | libaom | Integer Overflow | [#503810640](https://aomedia.issues.chromium.org/u/1/issues/503810640) |
| 46 | libaom | Integer Overflow | [#503689686](https://aomedia.issues.chromium.org/u/1/issues/503689686) (Embargo) |
| 47 | libaom | SEGV | [#503326506](https://aomedia.issues.chromium.org/u/1/issues/503326506) (Embargo) |
| 48 | libaom | Integer Overflow | [#503197490](https://aomedia.issues.chromium.org/u/1/issues/503197490) |
| 49 | libaom | Integer Overflow | [#503187935](https://aomedia.issues.chromium.org/u/1/issues/503187935) (Embargo) |
| 50 | libaom | Integer Overflow | [#503171644](https://aomedia.issues.chromium.org/u/1/issues/503171644) (Embargo) |
| 51 | libaom | Buffer Overflow | [#503197473](https://aomedia.issues.chromium.org/u/1/issues/503197473) (Embargo) |
| 52 | libaom | Buffer Overflow | [#503171639](https://aomedia.issues.chromium.org/u/1/issues/503171639) (Embargo) |
| 53 | libaom | Integer Overflow | [#503013394](https://aomedia.issues.chromium.org/u/1/issues/503013394) |
| 54 | libaom | Integer Overflow | [#502933723](https://aomedia.issues.chromium.org/u/1/issues/502933723) |
| 55 | libaom | Buffer Overflow | [#504317456](https://aomedia.issues.chromium.org/issues/504317456) (Embargo) |
| 56 | libaom | Buffer Overflow | [#503993985](https://aomedia.issues.chromium.org/issues/503993985) (Embargo) |
| 57 | libaom | Buffer Overflow | [#503993984](https://aomedia.issues.chromium.org/issues/503993984) (Embargo) |
| 58 | libaom | Buffer Overflow | [#503975732](https://aomedia.issues.chromium.org/issues/503975732) (Embargo) |
| 59 | libaom | SEGV | [#503689685](https://aomedia.issues.chromium.org/issues/503689685) (Embargo) |
| 60 | libaom | SEGV | [#503691210](https://aomedia.issues.chromium.org/issues/503691210) |
| 61 | libaom | Abort | [#471031723](https://aomedia.issues.chromium.org/issues/471031723) |
| 62 | libaom | Buffer Overflow | [#471095598](https://aomedia.issues.chromium.org/issues/471095598) |
| 63 | protobuf | SEGV | [#27099](https://github.com/protocolbuffers/protobuf/issues/27099) |
| 64 | protobuf | NULL Pointer | [#26546](https://github.com/protocolbuffers/protobuf/issues/26546) |
| 65 | OpenSSL | NULL Pointer | [#30919](https://github.com/openssl/openssl/issues/30919) |
| 66 | OpenSSL | NULL Pointer | [#30922](https://github.com/openssl/openssl/issues/30922) |
| 67 | OpenSSL | NULL Pointer | [#30921](https://github.com/openssl/openssl/issues/30921) |
| 68 | OpenSSL | Type Mismatch | [#29617](https://github.com/openssl/openssl/issues/29617) |
| 69 | OpenSSL | Type Mismatch | [#29616](https://github.com/openssl/openssl/issues/29616) |
| 70 | OpenSSL | Type Mismatch | [#29615](https://github.com/openssl/openssl/issues/29615) |
| 71 | OpenCV | Type Mismatch | [#28580](https://github.com/opencv/opencv/issues/28580) |
| 72 | OpenCV | Misaligned Address | [#28597](https://github.com/opencv/opencv/issues/28597) |
| 73 | OpenCV | Integer Overflow | [#28598](https://github.com/opencv/opencv/issues/28598) |
| 74 | OpenCV | Integer Overflow | [#28926](https://github.com/opencv/opencv/issues/28926) |
| 75 | OpenCV | Type Mismatch | [#28928](https://github.com/opencv/opencv/issues/28928) |
| 76 | OpenCV | Integer Overflow | [#28930](https://github.com/opencv/opencv/issues/28930) |
| 77 | OpenCV | Integer Overflow | [#28940](https://github.com/opencv/opencv/issues/28940) |
| 78 | OpenCV | Buffer Overflow | [#28941](https://github.com/opencv/opencv/issues/28941) |
| 79 | pugixml | Assertion Failure | [#713](https://github.com/zeux/pugixml/issues/713) |