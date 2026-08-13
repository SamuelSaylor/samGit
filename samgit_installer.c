#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

size_t write_callback(void *data, size_t size, size_t nmemb, void *user){

    return fwrite(data,size,nmemb,(FILE*)user);
}

int fetch(const char *url, const char *path){
    CURL *curl = curl_easy_init();

    if(curl){
        FILE *file = fopen(path, "wb");
        if(!file){
            printf("Error editing file.");
            curl_easy_cleanup(curl);
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        //added this n case githubs raw data decices to retrace to another page for whatever reason
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode ret = curl_easy_perform(curl);

        fclose(file);
        curl_easy_cleanup(curl);

        if (ret!=CURLE_OK){
            printf("Issue fetching url: %s (%s)\n", url, curl_easy_strerror(ret));
            remove(path);
            return 1;
        }

        return 0;
    }

    printf("Issue fetching url: %s", url);
    return 1;
}

int main(){
    curl_global_init(CURL_GLOBAL_DEFAULT);

    char response;
    printf("Install samGit? Y/N");
    scanf(" %c", &response);toupper(response);

    if(response != 'Y'){
        printf("samGit will not be installed. If you change your mind, run the installer again.");
        return 1;
    }

    printf("Installing samGit...");
    //TODO language packs and whatnot
    fetch("https://raw.githubusercontent.com/SamuelSaylor/samGit/main/samgit.c", "samgit.c");
    printf("samGit successfully installed.");
    curl_global_cleanup();
    return 0;
}