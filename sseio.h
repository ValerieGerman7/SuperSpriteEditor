#ifndef SSEIO_H
#define SSEIO_H

#include "animation.h"

class SSEIO {

public:

    SSEIO();
    void save(Animation& sprite, std::string path);
    Animation& load(std::string path);
    void exportGif();

private:

};

#endif // SSEIO_H
