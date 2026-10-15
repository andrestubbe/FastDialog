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

JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativeShowTaskDialog
  (JNIEnv *, jclass, jlong, jstring, jstring, jstring, jint, jint);

JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativePickColor
  (JNIEnv *, jclass, jlong, jint);

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowInFolder
  (JNIEnv *, jclass, jstring);

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowProperties
  (JNIEnv *, jclass, jlong, jstring);

JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativePickFont
  (JNIEnv *, jclass, jlong, jstring, jint, jboolean, jboolean);

#ifdef __cplusplus
}
#endif

#endif // FASTDIALOG_H
