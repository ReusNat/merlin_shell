#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "functions.h"

/* Coding assistant: Merlin
    |\__/,|   (`\
  _.|o o  |_   ) )
-(((---(((--------
*/

int main() {
    memset(command, '\0', 256);

    // Keep going and taking input until break
     while (1) {
         // Print prompt, take input, get arg array
         print_prompt();
         memset(command, '\0', 256); // Reset command to null bytes to ensure I get what I want

         read(0, command, 256);
         check_reder();
         get_argv();

         // This series of if statements should be self-explanatory
         if (strcmp(argv[0], "exit") == 0){
             break;
         }else if (strcmp(argv[0], "pwd") == 0) {
             if (output || append) {
                 pwd_outfile();
                 output=0, input=0, append=0;
             } else {
                 pwd();
             }
         } else if (strcmp(argv[0], "cd") == 0){
             cd(argv[1]);
         } else if(strcmp(argv[0], "debugargs") == 0) {
             debugargs();
         } else if(strcmp(argv[0], "meow") == 0){
             if (output || append) {
                 merlin_outfile();
                 output=0, input=0, append=0;
             } else {
                 print_merlin();
             }
         }else {
             exec();
             output=0, input=0, append=0;
         }

         // Free the args, start process again
         free_argv();
    }
     // FREE THE ARGS!!
     free_argv();
     return 0;
}
/* Merlin got scared
 _._     _,-'""`-._
(,-.`._,'(       |\`-/|
    `-.-' \ )-`( , o o)
          `-    \`_`"'-
*/
