#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <direct.h>
    #define create_folder(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define create_folder(path) mkdir(path, 0777)
#endif

//prereqs above ^^

int init(){
    const char *samgit = ".samgit";
    const char *objects = "objects";
    const char *refs = "refs";
    const char *heads = "heads";

    create_folder(samgit);
    create_folder(objects);
    create_folder(refs);
    create_folder(heads);

    return 0;
}

int main(){

    init();

    return 0;
}