#include "password.h"

using namespace std;
using namespace CryptoPP;

#pragma comment(lib, "cryptlib.lib")

string encrypt(const std::string& str_in, const std::string& key, const std::string& iv)
{
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

    CryptoPP::StringSource encryptor(str_in, true,
        new CryptoPP::StreamTransformationFilter(encryption,
            new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(str_out),
                false // do not append a newline
            )
        )
    );
    return str_out;
}

string decrypt(const std::string& str_in, const std::string& key, const std::string& iv)
{
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

    CryptoPP::StringSource decryptor(str_in, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StreamTransformationFilter(decryption,
                new CryptoPP::StringSink(str_out)
            )
        )
    );
    return str_out;
}