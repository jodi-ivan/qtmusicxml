#ifndef MUSICRENDERER_H
#define MUSICRENDERER_H

#include <QObject>
#include <QDebug>
#include <QScrollArea>
#include <QSvgRenderer>
#include <QSvgWidget>

#include "libhymn_renderer.h"

class MusicRenderer : public QObject
{
    Q_OBJECT

    int hymnNum;
    std::string variant;
    int verse;
    bool focusMode;
    QSvgRenderer* svgRenderer;

public:
    explicit MusicRenderer(QObject *parent = nullptr);
    void setRenderer(QSvgRenderer* renderer);


public slots:
    void onChange(int num, std::string variant, int verse, bool focusMode);
};

#endif // MUSICRENDERER_H
