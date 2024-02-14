#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <algorithm>
#include <string>

const long long int INCREMENT = 100000000;

const std::string NEXT_SEED = "2b8e9f51b7491aae5f189449be957b40b1f25ebc283c3b95352491cd260e3c78";
const std::string PATH = "/home/carlos/data/GeneracionHexAndSHA/inicio.txt";

struct Resultado
{
    long long int decimalNumber;
    std::string hexNumber;
    std::string sha256Value;
    bool encontrado;
};

std::string sha256(const std::string &input)
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();

    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1)
    {
        std::cerr << "Error al inicializar SHA256" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (EVP_DigestUpdate(mdctx, input.c_str(), input.size()) != 1)
    {
        std::cerr << "Error al actualizar SHA256" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1)
    {
        std::cerr << "Error al finalizar SHA256" << std::endl;
        exit(EXIT_FAILURE);
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string padLeft(long long int number)
{
    std::string str = std::to_string(number);
    int count = 64 - str.length();
    if (count <= 0)
    {
        return str;
    }
    return std::string(count, '0') + str;
}

std::string decimalToHex(long long int decimal)
{
    std::stringstream ss;
    ss << std::hex << decimal;
    return ss.str();
}

void buscarNumeroConcurrente(long long int inicio, long long int fin, std::mutex &mtx, std::vector<Resultado> &resultados)
{
    for (long long int i = inicio; i <= fin; ++i)
    {
        std::string paddedNumber = padLeft(i);
        std::string hexNumber = decimalToHex(i);
        std::string result = hexNumber;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        std::string value = sha256(result);

        if (value == NEXT_SEED)
        {
            std::lock_guard<std::mutex> lock(mtx);
            resultados.push_back({i, hexNumber, value, true});
            return;
        }
    }
    std::lock_guard<std::mutex> lock(mtx);
    resultados.push_back({0, "", "", false});
}

int main()
{
    std::ifstream file(PATH);
    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo: " << PATH << std::endl;
        return EXIT_FAILURE;
    }
    std::string line;

    if (std::getline(file, line))
    {
        std::istringstream buffer(line);
        long long int number;
        buffer >> number;
        for (int i = 0; i < 1; ++i)
        {

            std::mutex mtx;
            std::vector<Resultado> resultados;

            std::thread t1(buscarNumeroConcurrente, number, number + INCREMENT, std::ref(mtx), std::ref(resultados));
            std::thread t2(buscarNumeroConcurrente, number + INCREMENT, number + 2 * INCREMENT, std::ref(mtx), std::ref(resultados));
            std::thread t3(buscarNumeroConcurrente, number + 2 * INCREMENT, number + 3 * INCREMENT, std::ref(mtx), std::ref(resultados));
            std::thread t4(buscarNumeroConcurrente, number + 3 * INCREMENT, number + 4 * INCREMENT, std::ref(mtx), std::ref(resultados));

            t1.join();
            t2.join();
            t3.join();
            t4.join();

            for (const auto &resultado : resultados)
            {
                if (resultado.encontrado)
                {
                    std::cout << "ENCONTRADO" << std::endl;
                    std::cout << "NUMERO HEXADECIMAL: " << resultado.hexNumber << std::endl;
                    std::cout << "NUMERO DECIMAL: " << resultado.decimalNumber << std::endl;
                    std::cout << "NUMERO sha256: " << resultado.sha256Value << std::endl;
                }
            }

            number += INCREMENT * 4;
            std::ofstream outputFile(PATH);
            outputFile << number << std::endl;
            std::cout << "Todas las goroutines han terminado. index -> " << i << std::endl;
        }
    }

    file.close(); // Cerrar el archivo después de terminar todas las iteraciones.

    return 0;
}
