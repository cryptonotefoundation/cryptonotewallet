// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#import <objc/runtime.h>
#import <Cocoa/Cocoa.h>

#include "mainwindow.h"

namespace WalletGui {

namespace {

void dockClickHandler(id self, SEL _cmd) {
  Q_UNUSED(self)
  Q_UNUSED(_cmd)
  MainWindow::instance().restoreFromDock();
}

}

void MainWindow::installDockHandler() {
    Class cls = [[[NSApplication sharedApplication] delegate] class];
    if (!class_replaceMethod(cls, @selector(applicationShouldHandleReopen:hasVisibleWindows:), (IMP) dockClickHandler, "v@:")) {
      NSLog(@"MainWindow::installDockHandler() : class_addMethod failed!");
    }
}

}
