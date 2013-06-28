//
//  FESettingViewController.m
//  avDemo
//
//  Created by Pharaoh on 13-6-28.
//  Copyright (c) 2013年 free. All rights reserved.
//

#import "FESettingViewController.h"

@interface FESettingViewController ()

@end

@implementation FESettingViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
}
- (void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.currentIP.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"ip"];
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)textFieldDidEndEditing:(UITextField *)textField{
   
    
}
- (BOOL)textFieldShouldReturn:(UITextField *)textField{

    return YES;
}
- (IBAction)close:(id)sender{
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (IBAction)save:(id)sender{
    NSString* userSettingIP = self.ipInput.text;
    [[NSUserDefaults standardUserDefaults] setObject:userSettingIP forKey:@"ip"];
    self.currentIP.text = [[NSUserDefaults standardUserDefaults] objectForKey:@"ip"];
    [self.ipInput resignFirstResponder];
}

- (void)dealloc {
    [_ipInput release];
    [_currentIP release];
    [super dealloc];
}
@end
