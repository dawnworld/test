//
//  Shape.m
//  study
//
//  Created by mac on 13-10-10.
//  Copyright (c) 2013年 dawnworld. All rights reserved.
//

#import "Shape.h"

@implementation Shape
-(id)init
{
    if (self = [super init]) {
        size = 0;
        name = @"Shape";
    }
    
    return self;
}

-(int)shapeSize
{
    return size;
}

-(NSString *) shapeName
{
    return name;
}

-(void) printShape
{
    NSLog(@" %@ size is %d\n", name, size);
}
@end
