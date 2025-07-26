#ifndef SHA512_H
#define SHA512_H
#include <string>
#include <random>
#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>

// 평문 password 에 더할 임의의 문자열을 salting 한다.
std::string const generateSalt(size_t length = 16) {
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    std::string salt;
    for (size_t i = 0; i < length; ++i)
        salt += charset[dist(rng)];

    return salt;
}

// 평문 password + salt == SHA512 해싱 ==> DB(json) 에 저장될 단방향 암호화된 암호 Digest
std::string const sha512(const std::string& input) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512((const unsigned char*)input.c_str(), input.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

#endif // SHA512_H
