// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/native_theme/native_theme_android.h"

#include "base/basictypes.h"
#include "base/logging.h"
#include "ui/gfx/geometry/size.h"

namespace ui {

namespace {
// These are the default dimensions of radio buttons and checkboxes on Android.
const int kCheckboxAndRadioWidth = 16;
const int kCheckboxAndRadioHeight = 16;
}

#if !defined(USE_AURA)
// static
NativeTheme* NativeTheme::instance() {
  return NativeThemeAndroid::instance();
}
#endif

// static
NativeThemeAndroid* NativeThemeAndroid::instance() {
  CR_DEFINE_STATIC_LOCAL(NativeThemeAndroid, s_native_theme, ());
  return &s_native_theme;
}

gfx::Size NativeThemeAndroid::GetPartSize(Part part,
                                          State state,
                                          const ExtraParams& extra) const {
  if (part == kCheckbox || part == kRadio)
    return gfx::Size(kCheckboxAndRadioWidth, kCheckboxAndRadioHeight);
  return NativeThemeBase::GetPartSize(part, state, extra);
}

SkColor NativeThemeAndroid::GetSystemColor(ColorId color_id) const {
  NOTIMPLEMENTED();
  return SK_ColorBLACK;
}

NativeThemeAndroid::NativeThemeAndroid() {
}

NativeThemeAndroid::~NativeThemeAndroid() {
}

}  // namespace ui
