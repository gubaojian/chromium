# Copyright (c) 2010 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'webkit_support',
      'type': '<(library)',
      'dependencies': [
        '<(DEPTH)/ui/ui.gyp:ui_gfx',
        '<(DEPTH)/media/media.gyp:media',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/testing/gtest.gyp:gtest',
        'appcache',
        'blob',
        'database',
        'fileapi',
        'glue',
        'webkit_gpu',
        'webkit_support_common',
      ],
      'include_dirs': [
        '<(SHARED_INTERMEDIATE_DIR)/webkit', # for a header generated by grit
      ],
      'defines': [
        # Technically not a unit test but require functions available only to
        # unit tests.
        'UNIT_TEST'
      ],
      'sources': [
        'drt_application_mac.h',
        'drt_application_mac.mm',
        'platform_support.h',
        'platform_support_gtk.cc',
        'platform_support_mac.mm',
        'platform_support_win.cc',
        'test_webkit_client.cc',
        'test_webkit_client.h',
        'test_webplugin_page_delegate.cc',
        'test_webplugin_page_delegate.h',
        'webkit_support.cc',
        'webkit_support.h',
        'webkit_support_gfx.h',
        'webkit_support_glue.cc',
        'weburl_loader_mock.cc',
        'weburl_loader_mock.h',
        'weburl_loader_mock_factory.cc',
        'weburl_loader_mock_factory.h',
      ],
      'conditions': [
        ['OS=="mac"', {
          'copies': [{
            'destination': '<(SHARED_INTERMEDIATE_DIR)/webkit',
            'files': ['../tools/test_shell/resources/textAreaResizeCorner.png'],
          }],
        },{ # OS!="mac"
          'copies': [{
            'destination': '<(PRODUCT_DIR)/DumpRenderTree_resources',
            'files': [
              '../tools/test_shell/resources/missingImage.gif',
              '../tools/test_shell/resources/textAreaResizeCorner.png',
            ],
          }],
        }],
      ],
    },

    {
      'target_name': 'webkit_support_common',
      'type': '<(library)',
      'dependencies': [
        '<(DEPTH)/skia/skia.gyp:skia',
        'glue',
      ],
      'sources': [
        '<(DEPTH)/webkit/tools/test_shell/mac/DumpRenderTreePasteboard.h',
        '<(DEPTH)/webkit/tools/test_shell/mac/DumpRenderTreePasteboard.m',
        '<(DEPTH)/webkit/tools/test_shell/mock_webclipboard_impl.cc',
        '<(DEPTH)/webkit/tools/test_shell/mock_webclipboard_impl.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_appcache_system.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_appcache_system.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_clipboard_impl.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_file_system.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_file_system.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_file_writer.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_file_writer.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_resource_loader_bridge.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_resource_loader_bridge.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_socket_stream_bridge.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_socket_stream_bridge.h',
        '<(DEPTH)/webkit/tools/test_shell/simple_webcookiejar_impl.cc',
        '<(DEPTH)/webkit/tools/test_shell/simple_webcookiejar_impl.h',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_request_context.cc',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_request_context.h',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_webblobregistry_impl.cc',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_webblobregistry_impl.h',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_webmimeregistry_impl.cc',
        '<(DEPTH)/webkit/tools/test_shell/test_shell_webmimeregistry_impl.h',
        'simple_database_system.cc',
        'simple_database_system.h',
      ],
      'conditions': [
        ['inside_chromium_build==0', {
          'dependencies': [
            'setup_third_party.gyp:third_party_headers',
          ],
        }],
      ],
    },
  ],
}
# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
