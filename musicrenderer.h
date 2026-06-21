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

signals:
    void musicRendered(int numbered, std::string variant , int totalVariant, int totalVerse);


public slots:
    void onChange(int num, std::string variant, int verse, bool focusMode);
    void onVariantChange(std::string selected);
};

#endif // MUSICRENDERER_H
