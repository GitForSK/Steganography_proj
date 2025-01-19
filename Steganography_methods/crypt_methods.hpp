//
// Created by Konrad on 08.01.2022.
//

#ifndef PROJEKT_STEGANOGRAFIA_CRYPT_METHODS_HPP
#define PROJEKT_STEGANOGRAFIA_CRYPT_METHODS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include "Methods_file.hpp"

namespace cryptValues {

    /** string that start hidden message  **/
    const std::string signature = "ksnr";
    /** vector that stores binary
    * representation of signature e.g. 0101 0101 ->
    * {0, 1, 0, 1, 0, 1, 0, 1}
    **/
    std::vector<int> signatureBinary;
    /**  vector that stores binary
    * representation of message.  **/
    std::vector<int> messageBinarySequence;

}

/**
 * Initialize signature and signatureBinary variables
 */


void initializeSignature() {
    stringBinToVec(cryptValues::signature, cryptValues::signatureBinary);
}

/**
 * Check if last bit of n read element is equal to n element of signatureBinary.
 *
 * @param image -> fstream to read from
 * @param path -> path to file
 * @param dataStart -> image of pixel data
 * @return false if any element id different, else true
 */

bool isMessageHidden(std::fstream &image, const std::string &path, int dataStart) {

    image.open(path, std::ios_base::binary | std::ios_base::in);

    image.seekg(dataStart);

    uint8_t forRead;

    for (int i : cryptValues::signatureBinary) {
        image.read((char *) &forRead, 1);
        if ((forRead & 1) != i){
            image.close();
            return false;
        }
    }

    image.close();

    return true;
}

/**
 * Check if message can fit in image and check if image
 * can contain already hidden image. Then print appropriate message.
 *
 * @param mess -> string that contain a message
 * @param maxMessageLength -> max length of message that image can hide
 * @param image -> fstream
 * @param path -> path to file
 * @param dataStart -> value of index where image data is extracted
 */

void checkIfMessageComparable(std::string mess, int maxMessageLength,
                              std::fstream &image, const std::string &path,
                              int dataStart) {

    if (maxMessageLength <= 0){
        std::cout << "This file is too small to contain a hidden message." << '\n';
    } else if (mess.size() > maxMessageLength){
        std::cout << "Message is too long, file can contain only "
                  << maxMessageLength << " characters." << '\n';
    } else {
        std::cout << "Message is ok! This file can contain "
                  << maxMessageLength << " characters." << '\n';
    }

    if (isMessageHidden(image, path, dataStart)) {
        std::cout << "A message is hidden in this image (99% sure)." << '\n' << '\n';
    } else {
        std::cout << "This image do not contain hidden message." << '\n' << '\n';
    }

}

/**
 * Modifies image data to fit hidden message. Then
 * create copy of file where insert that data and
 * overwrite original file. At the end delete created file.
 *
 * @param mess -> string that contain message
 * @param maxMessageLength -> max length of message that image can hide
 * @param data -> vector that contains image data
 * @param dataStart -> value of index where image data is extracted
 * @param image -> fstream
 * @param path -> path to file
 */

void writeHiddenMessage(const std::string &mess, int &maxMessageLength,
                        std::vector<uint8_t> &data, int dataStart,
                        std::fstream &image,
                        std::string &path) {

    if (mess.size() > maxMessageLength || maxMessageLength <= 0){
        std::cerr << "Error: Message is too long or file is to small." << '\n';
        return;
    }

    std::string tmpPath = "tmp." + subExtension(path);
    image.open(path, std::ios_base::in | std::ios_base::binary);
    auto result = std::fstream(tmpPath, std::ios_base::out | std::ios_base::binary);

    result << image.rdbuf();

    messageToBinVec(mess, cryptValues::signature, cryptValues::messageBinarySequence);

    for (int i = 0; i < cryptValues::messageBinarySequence.size(); ++i) {
        if ((data[i] & 1) != cryptValues::messageBinarySequence[i]){
            data[i] ^= 1;
        }
    }

    result.seekg(dataStart);

    result.write((char *) data.data(), data.size());

    result.flush();
    result.close();
    image.close();

    image.open(path, std::ios_base::out | std::ios_base::binary);
    result.open(tmpPath, std::ios_base::in | std::ios_base::binary);

    image << result.rdbuf();

    image.flush();
    image.close();
    result.close();

    remove((char *) &tmpPath);

    clearIntVec(cryptValues::messageBinarySequence);

    std::cout << "Written." << '\n' << '\n';

}

/**
 * Read data from stream, decode it and print it.
 * Check if image contain hidden message. If false print appropriate message.
 *
 * @param maxMessageLength
 * @param data
 * @param dataStart
 * @param image
 * @param path
 */

void readHiddenMessage(int &maxMessageLength, std::vector<uint8_t> &data, int dataStart,
                       std::fstream &image, std::string &path) {

    if (!isMessageHidden(image, path, dataStart)){
        std::cerr << "Error: File do not contain hidden message." << '\n';
        return;
    }

    image.open(path, std::ios_base::binary | std::ios_base::in);
    image.seekg(dataStart+cryptValues::signatureBinary.size());

    std::vector<uint8_t> messageLengthCoded;
    messageLengthCoded.resize(32);
    image.read((char *) messageLengthCoded.data(), 32);

    uint32_t messSize = 0;
    std::reverse(messageLengthCoded.begin(), messageLengthCoded.end());
    decoder(messSize, messageLengthCoded);

    int numOfBits = messSize*8;

    std::vector<uint8_t> messageToDecode;
    messageToDecode.resize(numOfBits);
    image.read((char *) messageToDecode.data(), numOfBits);

    std::string hiddenMessage;

    uint8_t character = 0;
    std::vector<uint8_t> subVec;

    for (int i = 0; i < messSize; ++i) {
        subVec = std::vector(
                messageToDecode.begin() + i*8,
                messageToDecode.begin() + (i+1)*8
        );

        std::reverse(subVec.begin(), subVec.end());
        decoder(character, subVec);
        hiddenMessage.push_back((char)character);
        character = 0;
    }

    std::cout << "Message contents: " << hiddenMessage << '\n' << '\n';

    image.close();


}

#endif //PROJEKT_STEGANOGRAFIA_CRYPT_METHODS_HPP
