// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include "pugixml.hpp"

static std::string createSafeString(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "";
    }
    size_t safe_size = size < 100 ? size : 100;
    std::string result(safe_size, '\0');
    for (size_t i = 0; i < safe_size; ++i) {
        char c = static_cast<char>(data[i] % 128);
        if (c == '\0') {
            c = 'a';
        }
        result[i] = c;
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* Data, size_t Size) {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
    }

    std::string xml_content = createSafeString(Data, Size);
    if (xml_content.empty()) {
        xml_content = "<root></root>";
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    if (!result) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    std::string_view name1 = "child1";
    std::string_view name2 = "child2";
    std::string_view name3 = "child3";

    pugi::xml_node node1 = root.child(name1.data());
    pugi::xml_node node2 = root.child(name2.data());
    pugi::xml_node node3 = root.child(name3.data());

    pugi::xml_node temp1 = root.child(name1.data());
    if (temp1 && node2) {
        root.insert_copy_before(temp1, node2);
    }

    pugi::xml_node temp2 = root.child(name2.data());

    if (temp2 && node3) {
        root.insert_copy_before(temp2, node3);
    }

    pugi::xml_node temp3 = root.child(name1.data());
    pugi::xml_node temp4 = root.child(name2.data());
    pugi::xml_node temp5 = root.child(name3.data());
    pugi::xml_node temp6 = root.child(name1.data());

    if (temp6 && node2) {
        root.insert_copy_before(temp6, node2);
    }

    pugi::xml_node temp7 = root.child(name2.data());
    pugi::xml_node temp8 = root.first_child();
    if (temp8) {
        const char* name_ptr = temp8.name();
        (void)name_ptr;
    }

    pugi::xml_node temp9 = root.child(name3.data());

    if (temp9 && node1) {
        root.insert_copy_before(temp9, node1);
    }

    pugi::xml_node temp10 = root.child(name1.data());
    pugi::xml_node temp11 = root.first_child();
    pugi::xml_node temp12 = temp11.child(name2.data());
    pugi::xml_node temp13 = temp12.last_child();
    if (temp13) {
        const char* name_ptr2 = temp13.name();
        (void)name_ptr2;
    }

    pugi::xml_node temp14 = root.child(name2.data());

    if (temp14 && node3) {
        root.insert_copy_before(temp14, node3);
    }

    pugi::xml_node temp15 = root.child(name3.data());
    pugi::xml_node temp16 = root.first_child();
    pugi::xml_node temp17 = temp16.next_sibling();
    pugi::xml_node temp18 = temp17.child(name1.data());
    pugi::xml_node temp19 = temp18.first_child();
    if (temp19) {
        const char* value_ptr = temp19.value();
        (void)value_ptr;
    }

    return 0;
}