// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/android/infobars/confirm_infobar.h"

#include <utility>
#include <vector>

#include "base/android/jni_android.h"
#include "base/android/jni_array.h"
#include "base/android/jni_string.h"
#include "base/logging.h"
#include "base/memory/ptr_util.h"
#include "build/build_config.h"
#include "chrome/browser/android/resource_mapper.h"
#include "chrome/browser/infobars/infobar_service.h"
#include "chrome/browser/media/media_stream_infobar_delegate_android.h"
#include "chrome/browser/permissions/permission_infobar_delegate.h"
#include "content/public/browser/browser_context.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/infobars/core/confirm_infobar_delegate.h"
#include "content/public/browser/android/content_view_core.h"
#include "content/public/browser/web_contents.h"
#include "jni/ConfirmInfoBar_jni.h"
#include "ui/android/window_android.h"
#include "ui/gfx/android/java_bitmap.h"
#include "ui/gfx/image/image.h"

// InfoBarService -------------------------------------------------------------

std::unique_ptr<infobars::InfoBar> InfoBarService::CreateConfirmInfoBar(
    std::unique_ptr<ConfirmInfoBarDelegate> delegate) {
  return base::WrapUnique(new ConfirmInfoBar(std::move(delegate)));
}


// ConfirmInfoBar -------------------------------------------------------------

ConfirmInfoBar::ConfirmInfoBar(std::unique_ptr<ConfirmInfoBarDelegate> delegate)
    : InfoBarAndroid(std::move(delegate)) {}

ConfirmInfoBar::~ConfirmInfoBar() {
}

base::android::ScopedJavaLocalRef<jobject> ConfirmInfoBar::CreateRenderInfoBar(
    JNIEnv* env) {
  base::android::ScopedJavaLocalRef<jstring> ok_button_text =
      base::android::ConvertUTF16ToJavaString(
          env, GetTextFor(ConfirmInfoBarDelegate::BUTTON_OK));
  base::android::ScopedJavaLocalRef<jstring> cancel_button_text =
      base::android::ConvertUTF16ToJavaString(
          env, GetTextFor(ConfirmInfoBarDelegate::BUTTON_CANCEL));
  ConfirmInfoBarDelegate* delegate = GetDelegate();
  base::android::ScopedJavaLocalRef<jstring> message_text =
      base::android::ConvertUTF16ToJavaString(
          env, delegate->GetMessageText());
  base::android::ScopedJavaLocalRef<jstring> link_text =
      base::android::ConvertUTF16ToJavaString(
          env, delegate->GetLinkText());

  ScopedJavaLocalRef<jobject> java_bitmap;
  if (delegate->GetIconId() == infobars::InfoBarDelegate::kNoIconID &&
      !delegate->GetIcon().IsEmpty()) {
    java_bitmap = gfx::ConvertToJavaBitmap(delegate->GetIcon().ToSkBitmap());
  }

  std::vector<int> content_settings;
  if (delegate->AsPermissionInfobarDelegate()) {
    content_settings.push_back(
        delegate->AsPermissionInfobarDelegate()->content_setting());
  } else if (delegate->AsMediaStreamInfoBarDelegateAndroid()) {
    MediaStreamInfoBarDelegateAndroid* media_delegate =
        delegate->AsMediaStreamInfoBarDelegateAndroid();
    if (media_delegate->IsRequestingVideoAccess()) {
      content_settings.push_back(
          ContentSettingsType::CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA);
    }
    if (media_delegate->IsRequestingMicrophoneAccess()) {
      content_settings.push_back(
          ContentSettingsType::CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC);
    }
  }

  content::WebContents* web_contents =
      InfoBarService::WebContentsFromInfoBar(this);
  DCHECK(web_contents);
  bool is_incognito = web_contents->GetBrowserContext()->IsOffTheRecord();
  content::ContentViewCore* cvc =
      content::ContentViewCore::FromWebContents(web_contents);
  DCHECK(cvc);
  base::android::ScopedJavaLocalRef<jobject> jwindow_android =
      cvc->GetWindowAndroid()->GetJavaObject();

  return Java_ConfirmInfoBar_create(
      env, jwindow_android.obj(), GetEnumeratedIconId(), java_bitmap.obj(),
      message_text.obj(), link_text.obj(), ok_button_text.obj(),
      cancel_button_text.obj(),
      base::android::ToJavaIntArray(env, content_settings).obj(),
      is_incognito);
}

void ConfirmInfoBar::OnLinkClicked(JNIEnv* env,
                                   const JavaParamRef<jobject>& obj) {
  if (!owner())
      return; // We're closing; don't call anything, it might access the owner.

  if (GetDelegate()->LinkClicked(NEW_FOREGROUND_TAB))
    RemoveSelf();
}

void ConfirmInfoBar::ProcessButton(int action) {
  if (!owner())
    return; // We're closing; don't call anything, it might access the owner.

  DCHECK((action == CONTENT_SETTING_ALLOW)
        || (action == CONTENT_SETTING_BLOCK)
        || (action == CONTENT_SETTING_ALLOW_24H)
        || (action == CONTENT_SETTING_SESSION_ONLY));
  ConfirmInfoBarDelegate* delegate = GetDelegate();
  bool remove_self = false;
  switch(action) {
    case CONTENT_SETTING_ALLOW:
      remove_self = delegate->Accept();
      break;
    case CONTENT_SETTING_BLOCK:
      remove_self = delegate->Cancel();
      break;
    case CONTENT_SETTING_ALLOW_24H:
      remove_self = delegate->Accept(CONTENT_SETTING_ALLOW_24H);
      break;
    case CONTENT_SETTING_SESSION_ONLY:
      remove_self = delegate->Accept(CONTENT_SETTING_SESSION_ONLY);
      break;
    default:
      NOTREACHED();
      break;
  }
  if(remove_self) {
     RemoveSelf();
  }

}

ConfirmInfoBarDelegate* ConfirmInfoBar::GetDelegate() {
  return delegate()->AsConfirmInfoBarDelegate();
}

base::string16 ConfirmInfoBar::GetTextFor(
    ConfirmInfoBarDelegate::InfoBarButton button) {
  ConfirmInfoBarDelegate* delegate = GetDelegate();
  return (delegate->GetButtons() & button) ?
      delegate->GetButtonLabel(button) : base::string16();
}


// Native JNI methods ---------------------------------------------------------

bool RegisterConfirmInfoBar(JNIEnv* env) {
  return RegisterNativesImpl(env);
}