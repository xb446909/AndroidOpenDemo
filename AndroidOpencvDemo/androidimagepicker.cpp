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
            QAndroidJniObject IMAGE_CAPTURE = QAndroidJniObject::fromString("android.media.action.IMAGE_CAPTURE");
            QAndroidJniObject name = QAndroidJniObject::fromString("/sdcard/androidOpencv");
            QAndroidJniObject picture_dir( "java/io/File",
                                           "(Ljava/lang/String;)V",
                                           name.object<jobject>());
            picture_dir.callMethod<jboolean>("mkdirs", "()Z");
            QAndroidJniObject tmpImage( "java/io/File",
                                        "(Ljava/io/File;Ljava/lang/String;)V",
                                        picture_dir.object<jobject>(),
                                        QAndroidJniObject::fromString("/img_android_opencv.jpg").object<jobject>());
            if(tmpImage.isValid())
            {
                QAndroidJniObject tmpUri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",
                                                                                     "fromFile",
                                                                                     "(Ljava/io/File;)Landroid/net/Uri;",
                                                                                     tmpImage.object<jobject>());
                QAndroidJniObject extra_output = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore", "EXTRA_OUTPUT", "Ljava/lang/String;");
                intent.callObjectMethod("putExtra",
                                        "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;",
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
        QAndroidJniObject name = QAndroidJniObject::fromString("/sdcard/androidOpencv/img_android_opencv.jpg");
        QAndroidJniObject tmpImage( "java/io/File",
                                    "(Ljava/lang/String;)V",
                                    name.object<jobject>());
        jboolean isexist = tmpImage.callMethod<jboolean>("exists", "()Z");
        if(isexist == true)
        {
            emit imagePathSignal(name.toString());
        }
        else
        {
            qDebug() << "File not exist!";
        }
    }
    else
    {
        qDebug() << "Get Image Error";
    }
}
