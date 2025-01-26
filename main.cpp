#include <iostream>
#include <string>
#include "Steganography_methods/Console_comands.hpp"
#include "TGA_Coder/TGA_Class.hpp"
#include "BMP_Coder/BMP_Class.hpp"

int main(int argc, char * args[]) {

    initializeSignature();

    stegCons::getFlags(argc, args);

    std::cout << '\n';

    while (!stegCons::receivedFlags.empty()){
        stegCons::strflag tmp = stegCons::receivedFlags.front();
        stegCons::receivedFlags.pop();

        switch (tmp.flagMethod) {
            case stegCons::Info: {

                std::string value = stegCons::values.front();
                stegCons::values.pop();

                if (subExtension(value) == "tga") {
                    auto tgaImage = new TGA_Class(value);
                    tgaImage->fileInfo();

                } else if (subExtension(value) == "bmp") {
                    auto bmpImage = new BMP_Class(value);
                    bmpImage->fileInfo();

                }

                break;
                }
            case stegCons::Encrypt: {

                std::string value = stegCons::values.front();
                stegCons::values.pop();
                std::string value2 = stegCons::values.front();
                stegCons::values.pop();

                if (subExtension(value) == "tga") {
                    auto tgaImage = new TGA_Class(value);
                    tgaImage->eFlagActive(value2);

                } else if (subExtension(value) == "bmp") {
                    auto bmpImage = new BMP_Class(value);
                    bmpImage->eFlagActive(value2);

                }

                break;
            }
            case stegCons::Decrypt: {

                std::string value = stegCons::values.front();
                stegCons::values.pop();

                if (subExtension(value) == "tga") {
                    auto tgaImage = new TGA_Class(value);
                    tgaImage->dFlagActive();

                } else if (subExtension(value) == "bmp") {
                    auto bmpImage = new BMP_Class(value);
                    bmpImage->dFlagActive();

                }

                break;
            }
            case stegCons::Check: {

                std::string value = stegCons::values.front();
                stegCons::values.pop();
                std::string value2 = stegCons::values.front();
                stegCons::values.pop();

                if (subExtension(value) == "tga") {
                    auto tgaImage = new TGA_Class(value);
                    tgaImage->cFlagActive(value2);

                } else if (subExtension(value) == "bmp") {
                    auto bmpImage = new BMP_Class(value);
                    bmpImage->cFlagActive(value2);

                }

                break;
            }
            case stegCons::Help: {
                stegCons::helpFlagMethod();
                break;
            }
            default:
                        std::cerr << "Error: wrong flag.";
                break;
        }
    }

}