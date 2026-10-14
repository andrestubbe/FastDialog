#define WIN32_LEAN_AND_MEAN
#define STRICT_TYPED_ITEMIDS
#include <windows.h>
#include <shobjidl.h>     // IFileOpenDialog, IFileSaveDialog
#include <shlwapi.h>
#include <knownfolders.h>
#include <vector>
#include <string>

#include "FastDialog.h"

// Helper: Convert jstring to std::wstring
static std::wstring JStringToWString(JNIEnv* env, jstring str) {
    if (!str) return L"";
    const jchar* chars = env->GetStringChars(str, nullptr);
    jsize len = env->GetStringLength(str);
    std::wstring result((const wchar_t*)chars, len);
    env->ReleaseStringChars(str, chars);
    return result;
}

// Helper: Convert LPCWSTR to jstring
static jstring WStringToJString(JNIEnv* env, LPCWSTR str) {
    if (!str) return nullptr;
    return env->NewString((const jchar*)str, (jsize)wcslen(str));
}

// RAII COM Initializer ensuring apartment threaded mode for Shell dialogs
struct ComScope {
    HRESULT hr;
    ComScope() {
        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    }
    ~ComScope() {
        if (SUCCEEDED(hr)) {
            CoUninitialize();
        }
    }
};

// Helper: Build COMDLG_FILTERSPEC array from JNI arrays
static void BuildFilterSpecs(JNIEnv* env, jobjectArray jDescs, jobjectArray jExts, 
                             std::vector<COMDLG_FILTERSPEC>& specs,
                             std::vector<std::wstring>& storage) {
    if (!jDescs || !jExts) return;
    jsize countDescs = env->GetArrayLength(jDescs);
    jsize countExts = env->GetArrayLength(jExts);
    jsize count = countDescs < countExts ? countDescs : countExts;

    for (jsize i = 0; i < count; i++) {
        jstring jDesc = (jstring)env->GetObjectArrayElement(jDescs, i);
        jstring jExt = (jstring)env->GetObjectArrayElement(jExts, i);

        std::wstring desc = JStringToWString(env, jDesc);
        std::wstring ext = JStringToWString(env, jExt);

        storage.push_back(desc);
        storage.push_back(ext);

        if (jDesc) env->DeleteLocalRef(jDesc);
        if (jExt) env->DeleteLocalRef(jExt);
    }

    for (size_t i = 0; i < storage.size(); i += 2) {
        COMDLG_FILTERSPEC spec;
        spec.pszName = storage[i].c_str();
        spec.pszSpec = storage[i + 1].c_str();
        specs.push_back(spec);
    }
}

// Helper: Set default folder on dialog if valid path provided
static void SetDefaultFolder(IFileDialog* pDialog, const std::wstring& folderPath) {
    if (folderPath.empty()) return;
    IShellItem* pItem = NULL;
    HRESULT hr = SHCreateItemFromParsingName(folderPath.c_str(), NULL, IID_PPV_ARGS(&pItem));
    if (SUCCEEDED(hr) && pItem) {
        pDialog->SetFolder(pItem);
        pItem->Release();
    }
}

