//
// Created by Konrad on 07.01.2022.
//

#ifndef PROJEKT_STEGANOGRAFIA_METHODS_FILE_HPP
#define PROJEKT_STEGANOGRAFIA_METHODS_FILE_HPP

#include <vector>

/**
 * Append given vector by pushing char bits in their integer representation.
 *
 * @param character -> char
 * @param vec -> vector to append
 */

void charBinarySequence(char character, std::vector<int> &vec) {

    for (char c : std::bitset<8>(character).to_string()) {
        if (c == 48)
            vec.push_back(0);
        else
            vec.push_back(1);
    }
}

/**
 * Append given vector by pushing bits of string in their integer representation.
 *
 * @param str -> string
 * @param signVector  -> vector to append
 */

void stringBinToVec (const std::string &str, std::vector<int> &signVector) {
    for (char c : str) {
        charBinarySequence(c, signVector);
    }
}

/**
 * It append vector by pushing bits of hidden message in their integer representation,
 * where frist is signature, second is message length and third is message.
 *
 * @param message -> string that contains user message
 * @param signature -> const string that contains signature
 * @param messageBinVec -> vector to append
 */

void messageToBinVec(const std::string &message, const std::string &signature,
                     std::vector<int> &messageBinVec){

    std::string messSize = std::bitset<32>(message.size()).to_string();

    stringBinToVec(signature, messageBinVec);

    for (char character : messSize) {
        if (character == 48)
            messageBinVec.push_back(0);
        else
            messageBinVec.push_back(1);
    }
    stringBinToVec(message, messageBinVec);

}

/**
 * Clear vector from values
 *
 * @param vec -> vector to clear
 */

void clearIntVec(std::vector<int> &vec) {
    std::destroy(vec.begin(), vec.end());
    vec.clear();
    vec.resize(0);
}

/**
 * Iterate through vector and if n element have last bit set,
 * then set n bit in value.
 *
 * @tparam T -> type that have size of vec
 * @param value -> value where bits will be set
 * @param vec -> vector of bytes
 */

template<typename T>

void decoder(T &value, std::vector<uint8_t> vec) {

    for (int i = 0; i < vec.size(); ++i) {
        if ((vec[i] & 1) == 1){
            value |= (1 << i);
        }
    }
}

/**
 * Take file extension. Work only on files with extension name < 3.
 *
 * @param path -> path to file
 * @return substring that contain 3 last characters of string
 */

std::string subExtension(std::string path) {
    int length = path.size();
    if (length-3 == 0){
        return "";
    }
    return path.substr(length-3, length);
}


#endif //PROJEKT_STEGANOGRAFIA_METHODS_FILE_HPP
