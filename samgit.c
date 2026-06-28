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

#define maximum_hashes 100 //change to be number of files that commit will read
#define maximum_files 100 //change for filenames in commit

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

    //header format = blob + size_in_bytes + \0
    //blobs basically contain data from a file without getting outside info like location and other metrics

    char header[64];
    int header_size = sprintf(header, "blob %zu",bytesInBuffer)+1;
    size_t total_size = bytesInBuffer+header_size;

    char *totalsize = malloc(total_size);

    if(totalsize == NULL){
        fprintf(stderr,"Memory allocation failed");
        free(buffer);
        fclose(file);
        return 1;
    }

    memcpy(totalsize,header,header_size);
    memcpy(totalsize+header_size, buffer,bytesInBuffer);

    /////////////////////////////////////////////////////////

    //fun fact this SHA1 hash generates the 7 digit string attached to all your github uploads, its a shortened version of a 40 character hex
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)totalsize,total_size,hash);

    char hex[41];//41 for th null poitner part, %02x is for hexidecimal form
    for(int i = 0; i<SHA_DIGEST_LENGTH;i++){sprintf(hex+i*2,"%02x",hash[i]);}
    hex[40] = '\0';

    //first two characters in hexidecimal is folder location
    //the next few th is the filename
    char folder[3]; 
    char filename[39];

    strncpy(folder,hex,2); folder[2] = '\0';
    strncpy(filename,hex+2,38); filename[38] = '\0';

    char folder_Path[256];
    char object_path[256];

    sprintf(folder_Path,".samgit/objects/%s",folder);
    sprintf(object_path,".samgit/objects/%s/%s",folder,filename);

    create_folder(folder_Path);

    uLongf compressed_size = compressBound(total_size);
    char *compressed = malloc(compressed_size);
    compress((Bytef*)compressed, &compressed_size, (Bytef*)totalsize, total_size);
    
    FILE *ret = fopen(object_path,"wb");
    fwrite(compressed,1,compressed_size,ret);
    fclose(ret);
    
    FILE *index = fopen(".samgit/index", "a");
    fprintf(index, "%s %s\n", hex, filepath);
    fclose(index);

    free(buffer);
    free(totalsize);
    free(compressed);

    printf("File added.");

    return 0;
}

int commit(const char *message){
    char line[512];
    char hashes[maximum_hashes][41];
    char filenames[maximum_files][256];
    int file_cnt = 0;

    FILE *index = fopen(".samgit/index","r");
    if(index == NULL){fprintf(stderr, "Nothing staged, make sure you run add first.");return 1;}

    while(fgets(line,sizeof(line),index)){
        sscanf(line, "%40s %255s", hashes[file_cnt],filenames[file_cnt]);
        //%40s means parse the string line, which has a maximmum of 40 characters
        //%255s means the secondword of hashes is 255 characters max.
        //note to self, keepin mind the 41 and 256 is because of the null pointer
        file_cnt++;
    }

    fclose(index);

    size_t tree_size = 0;
    //each entry is 100644 (unix mode) + filename + null term + 20 raw bytes
    for(int i = 0; i < file_cnt; i++){tree_size+=7+strlen(filenames[i])+21;}//20, plus 1 cuz null temrinator
    
    char *tree_body = malloc(tree_size);
    if(tree_body == NULL){fprintf(stderr,"Error handling tree body size."); return 1;}

    size_t offset = 0;
    for(int i = 0; i < file_cnt; i++){
        memcpy(tree_body+offset,"100644 ",7);
        offset+=7;

        size_t len = strlen(filenames[i]);
        memcpy(tree_body + offset, filenames[i], len);
        offset += len;
        tree_body[offset]='\0';
        offset++;

        // convert hex string back to 20 raw bytes
        unsigned char raw_hash[20];
        for(int j = 0; j < 20; j++){sscanf(hashes[i]+j*2,"%02x",(unsigned int*)&raw_hash[j]);}
        memcpy(tree_body+offset,raw_hash,20);
        offset+=20;
    }

    char tree_header[64];
    int header_size = sprintf(tree_header, "tree %zu", tree_size) + 1;
    size_t total_size = tree_size+header_size;

    char *totalsize = malloc(total_size);

    if(totalsize == NULL){
        fprintf(stderr,"Memory allocation failed");
        return 1;
    }

    memcpy(totalsize,tree_header,header_size);
    memcpy(totalsize + header_size, tree_body, tree_size);

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)totalsize,total_size,hash);

    char hex[41];
    for(int i = 0; i<SHA_DIGEST_LENGTH;i++){sprintf(hex+i*2,"%02x",hash[i]);}
    hex[40] = '\0';

    char folder[3]; 
    char filename[39];

    strncpy(folder,hex,2); folder[2] = '\0';
    strncpy(filename,hex+2,38); filename[38] = '\0';

    char folder_Path[256];
    char object_path[256];

    sprintf(folder_Path,".samgit/objects/%s",folder);
    sprintf(object_path,".samgit/objects/%s/%s",folder,filename);

    create_folder(folder_Path);

    uLongf compressed_size = compressBound(total_size);
    char *compressed = malloc(compressed_size);
    compress((Bytef*)compressed, &compressed_size, (Bytef*)totalsize, total_size);

    FILE *ret = fopen(object_path,"wb");
    fwrite(compressed,1,compressed_size,ret);
    fclose(ret);
}

int main(int argc, char *argv[]){
    if(argc<2){
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1],"init")==0){return init();}

    if(strcmp(argv[1],"add")==0){
        if(argc < 3){ fprintf(stderr,"Usage: %s add <file>\n", argv[0]); return 1; }
        return add(argv[2]);
    }

    printf("Unkown command: %s\n",argv[1]);
    return 0;
}