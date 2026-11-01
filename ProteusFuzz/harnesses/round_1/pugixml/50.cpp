// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

static std::string generate_xml_from_input(const uint8_t *Data, size_t Size) {
    std::string result = "<root>";
    for (size_t i = 0; i < Size; ++i) {
        if (Data[i] >= 32 && Data[i] < 127) {
            result += static_cast<char>(Data[i]);
        } else {
            char buf[4];
            snprintf(buf, sizeof(buf), "%02x", Data[i]);
            result += buf;
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string xml_str = generate_xml_from_input(Data, Size);

    if (!doc.load_string(xml_str.c_str())) {
        if (!doc.load_string("<root></root>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node v1 = root.child("test");
    // 2. pugi::xml_node::child
    pugi::xml_node v2 = v1.child("test");
    // 3. pugi::xml_node::insert_copy_after
    pugi::xml_node v3 = root.insert_copy_after(v2, v1);
    // 4. pugi::xml_node::child
    pugi::xml_node v4 = v3.child("test");
    // 5. pugi::xml_node::insert_copy_after
    pugi::xml_node v5 = v4.insert_copy_after(v4, v3);
    // 6. pugi::xml_node::child
    pugi::xml_node v6 = v5.child("test");
    // 7. pugi::xml_node::child
    pugi::xml_node v7 = v6.child("test");
    // 8. pugi::xml_node::child
    pugi::xml_node v8 = v7.child("test");
    // 9. pugi::xml_node::child
    pugi::xml_node v9 = v8.child("test");
    // 10. pugi::xml_node::insert_copy_after
    pugi::xml_node v10 = v9.insert_copy_after(v9, v8);
    // 11. pugi::xml_node::child
    pugi::xml_node v11 = v10.child("test");
    // 12. pugi::xml_node::first_child
    pugi::xml_node v12 = v11.first_child();
    // 13. pugi::xml_node::name
    const pugi::char_t* v13 = v12.name();
    (void)v13;
    // 14. pugi::xml_node::child
    pugi::xml_node v14 = v12.child("test");
    // 15. pugi::xml_node::insert_copy_after
    pugi::xml_node v15 = v14.insert_copy_after(v14, v12);
    // 16. pugi::xml_node::child
    pugi::xml_node v16 = v15.child("test");
    // 17. pugi::xml_node::first_child
    pugi::xml_node v17 = v16.first_child();
    // 18. pugi::xml_node::child
    pugi::xml_node v18 = v17.child("test");
    // 19. pugi::xml_node::last_child
    pugi::xml_node v19 = v18.last_child();
    // 20. pugi::xml_node::value
    const pugi::char_t* v20 = v19.value();
    (void)v20;
    // 21. pugi::xml_node::child
    pugi::xml_node v21 = v19.child("test");
    // 22. pugi::xml_node::insert_copy_after
    pugi::xml_node v22 = v21.insert_copy_after(v21, v19);
    // 23. pugi::xml_node::child
    pugi::xml_node v23 = v22.child("test");
    // 24. pugi::xml_node::first_child
    pugi::xml_node v24 = v23.first_child();
    // 25. pugi::xml_node::child
    pugi::xml_node v25 = v24.child("test");
    // 26. pugi::xml_node::first_child
    pugi::xml_node v26 = v25.first_child();
    // 27. pugi::xml_node::value
    const pugi::char_t* v27 = v26.value();
    (void)v27;

    return 0;
}