#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Coding assistant: Merlin
    |\__/,|   (`\
  _.|o o  |_   ) )
-(((---(((--------
*/

void print(char* to_print) {write(1, to_print, strlen(to_print));}

void println(char* to_print){
    print(to_print);
    print("\n");
}

void print_prompt() {
    char prompt[256];
    getcwd(prompt, sizeof(char)*256);
    write(1, prompt, strlen(prompt));
    write(1, " $ ", 3);
}

void cd(char* dir){
    int result = chdir(dir);
    if (result != 0) {
        println(strerror(errno));
    }
}

void pwd(){
    char path[256];
    getcwd(path, sizeof(char)*256);
    println(path);
}

void debugargs(char** argv, int argc){
    print("cmd: [");
    print(argv[0]);
    print("]\n");
    for (int i=0; i < argc; i++){
        print("arg: [");
        print(argv[i]);
        print("]\n");
    }
}

char** get_argv(char* command, int* argc){
    int commLen = (int)strlen(command);
    *argc = 0;

    /*
     This could be done in one loop, but I wanted the memory to be dynamically allocated
     While it isn't saving a ton of memory, it ensures nothing is wasted
     */

    // Count the number of args in command
    for (int i = 0; i < commLen; i++) {
        if (command[i] != ' ' && command[i] != '\t' && command[i] != '\n') {
            (*argc)++; // If the char isn't whitespace it's an arg
            while (i < commLen && command[i] != ' ' && command[i] != '\t' && command[i] != '\n') {
                i++; // Increment over non-whitespace
            }
        }
    }

    // +1 to account for (null) at the end
    (*argc)++;

    // Allocate memory for the argv array of strings
    char** argv = (char**)malloc((*argc) * sizeof(char*));

    int currIndex = 0;
    int startIndex;

    // Gets rig of white space and stores the rest in argv
    for (int i = 0; i < commLen; i++) {
        if (command[i] != ' ' && command[i] != '\t' && command[i] != '\n') {
            startIndex = i;
            while (i < commLen && command[i] != ' ' && command[i] != '\t' && command[i] != '\n') {
                i++; // Increment over non-whitespace
            }
            int length = i - startIndex;
            argv[currIndex] = (char*)malloc((length + 1) * sizeof(char));

            memcpy(argv[currIndex], command+startIndex, length);
            argv[currIndex][length] = '\0';
            currIndex++;
        }
    }

    // Adds null to the end of the array to signify the end of array
    argv[*argc - 1] = (char*)malloc((7) * sizeof(char));
    memcpy(argv[*argc - 1], "(null)\0", 7);

    return argv;
}

// Since I malloc space for each arg I need to free each of them
void free_argv(char** argv, int argc) {
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
}

int main() {
    char command[256];
    memset(command, '\0', 256);
    int argc = 0;

    char** argv;

    // Keep going and taking input until break
     while (1) {

         // Print prompt, take input, get arg array
         print_prompt();
         memset(command, '\0', 256); // Reset command to null bytes to ensure I get what I want
         read(0, command, 256);
         argv = get_argv(command, &argc);

         // This series of if statements should be self-explanatory
         if (strcmp(argv[0], "exit") == 0){
             if (argc == 2) {
                 break;
             } else {
                 println("Too many arguments");
             }
         }else if (strcmp(argv[0], "pwd") == 0) {
             if (argc == 2) {
                 pwd();
             } else {
                 println("Too many arguments");
             }
         } else if (strcmp(argv[0], "cd") == 0){
             if (argc == 3) {
                 cd(argv[1]);
             } else if (argc == 2){
                 println("Not enough arguments");
             } else {
                 println("Too many arguments");
             }

         } else if(strcmp(argv[0], "debugargs") == 0) {
             debugargs(argv, argc);
         } else if (strcmp(argv[0], "(null)") != 0) {
             // Quick google search showed that #38 is "Function not implemented"
             // thought that was fitting for now
             println(strerror(38));
         }

         // Free the args, start process again
         free_argv(argv, argc);
    }
     // FREE THE ARGS!!
     free_argv(argv, argc);
     return 0;
}
/* Merlin got scared
 _._     _,-'""`-._
(,-.`._,'(       |\`-/|
    `-.-' \ )-`( , o o)
          `-    \`_`"'-
*/