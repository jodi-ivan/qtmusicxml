#include "musicrenderer.h"

MusicRenderer::MusicRenderer(QObject *parent)
    : QObject{parent}
{

}

void MusicRenderer::onChange(int num, std::string variant, int verse, bool focusMode)
{
    qInfo() << "changed";
}
