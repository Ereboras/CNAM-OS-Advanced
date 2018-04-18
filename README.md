# The SHELLTAN

## Generating files

To compile, you just need to use **make** at the root of the project. Then 
For doc generation, use **make doc**.
For gcov, use **make gcov** after installing lcov.  

## Executing

Since we did the batch & interactive mode, you can execute commands by 2 ways :

Interactive mode :
```bash
./bin/shelltan
```

Batch mode :
```bash
./bin/shelltan -c "ls -al > toto"
```


## Features

Here is the list of the functionnality we implemented or not in the selltan (refer to PDF) :  

Mandatory features:

- [X] FM01 - Executing a simple command
- [x] FM02 - Execute operators: >, <, >>, <<, |, ||, &&, ; (background is not managed)
- [x] FM03 - Built-In commands (cd, echo, pwd, exit)
- [x] FM04 - Log file  

- [X] CT01 - Makefile
- [X] CT02 - typedef.h must have all struct definitions
- [X] CT03 - Separate prototype and implementation methods
- [X] CT04 - Documented code
- [X] CT05 - Manage error with errno

Optional features :

- [X] FO01 - Batch mode
- [ ] FO02 - Environment variables
- [ ] FO03 - Alias  

- [X] CTO01 - Use doxygen
- [X] CTO02 - Use gcov
- [X] CTO02 - User manual

## Known bugs

When using double redirection in one line, the result displayed will be the second command (not the third). 
However, the correct result is present in the tmp_command file.

## Authors

Emilie Abdoul Malik
Mohamed Azzouz