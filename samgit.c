#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <zlib.h>

#ifdef _WIN32
    #include <direct.h>
    #define create_folder(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define create_folder(path) mkdir(path, 0777)
#endif

//prereqs above ^^

int init(){ //creares the folders i need and whatnot
    const char *samgit = ".samgit";
    const char *objects = ".samgit/objects";
    const char *refs = ".samgit/refs";
    const char *heads = ".samgit/refs/heads";

    create_folder(samgit);
    create_folder(objects);
    create_folder(refs);
    create_folder(heads);

    FILE *head = fopen(".samgit/HEAD", "w");
    if (!head) { printf("Error creating HEAD\n"); return 1; }
    fprintf(head, "ref: refs/heads/main\n");
    fclose(head);

    return 0;
}

int main(int argc, char *argv[]){
    if(argc<2){
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1],"init")==0){return init();}

    printf("Unkown command: %s\n",argv[1]);
    return 0;
}