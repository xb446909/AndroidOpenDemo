#include "androidimagepicker.h"

AndroidImagePicker::AndroidImagePicker()
{

}

AndroidImagePicker* AndroidImagePicker::pInst = NULL;

static QMutex mutex;

AndroidImagePicker* AndroidImagePicker::Get()
{
    if(pInst == NULL)
    {
        mutex.lock();
        if(pInst == NULL)
            pInst = new AndroidImagePicker();
        mutex.unlock();
    }
    return pInst;
}

void AndroidImagePicker::DoPicker(int source)
{
    QAndroidJniObject ACTION_PICK = QAndroidJniObject::fromString("android.intent.action.PICK");
    QAndroidJniObject IMAGE_CAPTURE = QAndroidJniObject::fromString("android.media.action.IMAGE_CAPTURE");
    QAndroidJniObject intent("android/content/Intent");
    if (ACTION_PICK.isValid() && intent.isValid() && IMAGE_CAPTURE.isValid())
    {
        if(source == FROM_GALLERY)
        {
            intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", ACTION_PICK.object<jstring>());
            intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;", QAndroidJniObject::fromString("image/*").object<jstring>());
            QtAndroid::startActivity(intent.object<jobject>(), 101, this);
        }
        else
        {
            QAndroidJniObject externalStorageDirectory = QAndroidJniObject::callStaticObjectMethod("android/os/Environment",
                                                                                                   "getExternalStorageDirectory",
                                                                                                   "()Ljava/io/File;");
            QAndroidJniObject tmpImage( "java/io/File",
                                        "(Ljava/io/File;Ljava/lang/String;)V",
                                        externalStorageDirectory.object<jobject>(),
                                        QAndroidJniObject::fromString("/tmpImage.jpg").object<jobject>());
            if(tmpImage.isValid())
            {
                QAndroidJniObject tmpUri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",
                                                                                     "fromFile",
                                                                                     "(Ljava/io/File;)Landroid/net/Uri;",
                                                                                     tmpImage.object<jobject>());
                QAndroidJniObject extra_output = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore", "EXTRA_OUTPUT", "Ljava/lang/String;");
                intent.callObjectMethod("putExtra",
                                        "(Ljava/lang/String;Landroid/net/Uri;)Landroid/content/Intent;",
                                        extra_output.object<jobject>(),
                                        tmpUri.object<jobject>());
                intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", IMAGE_CAPTURE.object<jstring>());
                QtAndroid::startActivity(intent.object<jobject>(), 102, this);
            }
        }
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

        QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");

        QAndroidJniObject dadosAndroid = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;");
        QAndroidJniEnvironment env;
        jobjectArray projecao = (jobjectArray)env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
        jobject projacaoDadosAndroid = env->NewStringUTF(dadosAndroid.toString().toStdString().c_str());
        env->SetObjectArrayElement(projecao, 0, projacaoDadosAndroid);
        QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
        QAndroidJniObject cursor = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), projecao, NULL, NULL, NULL);
        cursor.callMethod<jboolean>("moveToFirst", "()Z");
        jint columnIndex = cursor.callMethod<jint>("getColumnIndex", "(Ljava/lang/String;)I", dadosAndroid.object<jstring>());
        QAndroidJniObject resultado = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);
        QString imagemCaminho = resultado.toString();
        emit imagePathSignal(imagemCaminho);
    }
    else if (receiverRequestCode == 102 && resultCode == RESULT_OK)
    {
        QAndroidJniObject externalStorageDirectory = QAndroidJniObject::callStaticObjectMethod("android/os/Environment",
                                                                                               "getExternalStorageDirectory",
                                                                                               "()Ljava/io/File;");
        QAndroidJniObject dir = externalStorageDirectory.callObjectMethod("getPath", "()Ljava/lang/String;");
        QAndroidJniObject name = QAndroidJniObject::fromString(dir.toString() + "/tmpImage.jpg");
        emit imagePathSignal(name.toString());
    }
    else
    {
        qDebug() << "Get Image Error";
    }
}
