#ifndef MUSICRENDERER_H
#define MUSICRENDERER_H

#include <QDebug>
#include <QObject>
#include <QScrollArea>
#include <QSvgRenderer>
#include <QSvgWidget>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>

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
    void onVerseChange(int selected, bool focusMode);
};

#endif // MUSICRENDERER_H
