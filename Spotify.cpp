#include "Spotify.h"

//taken from: https://stackoverflow.com/a/45017565
size_t SpotifyWriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}