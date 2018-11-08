#ifndef SSEIO_H
#define SSEIO_H

#include "animation.h"

class SSEIO {

public:

    SSEIO();

    void save(Animation& sprite, QString path);
    Animation& load(QString path);
    void exportGif();

private:

};

#endif // SSEIO_H
