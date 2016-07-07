// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/android/download/chrome_download_delegate.h"

#include <jni.h>

#include <string>
#include <type_traits>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/android/scoped_java_ref.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/android/download/android_download_manager_overwrite_infobar_delegate.h"
#include "chrome/browser/android/tab_android.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/permissions/permission_update_infobar_delegate_android.h"
#include "chrome/common/safe_browsing/file_type_policies.h"
#include "chrome/grit/chromium_strings.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/android/download_controller_android.h"
#include "jni/ChromeDownloadDelegate_jni.h"
#include "ui/base/l10n/l10n_util.h"

using content::DownloadControllerAndroid;

// Gets the download warning text for the given file name.
static ScopedJavaLocalRef<jstring> GetDownloadWarningText(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jstring>& filename) {
  return base::android::ConvertUTF8ToJavaString(
      env, l10n_util::GetStringFUTF8(
               IDS_PROMPT_DANGEROUS_DOWNLOAD,
               base::android::ConvertJavaStringToUTF16(env, filename)));
}

// Returns true if a file name is dangerous, or false otherwise.
static jboolean IsDownloadDangerous(JNIEnv* env,
                                    const JavaParamRef<jclass>& clazz,
                                    const JavaParamRef<jstring>& filename) {
  base::FilePath path(base::android::ConvertJavaStringToUTF8(env, filename));
  return safe_browsing::FileTypePolicies::GetInstance()->GetFileDangerLevel(
             path) != safe_browsing::DownloadFileType::NOT_DANGEROUS;
}

// Called when a dangerous download is validated.
static void DangerousDownloadValidated(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jobject>& tab,
    const JavaParamRef<jstring>& jdownload_guid,
    jboolean accept) {
  std::string download_guid =
      base::android::ConvertJavaStringToUTF8(env, jdownload_guid);
  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, tab);
  DownloadControllerAndroid::Get()->DangerousDownloadValidated(
      tab_android->web_contents(), download_guid, accept);
}

// static
bool ChromeDownloadDelegate::EnqueueDownloadManagerRequest(
    jobject chrome_download_delegate,
    bool overwrite,
    jobject download_info,
    const std::string& dir_full_path) {
  JNIEnv* env = base::android::AttachCurrentThread();

  ScopedJavaLocalRef<jstring> j_dir_full_path =
      base::android::ConvertUTF8ToJavaString(env, dir_full_path);

  return Java_ChromeDownloadDelegate_enqueueDownloadManagerRequestFromNative(
      env, chrome_download_delegate, overwrite, download_info,
      j_dir_full_path.obj());
}

// Called when we need to interrupt download and ask users whether to overwrite
// an existing file.
static void LaunchDownloadOverwriteInfoBar(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jobject>& delegate,
    const JavaParamRef<jobject>& tab,
    const JavaParamRef<jobject>& download_info,
    const JavaParamRef<jstring>& jfile_name,
    jlong jtotal_bytes,
    const JavaParamRef<jstring>& jmime_type,
    const JavaParamRef<jstring>& jdir_name,
    const JavaParamRef<jstring>& jdir_full_path) {
  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, tab);

  std::string file_name =
      base::android::ConvertJavaStringToUTF8(env, jfile_name);
  int64_t total_bytes = jtotal_bytes;
  std::string mime_type =
      base::android::ConvertJavaStringToUTF8(env, jmime_type);
  std::string dir_name = base::android::ConvertJavaStringToUTF8(env, jdir_name);
  std::string dir_full_path =
      base::android::ConvertJavaStringToUTF8(env, jdir_full_path);

  chrome::android::AndroidDownloadManagerOverwriteInfoBarDelegate::Create(
      InfoBarService::FromWebContents(tab_android->web_contents()), file_name,
      total_bytes, mime_type, dir_name, dir_full_path, delegate, download_info);
}

static void LaunchPermissionUpdateInfoBar(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jobject>& tab,
    const JavaParamRef<jstring>& jpermission,
    jlong callback_id) {
  TabAndroid* tab_android = TabAndroid::GetNativeTab(env, tab);

  std::string permission =
      base::android::ConvertJavaStringToUTF8(env, jpermission);

  // Convert java long long int to c++ pointer, take ownership.
  static_assert(
      std::is_same<
          DownloadControllerAndroid::AcquireFileAccessPermissionCallback,
          base::Callback<void(bool)>>::value,
      "Callback types don't match!");
  std::unique_ptr<base::Callback<void(bool)>> cb(
      reinterpret_cast<base::Callback<void(bool)>*>(callback_id));

  std::vector<std::string> permissions;
  permissions.push_back(permission);

  PermissionUpdateInfoBarDelegate::Create(
      tab_android->web_contents(),
      permissions,
      IDS_MISSING_STORAGE_PERMISSION_DOWNLOAD_EDUCATION_TEXT,
      *cb);
}

bool RegisterChromeDownloadDelegate(JNIEnv* env) {
  return RegisterNativesImpl(env);
}