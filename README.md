# Project steganography

This program allows you to hide messages in supported image files. The supported formats are uncompressed TGA and BMP files.
To run this program, you first need to build it. Then, in the console, navigate to the directory where the executable file exists and run it by using its name. This will display the startup message along with the available flags.
If you want to use one of these flags, enter the executable file's name in the console, followed by the flag and its parameters. For example, to decrypt a hidden message, use:
```
name_of_exe_file -d "path to file"
```

Flag list:
1. -i, --info → Displays information about the file. Takes the file path as an argument.
2. -e, --encrypt → Hides a message in the file. Takes the file path and the message as arguments.
3. -d, --decrypt → Reads a hidden message if it exists. Takes the file path as an argument.
4. -c, --check → Takes the file path and a message, then checks if the given message can be hidden in the file.
5. -h, --help → Displays instructions.
