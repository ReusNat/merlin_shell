#ifndef HW_3_FUNCTIONS_H
#define HW_3_FUNCTIONS_H
char** argv;
char infile[256], outfile[256];
int input = 0, output = 0, append = 0;
char command[256];
int argc = 0;

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

void print_merlin() {
    println("      |\\      _,,,---,,_");
    println("ZZZzz /,`.-'`'    -.  ;-;;,_");
    println("     |,4-  ) )-,_. ,\\ (  `'-'");
    println("    '---''(_/--'  `-'\\_)  ");
}

void merlin_outfile() {
    char path[256];
    getcwd(path, sizeof(char) * 256);
    pid_t pid = fork();

    if(pid == 0) {
        close(1);
        if (output) {
            open(outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        } else if (append) {
            open(outfile, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        }
        println("      |\\      _,,,---,,_");
        println("ZZZzz /,`.-'`'    -.  ;-;;,_");
        println("     |,4-  ) )-,_. ,\\ (  `'-'");
        println("    '---''(_/--'  `-'\\_)  ");
        exit(0);
    } else {
        wait(NULL);
    }
}

void cd(char* dir){
    int result = chdir(dir);
    if (result != 0) {
        println(strerror(errno));
    }
}

void pwd() {
    char path[256];
    getcwd(path, sizeof(char) * 256);
    println(path);
}

void pwd_outfile(){
    char path[256];
    getcwd(path, sizeof(char) * 256);
    pid_t pid = fork();

    if(pid == 0) {
        close(1);
        if (output) {
            open(outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        } else if (append) {
            open(outfile, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
        }
        println(path);
        exit(0);
    } else {
        wait(NULL);
    }
}

void debugargs(){
    print("cmd: [");
    print(argv[0]);
    print("]\n");
    for (int i=0; i < argc; i++){
        print("arg: [");
        print(argv[i]);
        print("]\n");
    }
    print("arg: [");
    println("(null)]");

}

void get_argv(){
    int commLen = (int)strlen(command);
    argc = 0;

    /*
     This could be done in one loop, but I wanted the memory to be dynamically allocated
     While it isn't saving a ton of memory, it ensures nothing is wasted
     */

    // Count the number of args in command
    for (int i = 0; i < commLen; i++) {
        if (command[i] != ' ' && command[i] != '\n') {
            (argc)++; // If the char isn't whitespace it's an arg
            while (i < commLen && command[i] != ' ' && command[i] != '\n') {
                i++; // Increment over non-whitespace
            }
        }
    }

    // Allocate memory for the argv array of strings
    argv = (char**)malloc((argc+1) * sizeof(char*));

    int currIndex = 0;
    int startIndex;

    // Gets rig of white space and stores the rest in argv
    for (int i = 0; i < commLen; i++) {
        if (command[i] != ' ' && command[i] != '\n') {
            startIndex = i;
            while (i < commLen && command[i] != ' ' && command[i] != '\n') {
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
    argv[argc] = NULL;
}

// Since I malloc space for each arg I need to free each of them
void free_argv() {
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
}

void check_reder(){
    int i = 1, j = 0;
    char ch = command[0];

    while(ch != '\0') {
        if (ch == '>') {
            command[j] = '\0';
            j++;
            ch = command[j];

            if (ch == '>') append = 1, ch = command[j+1], j++; else output = 1;

            // Increment over spaces
            while(ch == ' ') ch=command[j+1], j++;

            outfile[0] = ch, ch=command[j+1], j++;

            // capture file name
            while (ch != ' ' && ch != '\0' && ch != '<' && ch != '\n') {
                outfile[i] = ch;
                ch = command[j+1];
                i++, j++;
            }
            outfile[i+1] = '\0';
            i = 1;
        } else if (ch == '<') {
            input = 1;
            command[j] = '\0';
            j++;
            ch = command[j];

            // increment over spaces
            while(ch == ' ') ch=command[j+1], j++;
            infile[0] = ch, ch=command[j+1], j++;

            // capture file name
            while (ch != ' ' && ch != '\0' && ch != '>' && ch != '\n') {
                infile[i] = ch;
                ch=command[j+1], i++, j++;
            }
            infile[i+1] = '\0';
            i = 1;
        } else if (input) {
            // increment over spaces
            while (ch == ' ') ch = command[j + 1], j++;
            infile[0] = ch, ch = command[j + 1], j++;

            // capture file name
            while (ch != ' ' && ch != '\0' && ch != '>' && ch != '\n') {
                infile[i] = ch;
                ch = command[j + 1], i++, j++;
            }
            infile[i + 1] = '\0';
        }
        if (ch != '<' && ch != '>') {
            j++;
            ch = command[j];
        }
    }
}

void exec(){
    pid_t pid = fork();
    if (pid == 0) {
        if (input) {
            close(0);
            if(open(infile, O_RDONLY) != 0) println(strerror(errno));
        }
        if (output) {
            close(1);
            open(outfile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
        } else if (append) {
            close(1);
            open(outfile, O_WRONLY|O_CREAT|O_APPEND, S_IRWXU);
        }
        execvp(argv[0], argv);
        //println("Unable to exec: ")
        println(strerror(errno));
        exit(-1);
    } else{
        wait(NULL);
    }
}
#endif //HW_3_FUNCTIONS_H
