//
//  Rectangle.m
//  study
//
//  Created by mac on 13-10-10.
//  Copyright (c) 2013年 dawnworld. All rights reserved.
//

#import "Rectangle.h"

@implementation Rectangle

-(id) init
{
    if (self = [super init]) {
        width = 0;
        height = 0;
        name = @"Rectangle";
    }
    
    return self;
}

-(void) setRectWidth:(int)w andHeight:(int)h
{
    width = w;
    height = h;
    size = width * height;
}

@end
