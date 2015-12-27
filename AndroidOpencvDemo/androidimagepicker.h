#ifndef ANDROIDIMAGEPICKER_H
#define ANDROIDIMAGEPICKER_H

#include <QObject>
#include <QtAndroidExtras>

class AndroidImagePicker : public QObject, public QAndroidActivityResultReceiver
{
    Q_OBJECT

private:
    static AndroidImagePicker* pInst;
public:
    AndroidImagePicker();
    static AndroidImagePicker* Get();

    void DoPicker();

    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);

    signals:
        void imagePathSignal(QString);
};

#endif // ANDROIDIMAGEPICKER_H