// =========================================================================
// JNI: nativeOpenFile
// =========================================================================
JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeOpenFile
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jFolder, jobjectArray jDescs, jobjectArray jExts) {
    ComScope com;

    IFileOpenDialog* pFileOpen = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (FAILED(hr)) return nullptr;

    // Set dialog title
    std::wstring title = JStringToWString(env, jTitle);
    if (!title.empty()) {
        pFileOpen->SetTitle(title.c_str());
    }

    // Set initial directory
    std::wstring folder = JStringToWString(env, jFolder);
    SetDefaultFolder(pFileOpen, folder);

    // Apply file filters
    std::vector<COMDLG_FILTERSPEC> specs;
    std::vector<std::wstring> storage;
    BuildFilterSpecs(env, jDescs, jExts, specs, storage);
    if (!specs.empty()) {
        pFileOpen->SetFileTypes((UINT)specs.size(), specs.data());
        pFileOpen->SetFileTypeIndex(1);
    }

    // Ensure standard file open options (force filesystem, must exist)
    FILEOPENDIALOGOPTIONS opt;
    if (SUCCEEDED(pFileOpen->GetOptions(&opt))) {
        pFileOpen->SetOptions(opt | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
    }

    HWND hwndOwner = (HWND)parentHwnd;
    hr = pFileOpen->Show(hwndOwner);

    jstring result = nullptr;
    if (SUCCEEDED(hr)) {
        IShellItem* pItem = NULL;
        hr = pFileOpen->GetResult(&pItem);
        if (SUCCEEDED(hr) && pItem) {
            PWSTR pszFilePath = NULL;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            if (SUCCEEDED(hr) && pszFilePath) {
                result = WStringToJString(env, pszFilePath);
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }

    pFileOpen->Release();
    return result;
}

// =========================================================================
// JNI: nativeOpenFiles (Multiselect)
// =========================================================================
JNIEXPORT jobjectArray JNICALL Java_fastdialog_FastDialog_nativeOpenFiles
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jFolder, jobjectArray jDescs, jobjectArray jExts) {
    ComScope com;

    IFileOpenDialog* pFileOpen = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));
    if (FAILED(hr)) return nullptr;

    std::wstring title = JStringToWString(env, jTitle);
    if (!title.empty()) {
        pFileOpen->SetTitle(title.c_str());
    }

    std::wstring folder = JStringToWString(env, jFolder);
    SetDefaultFolder(pFileOpen, folder);

    std::vector<COMDLG_FILTERSPEC> specs;
    std::vector<std::wstring> storage;
    BuildFilterSpecs(env, jDescs, jExts, specs, storage);
    if (!specs.empty()) {
        pFileOpen->SetFileTypes((UINT)specs.size(), specs.data());
        pFileOpen->SetFileTypeIndex(1);
    }

    // Enable multi-select flag
    FILEOPENDIALOGOPTIONS opt;
    if (SUCCEEDED(pFileOpen->GetOptions(&opt))) {
        pFileOpen->SetOptions(opt | FOS_ALLOWMULTISELECT | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
    }

    HWND hwndOwner = (HWND)parentHwnd;
    hr = pFileOpen->Show(hwndOwner);

    jobjectArray resultArr = nullptr;
    if (SUCCEEDED(hr)) {
        IShellItemArray* pItemArray = NULL;
        hr = pFileOpen->GetResults(&pItemArray);
        if (SUCCEEDED(hr) && pItemArray) {
            DWORD count = 0;
            pItemArray->GetCount(&count);

            jclass stringClass = env->FindClass("java/lang/String");
            resultArr = env->NewObjectArray((jsize)count, stringClass, nullptr);

            for (DWORD i = 0; i < count; i++) {
                IShellItem* pItem = NULL;
                if (SUCCEEDED(pItemArray->GetItemAt(i, &pItem)) && pItem) {
                    PWSTR pszFilePath = NULL;
                    if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)) && pszFilePath) {
                        jstring jPath = WStringToJString(env, pszFilePath);
                        env->SetObjectArrayElement(resultArr, (jsize)i, jPath);
                        env->DeleteLocalRef(jPath);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pItemArray->Release();
        }
    }

    pFileOpen->Release();
    return resultArr;
}

// =========================================================================
// JNI: nativeSaveFile
// =========================================================================
JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeSaveFile
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jFolder, 
   jstring jDefName, jstring jDefExt, jobjectArray jDescs, jobjectArray jExts) {
    ComScope com;

    IFileSaveDialog* pFileSave = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileSave));
    if (FAILED(hr)) return nullptr;

    std::wstring title = JStringToWString(env, jTitle);
    if (!title.empty()) {
        pFileSave->SetTitle(title.c_str());
    }

    std::wstring folder = JStringToWString(env, jFolder);
    SetDefaultFolder(pFileSave, folder);

    std::wstring defName = JStringToWString(env, jDefName);
    if (!defName.empty()) {
        pFileSave->SetFileName(defName.c_str());
    }

    std::wstring defExt = JStringToWString(env, jDefExt);
    if (!defExt.empty()) {
        pFileSave->SetDefaultExtension(defExt.c_str());
    }

    std::vector<COMDLG_FILTERSPEC> specs;
    std::vector<std::wstring> storage;
    BuildFilterSpecs(env, jDescs, jExts, specs, storage);
    if (!specs.empty()) {
        pFileSave->SetFileTypes((UINT)specs.size(), specs.data());
        pFileSave->SetFileTypeIndex(1);
    }

    FILEOPENDIALOGOPTIONS opt;
    if (SUCCEEDED(pFileSave->GetOptions(&opt))) {
        pFileSave->SetOptions(opt | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_OVERWRITEPROMPT);
    }

    HWND hwndOwner = (HWND)parentHwnd;
    hr = pFileSave->Show(hwndOwner);

    jstring result = nullptr;
    if (SUCCEEDED(hr)) {
        IShellItem* pItem = NULL;
        hr = pFileSave->GetResult(&pItem);
        if (SUCCEEDED(hr) && pItem) {
            PWSTR pszFilePath = NULL;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            if (SUCCEEDED(hr) && pszFilePath) {
                result = WStringToJString(env, pszFilePath);
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }

    pFileSave->Release();
    return result;
}

// =========================================================================
// JNI: nativeOpenFolder
// =========================================================================
JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativeOpenFolder
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jFolder) {
    ComScope com;

    IFileOpenDialog* pFolderDialog = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFolderDialog));
    if (FAILED(hr)) return nullptr;

    std::wstring title = JStringToWString(env, jTitle);
    if (!title.empty()) {
        pFolderDialog->SetTitle(title.c_str());
    }

    std::wstring folder = JStringToWString(env, jFolder);
    SetDefaultFolder(pFolderDialog, folder);

    // Set FOS_PICKFOLDERS
    FILEOPENDIALOGOPTIONS opt;
    if (SUCCEEDED(pFolderDialog->GetOptions(&opt))) {
        pFolderDialog->SetOptions(opt | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
    }

    HWND hwndOwner = (HWND)parentHwnd;
    hr = pFolderDialog->Show(hwndOwner);

    jstring result = nullptr;
    if (SUCCEEDED(hr)) {
        IShellItem* pItem = NULL;
        hr = pFolderDialog->GetResult(&pItem);
        if (SUCCEEDED(hr) && pItem) {
            PWSTR pszFilePath = NULL;
            hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
            if (SUCCEEDED(hr) && pszFilePath) {
                result = WStringToJString(env, pszFilePath);
                CoTaskMemFree(pszFilePath);
            }
            pItem->Release();
        }
    }

    pFolderDialog->Release();
    return result;
}
