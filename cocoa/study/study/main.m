//
//  main.m
//  study
//
//  Created by mac on 13-10-10.
//  Copyright (c) 2013年 dawnworld. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Rectangle.h"

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        Shape *shape = [[Shape alloc] init];
        [shape printShape];
        
        Rectangle *rect = [[Rectangle alloc] init];
        [rect setRectWidth:5 andHeight:6];
        [rect printShape];
        
    }
    return 0;
}

