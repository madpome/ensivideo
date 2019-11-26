#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include "stream_common.h"
#include "oggstream.h"


int main(int argc, char *argv[]) {
    int res;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s FILE", argv[0]);
	exit(EXIT_FAILURE);
    }
    assert(argc == 2);


    // Initialisation de la SDL
    res = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_EVENTS);
    atexit(SDL_Quit);
    assert(res == 0);

    // start the two stream readers
    pthread_t theora_stream, vorbis_stream;
    pthread_create(&theora_stream, NULL, theoraStreamReader,(void *) argv[1]);
    pthread_create(&vorbis_stream, NULL, vorbisStreamReader,(void *) argv[1]);
    // wait audio thread
    pthread_join(vorbis_stream, NULL);
    // 1 seconde de garde pour le son,
    sleep(1);

    // tuer les deux threads videos si ils sont bloqués
    pthread_cancel(theora_stream);
    pthread_cancel(vorbis_stream);
    // attendre les 2 threads videos
    pthread_join(theora_stream,NULL);
    pthread_join(vorbis_stream,NULL);

    exit(EXIT_SUCCESS);
}
