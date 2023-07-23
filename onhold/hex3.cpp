#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;



std::string sha256(const std::string& input) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, input.c_str(), input.size());
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}


std::string intToHexWithPadding(const mp::cpp_int& number) {
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::hex << number;
    std::string hex_value = ss.str();

    if (hex_value.length() < 66) {
        hex_value = std::string(66 - hex_value.length(), '0') + hex_value.substr(2);
    }
    return hex_value;
}






void generate_hex_files(const mp::cpp_int& start_num, const mp::cpp_int& end_num) {
    std::string folder_name = "archivos4"; // Nombre de la carpeta
    for (mp::cpp_int current_num = start_num; current_num >= end_num; --current_num) {
        std::string hex_value = intToHexWithPadding(current_num);
        std::string sha256_hash = sha256(hex_value);
        std::string file_name = folder_name + "/" + sha256_hash + ".txt";

        std::ofstream file(file_name);
        if (file.is_open()) {
            file << hex_value;
            file.close();
        }
    }
}



// de -> 115792089237316195423570985008687907853269984665640564039457579001990000000000
int main() {
    mp::cpp_int start_num("115792089237316195423570985008687907853269984665640564039457579001990000000000");
    mp::cpp_int end_num("115792089237316195423570985008687907853269984665640564039457584007913032309304");
    generate_hex_files(start_num, end_num);
    return 0;
}
