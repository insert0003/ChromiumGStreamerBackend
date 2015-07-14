# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'targets': [
    {
      'target_name': 'background',
      'variables': {
        'depends': [
          '../../../../third_party/jstemplate/compiled_resources.gyp:jstemplate',
          '../../../webui/resources/js/cr.js',
          '../../../webui/resources/js/cr/event_target.js',
          '../../../webui/resources/js/cr/ui.js',
          '../../../webui/resources/js/cr/ui/array_data_model.js',
          '../../../webui/resources/js/cr/ui/dialogs.js',
          '../../../webui/resources/js/load_time_data.js',
          '../../../webui/resources/js/util.js',
          '../../file_manager/common/js/util.js',
          '../../file_manager/common/js/async_util.js',
          '../../file_manager/common/js/volume_manager_common.js',
          '../../file_manager/common/js/error_util.js',
          '../../file_manager/common/js/file_type.js',
          '../../file_manager/background/js/app_window_wrapper.js',
          '../../file_manager/background/js/background_base.js',
          '../../file_manager/background/js/volume_manager.js',
        ],
        'externs': [
          '<(EXTERNS_DIR)/chrome_send.js',
          '<(EXTERNS_DIR)/chrome_extensions.js',
          '<(EXTERNS_DIR)/file_manager_private.js',
          '../../externs/chrome_test.js',
          '../../externs/gallery_background.js',
          '../../externs/platform.js',
        ],
      },
      'includes': [
        '../../../../third_party/closure_compiler/compile_js.gypi'
      ],
    },
    {
      'target_name': 'gallery',
      'variables': {
        'depends': [
          '../../../../third_party/jstemplate/compiled_resources.gyp:jstemplate',
          '../../../webui/resources/js/util.js',
          '../../../webui/resources/js/event_tracker.js',
          '../../../webui/resources/js/load_time_data.js',
          '../../../webui/resources/js/cr.js',
          '../../../webui/resources/js/cr/ui.js',
          '../../../webui/resources/js/cr/event_target.js',
          '../../../webui/resources/js/cr/ui/touch_handler.js',
          '../../../webui/resources/js/cr/ui/array_data_model.js',
          '../../../webui/resources/js/cr/ui/dialogs.js',
          '../../../webui/resources/js/cr/ui/list_item.js',
          '../../../webui/resources/js/cr/ui/list_selection_model.js',
          '../../../webui/resources/js/cr/ui/list_single_selection_model.js',
          '../../../webui/resources/js/cr/ui/list_selection_controller.js',
          '../../../webui/resources/js/cr/ui/list.js',
          '../../../webui/resources/js/cr/ui/grid.js',
          '../../../webui/resources/js/compiled_resources.gyp:i18n_template_no_process',
          '../../file_manager/common/js/volume_manager_common.js',
          '../../file_manager/common/js/lru_cache.js',
          '../../file_manager/common/js/async_util.js',
          '../../file_manager/common/js/file_type.js',
          '../../file_manager/common/js/util.js',
          '../../file_manager/common/js/metrics_base.js',
          '../../file_manager/common/js/metrics.js',
          '../../file_manager/foreground/js/metrics_start.js',
          '../../file_manager/foreground/js/metadata/content_metadata_provider.js',
          '../../file_manager/foreground/js/metadata/exif_constants.js',
          '../../file_manager/foreground/js/metadata/external_metadata_provider.js',
          '../../file_manager/foreground/js/metadata/file_system_metadata_provider.js',
          '../../file_manager/foreground/js/metadata/metadata_cache_item.js',
          '../../file_manager/foreground/js/metadata/metadata_cache_set.js',
          '../../file_manager/foreground/js/metadata/metadata_item.js',
          '../../file_manager/foreground/js/metadata/metadata_model.js',
          '../../file_manager/foreground/js/metadata/multi_metadata_provider.js',
          '../../file_manager/foreground/js/metadata/new_metadata_provider.js',
          '../../file_manager/foreground/js/metadata/thumbnail_model.js',
          '../../file_manager/foreground/js/mouse_inactivity_watcher.js',
          '../../file_manager/foreground/js/share_client.js',
          '../../file_manager/foreground/js/thumbnail_loader.js',
          '../../file_manager/foreground/js/ui/file_manager_dialog_base.js',
          '../../file_manager/foreground/js/ui/share_dialog.js',
          '../../file_manager/foreground/js/volume_manager_wrapper.js',
          '../../file_manager/background/js/volume_manager.js',
          '../../image_loader/image_loader_client.js',
          './image_editor/commands.js',
          './image_editor/exif_encoder.js',
          './image_editor/filter.js',
          './image_editor/image_adjust.js',
          './image_editor/image_buffer.js',
          './image_editor/image_editor.js',
          './image_editor/image_encoder.js',
          './image_editor/image_transform.js',
          './image_editor/image_util.js',
          './image_editor/image_view.js',
          './image_editor/viewport.js',
          './entry_list_watcher.js',
          './error_banner.js',
          './gallery_data_model.js',
          './gallery_item.js',
          './gallery_util.js',
          './mosaic_mode.js',
          './ribbon.js',
          './slide_mode.js',
        ],
        'externs': [
          '<(EXTERNS_DIR)/chrome_send.js',
          '<(EXTERNS_DIR)/chrome_extensions.js',
          '<(EXTERNS_DIR)/file_manager_private.js',
          '<(EXTERNS_DIR)/metrics_private.js',
          '../../../../third_party/analytics/externs.js',
          '../../externs/chrome_test.js',
          '../../externs/exif_entry.js',
          '../../externs/gallery_foreground.js',
          '../../externs/paper_elements.js',
          '../../externs/platform.js',
          '../../externs/webview_tag.js',
        ],
      },
      'includes': [
        '../../../../third_party/closure_compiler/compile_js.gypi'
      ],
    }
  ],
}
