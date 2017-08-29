// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Chavezcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Chavezcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Chavezcoin.  If not, see <http://www.gnu.org/licenses/>.

#import <Foundation/NSObject.h>
#import <Foundation/NSNotification.h>

namespace WalletGui {
  class INotificationMonitorObserver;
}

@interface NotificationMonitor: NSObject {
  WalletGui::INotificationMonitorObserver* m_observer;
}
-(id) init: (WalletGui::INotificationMonitorObserver*) observer;
-(void) receive: (NSNotification*) notification;
@end
