#ifndef MUSICRENDERER_H
#define MUSICRENDERER_H

#include <QObject>
#include <QDebug>


class MusicRenderer : public QObject
{
    Q_OBJECT

    int hymnNum;
    std::string variant;
    int verse;
    bool focusMode;

public:
    explicit MusicRenderer(QObject *parent = nullptr);


public slots:
    void onChange(int num, std::string variant, int verse, bool focusMode);
};

#endif // MUSICRENDERER_H
