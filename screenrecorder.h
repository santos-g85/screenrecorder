#ifndef SCREENRECORDER_H
#define SCREENRECORDER_H

#include <QObject>
#include <QProcess>
#include <stdexcept>
#include <QPushButton>

struct VideoSettings {
    int fps;
    float quality;
};

class ScreenRecorder : public QObject {
    Q_OBJECT

public:
    ScreenRecorder(QObject* parent = nullptr);
    ~ScreenRecorder();

    void startRecorder();
    void stopRecorder();

// public slots:
//     void startRecording();

//     void stopRecording();

private:
    QProcess* recorderProcess;
    VideoSettings settings;
    bool isrecording;
};

#endif // SCREENRECORDER_H
