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

int add(const char *filepath) {
    /////////////////////////////////////////////////////////
    //putting file into the buffer
    FILE *file = fopen(filepath,"rb");
    if(file == NULL){fprintf(stderr,"Error opening file %s",filepath);return 1;}

    //grabbing file size
    fseek(file,0,SEEK_END);
    long fileSize = ftell(file); //sorry about all the comments but im teaching myself- essentially ftell actually assigns the cursor to the final byte which fseek got.

    if(fileSize<0){
        fprintf(stderr,"Error determining file size: %s",filepath);
        fclose(file);
        return 1;
    }
    rewind(file); //and then this should ove the poitner back tothe beginning.

    char *buffer = (char*)malloc(fileSize+1);
    if(buffer == NULL){
        fprintf(stderr,"Memory allocation failed: %s",filepath);
        fclose(file);
        return 1;
    }

    size_t bytesInBuffer = fread(buffer,1,fileSize,file);
    if((bytesInBuffer<(size_t)fileSize)&&ferror(file)){
        fprintf(stderr,"Error reading file %s",filepath);
        free(buffer);
        fclose(file);
        return 1;
    }
    buffer[bytesInBuffer] = '\0'; //adding a null temrinator
    /////////////////////////////////////////////////////////

    // Step 2: build the blob header "blob {size}\0"
    // hint: sprintf for the header, malloc for the full buffer
    // hint: use memcpy not strcpy — there's a null byte in the middle

    // Step 3: SHA-1 hash the full buffer
    // hint: SHA1(data, length, hash_output)
    // SHA1 gives you 20 raw bytes — convert to hex string with a loop

    // Step 4: build the object path
    // first 2 hex chars = folder, rest = filename
    // .samgit/objects/ab/cdef1234...

    // Step 5: create the folder and write compressed content
    // hint: compress() from zlib, then fwrite
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