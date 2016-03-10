#import <Foundation/Foundation.h>

@interface Shape: NSObject
{
    int size;
    int type;
}
-(int)shapeSize;
-(int)shapeType;

-(void)setShapeType: (int) t;
-(void)setShapeParam: (int) s type: (int) t;

-(void)printShape;

@end

@interface Rectangle: Shape
{
    int width;
    int height;
}

-(void)setWidth:(int) w andHeight: (int) h;
@end
