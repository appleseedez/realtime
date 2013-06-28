//
//  FESettingViewController.h
//  avDemo
//
//  Created by Pharaoh on 13-6-28.
//  Copyright (c) 2013年 free. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface FESettingViewController : UITableViewController<UITextFieldDelegate>
@property (retain, nonatomic) IBOutlet UILabel *currentIP;
@property (retain, nonatomic) IBOutlet UITextField *ipInput;
@end
