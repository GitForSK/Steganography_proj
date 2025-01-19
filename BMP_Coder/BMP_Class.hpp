//
// Created by Konrad on 03.01.2022.
//

#ifndef PROJEKT_STEGANOGRAFIA_BMP_CLASS_HPP
#define PROJEKT_STEGANOGRAFIA_BMP_CLASS_HPP

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <ctime>
#include <vector>
#include <bitset>
#include <algorithm>
#include <iterator>
#include "../Steganography_methods/Methods_file.hpp"
#include "../Steganography_methods/crypt_methods.hpp"

struct BMP_file_header {
    /** value that represents file type   **/
    uint16_t file_type;
    /**  size of file   **/
    uint32_t file_size;
    /**  value that represent index in fstream
    * from which starts data extraction **/
    uint32_t pixel_Offset;
};

/*!<
 * Struct that represents cut header of BMP file.
 */

struct BMP_file_DIB_Header {
    /**  size of header   **/
    uint32_t size;
    /**  width of file   **/
    int32_t width;
    /**  height of file   **/
    int32_t height;
    /**  number of bits per pixel   **/
    uint16_t bit_count;
    /**  value 0-3 uncompressed   **/
    uint32_t compression;
    /**  horizontal resolution of file   **/
    int32_t horizontalResolution;
    /**  vertical resolution of file   **/
    int32_t verticalResolution;
};

/*!<
 * Struct that represents cut Dib header of BMP file.
 */

class BMP_Class {

private:

    std::fstream file = std::fstream();
    /**  path to file   **/
    std::string path;

    /**  header of BMP file  **/
    BMP_file_header header;
    /**  dib header of BMP file  **/
    BMP_file_DIB_Header dib;
    /**  image data   **/
    std::vector<uint8_t> data;

    /**
    * Container for stat() method.
    */
    struct stat filestat;

    /**  length of the message that can be hidden in file.
    * It value depends on size divided by 8 with subtraction of signature number
    * of bits plus 32 bits reserved for message length**/
    int maxMessageLength;
    /**  checker that should be false
    * if file doesn't exist or true if exist  **/
    bool exist;

public:

    /**
     * BMP_Class
     *
     * Check if extension is correct and if file exists. Then extract the data.
     * If incorrect print error and block object by setting exist = false.
     *
     * @param givenPath  -> path to image
     */

    BMP_Class(std::string givenPath){

        path = givenPath;
        file.open(path);

        if (subExtension(path) != "bmp") {
            exist = false;
            file.close();
            std::cerr << "Error: Wrong extension/incorrect path." << '\n';
        } else if (file){
            exist = true;
            file.close();
            extractData();
        } else {
            exist = false;
            file.close();
            std::cerr << "Error: File does not exist" << '\n';
        }
    }

    /**
    * BMP_Class
    * Passes file information to cout.
    */

    void fileInfo() {

        if (!exist){
            return;
        }

        std::cout <<
                  "Type: BMP"  << '\n' <<
                  "Size: " << header.file_size << '\n' <<
                  "Image size: " << dib.width << " x " << dib.height << '\n' <<
                  "Resolution: " << dib.horizontalResolution << " x " << dib.verticalResolution << '\n' <<
                  "Modification date: " << ctime(&filestat.st_mtime) << '\n';
    }

    /**
     * BMP_Class
     *
     * actives --check flag.
     *
     * @param mess -> string that contains message from user
     */

    void cFlagActive(const std::string &mess) {
        if (!exist){
            return;
        }
        checkIfMessageComparable(mess, maxMessageLength, file, path,
                                 header.pixel_Offset);
    }

    /**
     * BMP_Class
     *
     * actives --encrypt flag.
     *
     * @param mess -> string that contains message from user
     */

    void eFlagActive(const std::string &mess) {
        if (!exist){
            return;
        }
        writeHiddenMessage(mess, maxMessageLength, data, header.pixel_Offset,
                           file,path);
    }

    /**
     * BMP_Class
     *
     * actives --decrypt flag.
     */

    void dFlagActive() {
        if (!exist){
            return;
        }
        readHiddenMessage(maxMessageLength, data, header.pixel_Offset,
                          file, path);
    }


private:

    /**
     * BMP_Class
     *
     * Extract necessary values from file that are needed to
     * read/write/check hidden message.
     *
     * Important: Check compression if compatible.
     */

    void extractData() {

        stat(path.data(), &filestat);

        file.open(path, std::ios_base::binary | std::ios_base::in);

        file.read((char *) &(header.file_type), 2);
        file.read((char *) &(header.file_size), 4);
        file.seekg(4, std::ios_base::cur);
        file.read((char *) &(header.pixel_Offset), 4);

        file.read((char *) &(dib.size), 4);
        file.read((char *) &(dib.width), 4);
        file.read((char *) &(dib.height), 4);
        file.seekg(2, std::ios_base::cur);
        file.read((char *) &(dib.bit_count), 2);
        file.read((char *) &(dib.compression), 4);
        file.seekg(4, std::ios_base::cur);
        file.read((char *) &(dib.horizontalResolution), 4);
        file.read((char *) &(dib.verticalResolution), 4);

        file.seekg(header.pixel_Offset);

        data.resize(dib.height * dib.width * dib.bit_count / 8);


        file.read((char *)data.data(), data.size());

        maxMessageLength = (data.size()/8) - ((cryptValues::signature.size() + 32));

        if (dib.compression > 3){
            exist = false;
            std::cerr << "Incompatible type" << '\n';
        }

        file.close();
    }

};

/*!<
 * BMP_Class
 *
 * Class that disassembling BMP file and extract data from this.
 * It dissembling it to BMP_file_header, BMP_file_DIB_Header and
 * data that contain pixel information.
 *
 * Contain methods that show file info and active flags from command lines
 */


#endif //PROJEKT_STEGANOGRAFIA_BMP_CLASS_HPP
