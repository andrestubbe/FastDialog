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

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowAbout
  (JNIEnv *, jclass, jlong, jstring, jstring, jstring);

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeOpenWith
  (JNIEnv *, jclass, jlong, jstring);

JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativePromptCredentials
  (JNIEnv *, jclass, jlong, jstring, jstring);

JNIEXPORT jlong JNICALL Java_fastdialog_FastDialog_nativeCreateProgress
  (JNIEnv *, jclass, jlong, jstring, jstring, jint);

JNIEXPORT void JNICALL Java_fastdialog_FastDialog_nativeUpdateProgress
  (JNIEnv *, jclass, jlong, jint, jint, jstring, jstring);

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeIsProgressCancelled
  (JNIEnv *, jclass, jlong);

JNIEXPORT void JNICALL Java_fastdialog_FastDialog_nativeCloseProgress
  (JNIEnv *, jclass, jlong);

JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativeShowCustomTaskDialog
  (JNIEnv *, jclass, jlong, jstring, jstring, jstring, jstring, jstring, jobjectArray, jint, jint, jbooleanArray);

#ifdef __cplusplus
}
#endif

#endif // FASTDIALOG_H
