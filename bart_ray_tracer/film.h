//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_FILM_H
#define BART_RAY_TRACER_FILM_H

class Film {
public:
    int width; int height;

    Film() {
        width = 0; height = 0;
    }
    void setSize(int w, int h) {
        width = w; height = h;
    }
    /* TODO: wright to disk. */
};

#endif //BART_RAY_TRACER_FILM_H
