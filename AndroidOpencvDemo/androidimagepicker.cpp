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

        QAndroidJniObject uri = data.callObjectMethod("getData", "()Landroid/net/Uri;");
        QAndroidJniObject jnipath = uri.callObjectMethod("getPath", "()Ljava/lang/String;");
        qDebug() << "The path is: " << jnipath.toString();

        /*QAndroidJniObject dadosAndroid = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$ImageColumns", "DATA", "Ljava/lang/String;");
        QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
        QAndroidJniObject cursor = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), NULL, NULL, NULL, NULL);
        cursor.callMethod<jboolean>("moveToFirst", "()Z");
        jint columnIndex = cursor.callMethod<jint>("getColumnIndex", "(Ljava/lang/String;)I", dadosAndroid.object<jstring>());
        qDebug() << "columIndex = " << columnIndex;

        QAndroidJniObject resultado = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);
        QString imagemCaminho = "file://" + resultado.toString();*/

        QAndroidJniObject dadosAndroid = QAndroidJniObject::getStaticObjectField("android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;");

        qDebug() << dadosAndroid.toString();

        QAndroidJniEnvironment env;
        jobjectArray projecao = (jobjectArray)env->NewObjectArray(1, env->FindClass("java/lang/String"), NULL);
        jobject projacaoDadosAndroid = env->NewStringUTF(dadosAndroid.toString().toStdString().c_str());
        env->SetObjectArrayElement(projecao, 0, projacaoDadosAndroid);
        QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
        QAndroidJniObject cursor = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), projecao, NULL, NULL, NULL);
        jint columnIndex = cursor.callMethod<jint>("getColumnIndex", "(Ljava/lang/String;)I", dadosAndroid.object<jstring>());
        qDebug() << "columIndex = " << columnIndex;
        cursor.callMethod<jboolean>("moveToFirst", "()Z");
        QAndroidJniObject resultado = cursor.callObjectMethod("getString", "(I)Ljava/lang/String;", columnIndex);
        QString imagemCaminho = "file://" + resultado.toString();
        emit imagePathSignal(imagemCaminho);

        qDebug() << imagemCaminho;
    }
    else
    {
        qDebug() << "Get Image Error";
    }
}
