#define WIN32_LEAN_AND_MEAN
#define STRICT_TYPED_ITEMIDS
#include <windows.h>
#include <commdlg.h>      // ChooseColorW, ChooseFontW
#include <shellapi.h>     // ShellExecuteExW
#include <shlobj.h>       // SHParseDisplayName, SHOpenFolderAndSelectItems, IProgressDialog
#include <shobjidl.h>     // IFileOpenDialog, IFileSaveDialog
#include <shlwapi.h>
#include <wincred.h>      // CredUIPromptForWindowsCredentialsW
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
        pDialog->SetFolder(pItem);        // Forces the current folder to open directly here
        pDialog->SetDefaultFolder(pItem); // Fallback default folder
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

// =========================================================================
// JNI: nativeShowTaskDialog
// =========================================================================
JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativeShowTaskDialog
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jInstruction, jstring jContent, jint buttons, jint iconId) {
    ComScope com;

    std::wstring title = JStringToWString(env, jTitle);
    std::wstring instruction = JStringToWString(env, jInstruction);
    std::wstring content = JStringToWString(env, jContent);

    TASKDIALOGCONFIG config = { sizeof(config) };
    config.hwndParent = (HWND)parentHwnd;
    config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_POSITION_RELATIVE_TO_WINDOW;
    config.pszWindowTitle = title.empty() ? L"Notice" : title.c_str();
    config.pszMainInstruction = instruction.empty() ? NULL : instruction.c_str();
    config.pszContent = content.empty() ? NULL : content.c_str();
    config.dwCommonButtons = (TASKDIALOG_COMMON_BUTTON_FLAGS)(buttons != 0 ? buttons : TDCBF_OK_BUTTON);

    // Map iconId to TaskDialog main icon
    if (iconId == 1) config.pszMainIcon = TD_INFORMATION_ICON;
    else if (iconId == 2) config.pszMainIcon = TD_WARNING_ICON;
    else if (iconId == 3) config.pszMainIcon = TD_ERROR_ICON;
    else if (iconId == 4) config.pszMainIcon = TD_SHIELD_ICON;

    int nButtonPressed = 0;
    HRESULT hr = TaskDialogIndirect(&config, &nButtonPressed, NULL, NULL);
    if (FAILED(hr)) {
        return 0; // Cancel / Error
    }
    return (jint)nButtonPressed;
}

// =========================================================================
// JNI: nativePickColor
// =========================================================================
static COLORREF g_customColors[16] = { 0 };

JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativePickColor
  (JNIEnv *, jclass, jlong parentHwnd, jint initialRgb) {
    CHOOSECOLORW cc = { sizeof(cc) };
    cc.hwndOwner = (HWND)parentHwnd;

    // Convert initial packed ARGB/RGB to Win32 COLORREF (0x00bbggrr)
    int r = (initialRgb >> 16) & 0xFF;
    int g = (initialRgb >> 8) & 0xFF;
    int b = initialRgb & 0xFF;
    cc.rgbResult = RGB(r, g, b);
    cc.lpCustColors = g_customColors;
    cc.Flags = CC_RGBINIT | CC_FULLOPEN;

    if (ChooseColorW(&cc)) {
        int resR = GetRValue(cc.rgbResult);
        int resG = GetGValue(cc.rgbResult);
        int resB = GetBValue(cc.rgbResult);
        // Pack into 32-bit ARGB (alpha 255)
        return (jint)(0xFF000000 | (resR << 16) | (resG << 8) | resB);
    }

    return -1; // Cancelled
}

// =========================================================================
// JNI: nativeShowInFolder (Reveal in Windows Explorer)
// =========================================================================
JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowInFolder
  (JNIEnv *env, jclass, jstring jFilePath) {
    if (!jFilePath) return JNI_FALSE;
    ComScope com;

    std::wstring path = JStringToWString(env, jFilePath);
    if (path.empty()) return JNI_FALSE;

    PIDLIST_ABSOLUTE pidl = NULL;
    SFGAOF sfgao;
    HRESULT hr = SHParseDisplayName(path.c_str(), NULL, &pidl, 0, &sfgao);
    if (SUCCEEDED(hr) && pidl) {
        hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
        CoTaskMemFree(pidl);
        return SUCCEEDED(hr) ? JNI_TRUE : JNI_FALSE;
    }

    return JNI_FALSE;
}

