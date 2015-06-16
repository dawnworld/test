#import "foo.h"

@implementation Shape: NSObject
- (int)shapeSize
{
    return size;
}

- (int)shapeType
{
    return type;
}

- (void)setShapeType: (int) t
{
    type = t;
}

- (void)setShapeParam: (int) s type: (int) t
{
    size = s;
    type = t;
}

-(void) printShape
{
    NSLog(@" This Shape size:%d, type:%d\n", size, type);
}
@end

@implementation Rectangle: Shape
-(id)init
{
    if(self = [super init])
    {
        type = 2;
    }
    return self;
}

-(void) setWidth:(int) w andHeight: (int)h
{
    width = w;
    height = h;

    size = width * height;
}
@end
int main()
{
    Shape *mShape = [Shape new];
    [mShape setShapeParam :5 type :1];
    [mShape printShape];

    Rectangle *rect = [[Rectangle alloc] init];
    [rect setWidth: 5 andHeight: 6];
    [rect printShape];
    [rect release];
    return 0;
}
