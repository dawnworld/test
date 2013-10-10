//
//  Rectangle.h
//  study
//
//  Created by mac on 13-10-10.
//  Copyright (c) 2013年 dawnworld. All rights reserved.
//

#import "Shape.h"

@interface Rectangle : Shape
{
    int width;
    int height;
}

-(void) setRectWidth: (int)w andHeight:(int) h;
@end
