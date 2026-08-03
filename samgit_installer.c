#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

size_t write_callback(void *data, size_t size, size_t nmemb, void *user){

    return (size*nmemb);
}

int fetch(const char *url, const char *path){
    CURL *curl = curl_easy_init();

    if(curl){
        FILE *file = fopen(path, "wb");
        if(!file){
            return NULL;}

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        //added this n case githubs raw data decices to retrace to another page for whatever reason
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_perform(curl);

        fclose(file);
        curl_easy_cleanup(curl);
        return 0;
    }

    printf("Issue fetching url: %s", url);
    return 1;
}

int main(){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return 0;
}