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
    QAndroidJniObject ACTION_PICK = QAndroidJniObject::fromString("android.intent.action.PICK");
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
    else
    {
        qDebug() << "Get Image Error";
    }
}
