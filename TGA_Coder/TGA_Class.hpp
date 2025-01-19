//
// Created by Konrad on 07.01.2022.
//

#ifndef PROJEKT_STEGANOGRAFIA_TGA_CLASS_HPP
#define PROJEKT_STEGANOGRAFIA_TGA_CLASS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <ctime>
#include <bitset>
#include <algorithm>
#include "../Steganography_methods/Methods_file.hpp"
#include "../Steganography_methods/crypt_methods.hpp"

#pragma pack(push, 1)

struct headerTGA {
    char idlength;                      /**  idlength -> length of a string located after the header.   **/
    char colourmaptype;                 /**  colourmaptype -> determinate if have colour map   **/
    char datatypecode;                  /**  datatypecode -> determinate Targa format.   **/
    short int colourmaporigin;          /**  colourmaporigin -> index of first color map entry   **/
    short int colourmaplength;          /**  colourmaplength -> length of color map   **/
    char colourmapdepth;                /**  colourmapdepth -> number of bits in color map entry   **/
    short int x_origin;                 /**  x_origin -> X coordinate of the lower left corner
                                           *  of the image.**/
    short int y_origin;                 /**  y_origin -> Y coordinate of the lower left corner
                                           *  of the image.**/
    short width;                        /**  width -> width of image   **/
    short height;                       /**  height -> height of image   **/
    char bitsperpixel;                  /**  bitsperpixel ->  number of bits in a pixel   **/
    char imagedescriptor;               /**  imagedescriptor -> image descriptor byte   **/
};

/*!<
 * Structs that represents header of TGA file. It's size should always equal to 18 bytes.
 * Important: param is needed in order to keep data alignment.
 */

#pragma pack(pop)

struct stat filestat;

/*!<
 * Container for stat() method.
 */

class TGA_Class {

private:

    /**  path to file   **/
    std::string path;
    std::fstream image = std::fstream();

    /**  header of TGA file  **/
    headerTGA header;
    /**  image data   **/
    std::vector<uint8_t> data;

    /**  checker that should be false
     * if file doesn't exist or true if exist  **/
    bool exist;

    /**
    * Container for stat() method.
    **/
    struct stat filestat;

    /** size of file   **/
    int size;
    /** value that represent index in fstream
     * from which starts data extraction
     **/
    int dataStart;
    /** length of the message that can be hidden in file.
    * It value depends on size divided by 8 with subtraction of signature number
    * of bits plus 32 bits reserved for message length**/
    int maxMessageLength;

public:

    /**
     * TGA_Class
     *
     * Check if extension is correct and if file exists. Then extract the data.
     * If incorrect print error and block object by setting exist = false.
     *
     * @param givenPath -> path to image
     */

    TGA_Class(std::string givenPath){

        path = givenPath;
        image.open(path);

        if (subExtension(path) != "tga") {
            exist = false;
            image.close();
            std::cerr << "Error: Wrong extension/incorrect path." << '\n';
        } else if (image){
            exist = true;
            image.close();
            extractData();
        } else {
            exist = false;
            image.close();
            std::cerr << "Error: File does not exist" << '\n';
        }
    }

    /**
     * TGA_Class
     *
     * Passes file information to cout.
     */

    void fileInfo() {

        if (!exist){
            return;
        }

        std::cout << "Type: TGA" <<  '\n'
                  << "Image size: " << header.width << " x " << header.height <<  '\n'
                  << "File size: " << size <<  '\n'
                  << "Modification date: " << ctime(&filestat.st_mtime) <<  '\n';
    }

    /**
     * TGA_Class
     *
     * actives --check flag
     *
     * @param mess -> string that contains message from user
     */

    void cFlagActive(const std::string &mess) {
        if (!exist){
            return;
        }
        checkIfMessageComparable(mess, maxMessageLength,image, path, dataStart);
    }

    /**
     * TGA_Class
     *
     * actives --encrypt flag
     *
     * @param mess -> string that contains message from user
     */

    void eFlagActive(const std::string &mess) {
        if (!exist){
            return;
        }
        writeHiddenMessage(mess, maxMessageLength, data, dataStart, image,
                           path);
    }

    /**
     * TGA_Class
     *
     * actives --decrypt flag
     */

    void dFlagActive() {
        if (!exist){
            return;
        }
        readHiddenMessage(maxMessageLength, data, dataStart, image, path);
    }

private:

    /**
     * TGA_Class
     *
     * Extract necessary values from file that are needed to
     * read/write/check hidden message.
     *
     * Important: Check datatype if compatible.
     *
     */

    void extractData(){

        image.open(path, std::ios_base::binary | std::ios_base::in);

        stat(path.data(), &filestat);

        image.read((char *) &header, sizeof header);

        size = 18 + header.idlength + header.colourmaplength;
        dataStart = 18 + header.idlength + header.colourmaplength;

        image.seekg(dataStart);

        uint8_t toPush;

        while (image.read((char *) &toPush, 1)){
            size++;
            data.push_back(toPush);
        }

        maxMessageLength = ((data.size())/8) - cryptValues::signatureBinary.size() - 32 - 26;

        image.close();

        if (header.datatypecode > 11){
            exist = false;
            std::cerr << "Incompatible type" << '\n';
        }

    }


};

/*!<
 * TGA_Class
 *
 * Class that disassembling TGA file and extract data from this.
 * It dissembling it to headerTGA and data that contain pixel information.
 *
 * Contain methods that show file info and active flags from command lines
 */

#endif //PROJEKT_STEGANOGRAFIA_TGA_CLASS_HPP
