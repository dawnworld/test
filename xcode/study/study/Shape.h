//
//  Shape.h
//  study
//
//  Created by mac on 13-10-10.
//  Copyright (c) 2013年 dawnworld. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Shape : NSObject
{
    int size;
    NSString* name;
}

-(int) shapeSize;
-(NSString*) shapeName;
-(void) printShape;
@end