// =========================================================================
// JNI: nativeShowProperties (Windows Properties Sheet)
// =========================================================================
JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowProperties
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jFilePath) {
    if (!jFilePath) return JNI_FALSE;
    std::wstring path = JStringToWString(env, jFilePath);
    if (path.empty()) return JNI_FALSE;

    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.fMask = SEE_MASK_INVOKEIDLIST;
    sei.hwnd = (HWND)parentHwnd;
    sei.lpVerb = L"properties";
    sei.lpFile = path.c_str();
    sei.nShow = SW_SHOWNORMAL;

    return ShellExecuteExW(&sei) ? JNI_TRUE : JNI_FALSE;
}

// =========================================================================
// JNI: nativePickFont
// =========================================================================
JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativePickFont
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jName, jint sizePt, jboolean bold, jboolean italic) {
    CHOOSEFONTW cf = { sizeof(cf) };
    LOGFONTW lf = { 0 };

    std::wstring name = JStringToWString(env, jName);
    if (!name.empty()) {
        wcsncpy_s(lf.lfFaceName, name.c_str(), LF_FACESIZE - 1);
    } else {
        wcsncpy_s(lf.lfFaceName, L"Segoe UI", LF_FACESIZE - 1);
    }

    HDC hdc = GetDC(NULL);
    int logPixelsY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(NULL, hdc);

    lf.lfHeight = -MulDiv(sizePt > 0 ? sizePt : 10, logPixelsY, 72);
    lf.lfWeight = bold ? FW_BOLD : FW_NORMAL;
    lf.lfItalic = italic ? TRUE : FALSE;
    lf.lfCharSet = DEFAULT_CHARSET;

    cf.hwndOwner = (HWND)parentHwnd;
    cf.lpLogFont = &lf;
    cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;

    if (ChooseFontW(&cf)) {
        int pointSize = cf.iPointSize / 10;
        bool resBold = (lf.lfWeight >= FW_BOLD);
        bool resItalic = (lf.lfItalic != 0);

        // Format: "Family;PointSize;Bold;Italic"
        std::wstring result = lf.lfFaceName;
        result += L";";
        result += std::to_wstring(pointSize);
        result += L";";
        result += (resBold ? L"true" : L"false");
        result += L";";
        result += (resItalic ? L"true" : L"false");

        return WStringToJString(env, result.c_str());
    }

    return nullptr; // Cancelled
}

// =========================================================================
// JNI: nativeShowAbout (ShellAboutW)
// =========================================================================
JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeShowAbout
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jAppTitle, jstring jOtherStuff, jstring jIconPath) {
    std::wstring appTitle = JStringToWString(env, jAppTitle);
    std::wstring otherStuff = JStringToWString(env, jOtherStuff);

    HICON hIcon = NULL;
    if (jIconPath) {
        std::wstring iconPath = JStringToWString(env, jIconPath);
        if (!iconPath.empty()) {
            hIcon = (HICON)LoadImageW(NULL, iconPath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
        }
    }

    int ret = ShellAboutW((HWND)parentHwnd, 
                          appTitle.empty() ? L"Application" : appTitle.c_str(), 
                          otherStuff.empty() ? NULL : otherStuff.c_str(), 
                          hIcon);

    if (hIcon) {
        DestroyIcon(hIcon);
    }

    return ret != 0 ? JNI_TRUE : JNI_FALSE;
}

// =========================================================================
// JNI: nativeOpenWith (SHOpenWithDialog)
// =========================================================================
JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeOpenWith
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jFilePath) {
    if (!jFilePath) return JNI_FALSE;
    std::wstring path = JStringToWString(env, jFilePath);
    if (path.empty()) return JNI_FALSE;

    OPENASINFO oai = { 0 };
    oai.pcszFile = path.c_str();
    oai.pcszClass = NULL;
    oai.oaifInFlags = OAIF_ALLOW_REGISTRATION | OAIF_EXEC;

    HRESULT hr = SHOpenWithDialog((HWND)parentHwnd, &oai);
    return SUCCEEDED(hr) ? JNI_TRUE : JNI_FALSE;
}

