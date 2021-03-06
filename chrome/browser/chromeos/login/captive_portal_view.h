// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_CAPTIVE_PORTAL_VIEW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_CAPTIVE_PORTAL_VIEW_H_
#pragma once

#include <string>
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/command_updater.h"
#include "chrome/browser/ui/toolbar/toolbar_model_delegate.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/gfx/size.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/widget/widget_delegate.h"

class Profile;
class ReloadButton;
class ToolbarModel;

namespace views {
class WebView;
class Widget;
}

namespace chromeos {

class CaptivePortalWindowProxy;
class StubBubbleModelDelegate;

// View class which shows the light version of the toolbar and the tab contents.
// Light version of the toolbar includes back, forward buttons and location
// bar. Location bar is shown in read only mode, because this view is designed
// to be used for sign in to captive portal on login screen (when Browser
// isn't running).
class CaptivePortalView : public views::ButtonListener,
                          public views::WidgetDelegateView,
                          public LocationBarView::Delegate,
                          public ToolbarModelDelegate,
                          public CommandUpdater::CommandUpdaterDelegate,
                          public content::WebContentsDelegate {
 public:
  CaptivePortalView(Profile* profile, CaptivePortalWindowProxy* proxy);
  virtual ~CaptivePortalView();

  // Starts loading.
  void StartLoad();

  // Inits view. Should be attached to a Widget before call.
  void Init();

  // Overridden from views::WidgetDelegate:
  virtual bool CanResize() const OVERRIDE;
  virtual ui::ModalType GetModalType() const OVERRIDE;
  virtual string16 GetWindowTitle() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool ShouldShowWindowTitle() const OVERRIDE;

  // Implements views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const views::Event& event) OVERRIDE;

  // Implements WebContentsDelegate:
  virtual void NavigationStateChanged(const content::WebContents* source,
                                      unsigned changed_flags) OVERRIDE;
  virtual void LoadingStateChanged(content::WebContents* source) OVERRIDE;

  // Implements LocationBarView::Delegate:
  virtual TabContentsWrapper* GetTabContentsWrapper() const OVERRIDE;
  virtual InstantController* GetInstant() OVERRIDE;
  virtual views::Widget* CreateViewsBubble(
      views::BubbleDelegateView* bubble_delegate) OVERRIDE;
  virtual PageActionImageView* CreatePageActionImageView(
      LocationBarView* owner,
      ExtensionAction* action) OVERRIDE;
  virtual ContentSettingBubbleModelDelegate*
      GetContentSettingBubbleModelDelegate() OVERRIDE;
  virtual void ShowPageInfo(content::WebContents* web_contents,
                            const GURL& url,
                            const content::SSLStatus& ssl,
                            bool show_history) OVERRIDE;
  virtual void OnInputInProgress(bool in_progress) OVERRIDE;

  // Implements ToolbarModelDelegate:
  virtual content::WebContents* GetActiveWebContents() const OVERRIDE;

  // Implements CommandUpdaterDelegate:
  virtual void ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition) OVERRIDE;

 private:
  void LoadImages();
  void UpdateButtons();
  void UpdateReload(bool is_loading, bool force);

  Profile* profile_;
  scoped_ptr<ToolbarModel> toolbar_model_;
  scoped_ptr<CommandUpdater> command_updater_;
  bool redirected_;

  // Contains CaptivePortalWindowProxy to be notified when redirection state is
  // resolved.
  CaptivePortalWindowProxy* proxy_;

  // Controls
  views::ImageButton* back_;
  views::ImageButton* forward_;
  ReloadButton* reload_;
  LocationBarView* location_bar_;
  views::WebView* web_view_;

  // Contains |web_view_| while it isn't owned by the view.
  scoped_ptr<views::WebView> web_view_container_;

  scoped_ptr<StubBubbleModelDelegate> bubble_model_delegate_;

  DISALLOW_COPY_AND_ASSIGN(CaptivePortalView);
};

}  // chromeos

#endif  // CHROME_BROWSER_CHROMEOS_LOGIN_CAPTIVE_PORTAL_VIEW_H_
