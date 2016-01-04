#ifndef ANDROIDIMAGEPICKER_H
#define ANDROIDIMAGEPICKER_H

#include <QObject>
#include <QtAndroidExtras>
#include <QMutex>

#define FROM_GALLERY    0
#define FROM_CAMERA     1

class AndroidImagePicker : public QObject, public QAndroidActivityResultReceiver
{
    Q_OBJECT

private:
    static AndroidImagePicker* pInst;
public:
    AndroidImagePicker();
    static AndroidImagePicker* Get();

    void DoPicker(int source);

    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);

    signals:
        void imagePathSignal(QString);
};

#endif // ANDROIDIMAGEPICKER_H
