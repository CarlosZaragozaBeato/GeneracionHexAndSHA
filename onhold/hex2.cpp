#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <openssl/evp.h>
#include <boost/multiprecision/cpp_int.hpp>

#include <iostream>
#include <fstream>
#include <string>



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
    std::string folder_name = "sha"; // Nombre de la carpeta

    for (mp::cpp_int current_num = start_num; current_num >= end_num; --current_num) {
        std::string hex_value = intToHexWithPadding(current_num);
        std::string sha256_hash = sha256(hex_value);
        std::string firstTwoChars = sha256_hash.substr(0, 2);
        std::string file_name = folder_name + "/"+firstTwoChars+"/"+sha256_hash+".txt";

        std::cout << file_name;


    //+ sha256_hash + ".txt";
        std::ofstream file(file_name);
        if (file.is_open()) {
            file << hex_value;
            file.close();
        }
    }
}




// principio -> 
//de -> 115792089237316195423570985008687907853269984665640564039457584007913032309305
//hasta ->
int main() {
    mp::cpp_int start_num;//("115792089237316195423570985008687907853269984665640564039457584007913031808558");
    mp::cpp_int end_num;//(  "115792089237316195423570985008687907853269984665640564039457584007913031800000");

    // Variables para almacenar el número y la resta
    std::string numeroStr;
    mp::cpp_int numero, resta;

    // Nombre del archivo de texto
    std::string filename = "estado.txt";

    // Objeto de entrada de archivo
    std::ifstream archivo(filename);

    std::getline(archivo, numeroStr);


    try {
        start_num = mp::cpp_int(numeroStr);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    archivo.close();



    end_num = start_num - 400000;
    resta = end_num-1;
    

    std::ofstream archivo_out(filename, std::ofstream::out | std::ofstream::trunc);
    archivo_out << resta;
    archivo_out.close();


    generate_hex_files(start_num, end_num);


//115792089237316195423570985008687907853269984665640564039457584007913031808558


    return 0;
}
