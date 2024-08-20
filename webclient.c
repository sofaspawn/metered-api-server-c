//#include <curl/easy.h>
#include <stdio.h>
#include <curl/curl.h>

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata){
    size_t total_size = size*nmemb;
    fwrite(ptr, size, nmemb, stdout);
    return total_size;
}

int main(int argc, char *argv[]){
    if(argc<2){
        printf("USAGE: ./client <api_key>\n");
        return 1;
    }

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, argv[1]);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
