//
// Created by Konrad on 08.01.2022.
//

#ifndef PROJEKT_STEGANOGRAFIA_CONSOLE_COMANDS_HPP
#define PROJEKT_STEGANOGRAFIA_CONSOLE_COMANDS_HPP

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

namespace stegCons {

    enum forFlags {
        Info,
        Encrypt,
        Decrypt,
        Check,
        Help
    };

    /*!<
     * Enum forFlags
     *
     * Contain flag name functionality
     */

    struct strflag {
        /** flag name **/
        std::string name;
        /** alias name for flag **/
        std::string alias;
        /** number of arguments that flag can take **/
        int argCount;
        /** enum that contains flag functionality **/
        forFlags flagMethod;
    };

    /*!<
     * Contains structure of flag
     */

    const std::vector<strflag> flags = {
            {"--info", "-i", 1, Info},
            {"--encrypt", "-e", 2, Encrypt},
            {"--decrypt", "-d", 1, Decrypt},
            {"--check", "-c", 2, Check},
            {"--help", "-h", 0, Help},
    };

    std::queue<strflag> recivedFlags;
    std::queue<std::string> values;

    /**
     * Passes flag --help content to cout.
     */

    void helpFlagMethod() {
        std::cout << '\n'
                  << "Hi! This program can hide message in image and eventually extract it from that file."
                  << '\n'
                  << "This program accept only uncompressed BMP images and most of TGA images (Accepted data types: 0-10)."
                  << '\n' << '\n'
                  << "Flags:"
                  << '\n'
                  << "-h, --help " << '\n'
                  << "That flag print manual." << '\n' << '\n'
                  << "-i, --info -> Require path to image. " << '\n'
                  << "That flag print image info." << '\n' << '\n'
                  << "-e, --encrypt -> Require path to image and message in message in\n"
                     "quotation marks." << '\n'
                  << "That flag hides message in image" << '\n' << '\n'
                  << "-d, --decrypt -> Require path to image. " << '\n'
                  << "That flag reads hidden message if found." << '\n' << '\n'
                  << "-c, --check -> Require path to image. " << '\n'
                  << "That flag check if you can hide message in image." << '\n';
    }

    /**
     * Gather flags to queue recivedFlags and thier values to
     * queue values. Check if flags and arguments were properly
     * given, if not print error and exit program.
     *
     * @param argc -> number of argument from command line
     * @param args -> table of pointers to arguments from command line
     */

    void getFlags(int argc, char * args[]) {
        if (argc == 1){
            helpFlagMethod();
            exit(0);
        } else {

            for (int i = 1; i < argc; ++i) {

                auto foundFlag = std::find_if(flags.begin(), flags.end(),
                                              [args, i] (const strflag &flg) {
                                                  return flg.name == args[i] || flg.alias == args[i];
                                              });

                if (foundFlag != flags.end()){
                    recivedFlags.push(*foundFlag);

                    for (int j = 0; j < foundFlag->argCount; ++j) {
                        ++i;

                        if (args[i] == nullptr){
                            std::cerr << "Lack of argument. Use '-h' or '--help' for specification." << '\n';
                            exit(0);
                        }

                        values.push(args[i]);
                    }

                } else {
                    std::cerr << "Incorrect flag. Use '-h' or '--help' for specification." << '\n';
                    exit(0);
                }
            }

        }
    }

}

#endif //PROJEKT_STEGANOGRAFIA_CONSOLE_COMANDS_HPP