// =========================================================================
// JNI: nativePromptCredentials (CredUIPromptForWindowsCredentialsW)
// =========================================================================
JNIEXPORT jstring JNICALL Java_fastdialog_FastDialog_nativePromptCredentials
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jMessage) {
    std::wstring title = JStringToWString(env, jTitle);
    std::wstring message = JStringToWString(env, jMessage);

    CREDUI_INFOW cui = { sizeof(cui) };
    cui.hwndParent = (HWND)parentHwnd;
    cui.pszCaptionText = title.empty() ? L"Windows Security" : title.c_str();
    cui.pszMessageText = message.empty() ? L"Please enter your credentials to continue." : message.c_str();

    ULONG authPackage = 0;
    LPVOID authBuffer = NULL;
    ULONG authBufferSize = 0;
    BOOL save = FALSE;

    DWORD status = CredUIPromptForWindowsCredentialsW(
        &cui,
        0,
        &authPackage,
        NULL,
        0,
        &authBuffer,
        &authBufferSize,
        &save,
        CREDUIWIN_GENERIC
    );

    if (status == ERROR_SUCCESS && authBuffer) {
        WCHAR user[256] = { 0 };
        DWORD userLen = 256;
        WCHAR domain[256] = { 0 };
        DWORD domainLen = 256;
        WCHAR pass[256] = { 0 };
        DWORD passLen = 256;

        BOOL unpacked = CredUnPackAuthenticationBufferW(
            0,
            authBuffer,
            authBufferSize,
            user,
            &userLen,
            domain,
            &domainLen,
            pass,
            &passLen
        );

        // Securely wipe raw auth buffer
        SecureZeroMemory(authBuffer, authBufferSize);
        CoTaskMemFree(authBuffer);

        if (unpacked) {
            std::wstring fullUser = user;
            if (domainLen > 0 && wcslen(domain) > 0) {
                fullUser = std::wstring(domain) + L"\\" + fullUser;
            }
            std::wstring result = fullUser + L"\n" + pass;
            SecureZeroMemory(pass, sizeof(pass));
            return WStringToJString(env, result.c_str());
        }
    }

    return nullptr; // Cancelled or failed
}

// =========================================================================
// JNI: IProgressDialog management
// =========================================================================
JNIEXPORT jlong JNICALL Java_fastdialog_FastDialog_nativeCreateProgress
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jLine1, jint flags) {
    ComScope com;

    IProgressDialog* pProgress = NULL;
    HRESULT hr = CoCreateInstance(CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, IID_IProgressDialog, (void**)&pProgress);
    if (FAILED(hr) || !pProgress) {
        return 0;
    }

    std::wstring title = JStringToWString(env, jTitle);
    std::wstring line1 = JStringToWString(env, jLine1);

    if (!title.empty()) {
        pProgress->SetTitle(title.c_str());
    }
    if (!line1.empty()) {
        pProgress->SetLine(1, line1.c_str(), FALSE, NULL);
    }

    DWORD dwFlags = (flags != 0) ? (DWORD)flags : (PROGDLG_NORMAL | PROGDLG_AUTOTIME | PROGDLG_NOMINIMIZE);
    pProgress->StartProgressDialog((HWND)parentHwnd, NULL, dwFlags, NULL);

    return (jlong)(intptr_t)pProgress;
}

JNIEXPORT void JNICALL Java_fastdialog_FastDialog_nativeUpdateProgress
  (JNIEnv *env, jclass, jlong handle, jint current, jint total, jstring jLine2, jstring jLine3) {
    if (!handle) return;
    IProgressDialog* pProgress = (IProgressDialog*)(intptr_t)handle;

    pProgress->SetProgress((DWORD)current, (DWORD)total);

    if (jLine2) {
        std::wstring line2 = JStringToWString(env, jLine2);
        pProgress->SetLine(2, line2.c_str(), FALSE, NULL);
    }
    if (jLine3) {
        std::wstring line3 = JStringToWString(env, jLine3);
        pProgress->SetLine(3, line3.c_str(), FALSE, NULL);
    }
}

