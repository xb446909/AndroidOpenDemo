#include "androidimagepicker.h"

QMutex mutex;

AndroidImagePicker::AndroidImagePicker()
{

}

AndroidImagePicker* AndroidImagePicker::pInst = NULL;

AndroidImagePicker* AndroidImagePicker::Get()
{
    if(pInst == NULL)
    {
        mutex.lock();
        pInst = new AndroidImagePicker();
        mutex.unlock();
    }
    return pInst;
}

void AndroidImagePicker::DoPicker()
{
    QAndroidJniObject ACTION_PICK = QAndroidJniObject::fromString("android.intent.action.GET_CONTENT");
    QAndroidJniObject intent("android/content/Intent");
    if (ACTION_PICK.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_PICK.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString("image/*").object<jstring>());
        QtAndroid::startActivity(intent.object<jobject>(), 101, this);
        qDebug() << "OK";
    }
    else
    {
        qDebug() << "ERROR";
    }
}

void AndroidImagePicker::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>("android/app/Activity", "RESULT_OK");
    if (receiverRequestCode == 101 && resultCode == RESULT_OK) {
        QString imagemCaminho = data.callObjectMethod("getData", "()Landroid/net/Uri;").callObjectMethod("getPath", "()Ljava/lang/String;").toString();
        emit imagePathSignal(imagemCaminho);

        qDebug() << imagemCaminho;
    }
    else
    {
        qDebug() << "Get Image Error";
    }
}
