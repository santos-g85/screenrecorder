#include "ScreenRecorder.h"
#include <QDebug>

ScreenRecorder::ScreenRecorder(QObject* parent) : QObject(parent), isrecording(false){
    settings.fps=30;
    settings.quality=1;
}
ScreenRecorder::~ScreenRecorder(){
    qDebug() << "window destroyed!";
    //stopRecorder();
}

void ScreenRecorder::startRecorder() {

    qDebug() << "Started screen capture using Desktop Duplication";
}

void ScreenRecorder::stopRecorder() {

    qDebug() << "Stopped screen capture";
}
