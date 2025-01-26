# Project steganography

This program allow you to hide message in supported images file. The supported types are not compressed TGA and BMP files. To run this program first you need to build it and then in console go to the directory where exe file exist and use name of exe file. This will show you the starting message with available flags. If you want to use one of these flags just write in console name of exe file with the flag and it's parameters. For example for decrypting the hidden message you write:
```
name_of_exe_file -d "path to file"
```

Flag list:
1. -i, --info -> display information about file. As argument takes file path.
2. -e, --encrypt -> hide message in file. Take path to file and message.
3. -d, --decrypt -> read hidden message if exist. Takes path to file as argument.
4. -c, --check -> takes path to file and message, then check if given message can be hidden in file.
5. -h, --help -> display instruction.