JNIEXPORT jboolean JNICALL Java_fastdialog_FastDialog_nativeIsProgressCancelled
  (JNIEnv *, jclass, jlong handle) {
    if (!handle) return JNI_TRUE;
    IProgressDialog* pProgress = (IProgressDialog*)(intptr_t)handle;
    return pProgress->HasUserCancelled() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL Java_fastdialog_FastDialog_nativeCloseProgress
  (JNIEnv *, jclass, jlong handle) {
    if (!handle) return;
    IProgressDialog* pProgress = (IProgressDialog*)(intptr_t)handle;
    pProgress->StopProgressDialog();
    pProgress->Release();
}

// =========================================================================
// JNI: nativeShowCustomTaskDialog (TaskDialog with Command Links, Details, Checkbox)
// =========================================================================
JNIEXPORT jint JNICALL Java_fastdialog_FastDialog_nativeShowCustomTaskDialog
  (JNIEnv *env, jclass, jlong parentHwnd, jstring jTitle, jstring jInstruction, jstring jContent, 
   jstring jDetails, jstring jCheckboxText, jobjectArray jButtons, jint commonButtons, jint iconId, jbooleanArray jCheckboxResult) {
    ComScope com;

    std::wstring title = JStringToWString(env, jTitle);
    std::wstring instruction = JStringToWString(env, jInstruction);
    std::wstring content = JStringToWString(env, jContent);
    std::wstring details = JStringToWString(env, jDetails);
    std::wstring checkText = JStringToWString(env, jCheckboxText);

    TASKDIALOGCONFIG config = { sizeof(config) };
    config.hwndParent = (HWND)parentHwnd;
    config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_POSITION_RELATIVE_TO_WINDOW | TDF_SIZE_TO_CONTENT;

    config.pszWindowTitle = title.empty() ? L"Notice" : title.c_str();
    config.pszMainInstruction = instruction.empty() ? NULL : instruction.c_str();
    config.pszContent = content.empty() ? NULL : content.c_str();

    if (!details.empty()) {
        config.pszExpandedInformation = details.c_str();
        config.pszCollapsedControlText = L"Show Details";
        config.pszExpandedControlText = L"Hide Details";
    }

    if (!checkText.empty()) {
        config.pszVerificationText = checkText.c_str();
    }

    // Custom Buttons / Command Links
    std::vector<TASKDIALOG_BUTTON> customButtons;
    std::vector<std::wstring> buttonStrings;

    if (jButtons) {
        jsize count = env->GetArrayLength(jButtons);
        config.dwFlags |= TDF_USE_COMMAND_LINKS;
        for (jsize i = 0; i < count; i++) {
            jstring btnStr = (jstring)env->GetObjectArrayElement(jButtons, i);
            buttonStrings.push_back(JStringToWString(env, btnStr));
            TASKDIALOG_BUTTON tb = { 0 };
            tb.nButtonID = 100 + (int)i;
            tb.pszButtonText = buttonStrings.back().c_str();
            customButtons.push_back(tb);
        }
        config.pButtons = customButtons.data();
        config.cButtons = (UINT)customButtons.size();
    }

    config.dwCommonButtons = (TASKDIALOG_COMMON_BUTTON_FLAGS)commonButtons;

    if (iconId == 1) config.pszMainIcon = TD_INFORMATION_ICON;
    else if (iconId == 2) config.pszMainIcon = TD_WARNING_ICON;
    else if (iconId == 3) config.pszMainIcon = TD_ERROR_ICON;
    else if (iconId == 4) config.pszMainIcon = TD_SHIELD_ICON;

    int nButtonPressed = 0;
    BOOL bVerificationChecked = FALSE;
    HRESULT hr = TaskDialogIndirect(&config, &nButtonPressed, NULL, &bVerificationChecked);
    if (FAILED(hr)) {
        return 0;
    }

    if (jCheckboxResult && env->GetArrayLength(jCheckboxResult) > 0) {
        jboolean val = bVerificationChecked ? JNI_TRUE : JNI_FALSE;
        env->SetBooleanArrayRegion(jCheckboxResult, 0, 1, &val);
    }

    return (jint)nButtonPressed;
}

