#ifndef FASTDIALOG_H
#define FASTDIALOG_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeOpenFile
  (JNIEnv *, jclass, jlong, jstring, jstring, jobjectArray, jobjectArray);

JNIEXPORT jobjectArray JNICALL Java_fastdialog_FastDialog_nativeOpenFiles
  (JNIEnv *, jclass, jlong, jstring, jstring, jobjectArray, jobjectArray);

JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeSaveFile
  (JNIEnv *, jclass, jlong, jstring, jstring, jstring, jstring, jobjectArray, jobjectArray);

JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeOpenFolder
  (JNIEnv *, jclass, jlong, jstring, jstring);

#ifdef __cplusplus
}
#endif

#endif // FASTDIALOG_H
