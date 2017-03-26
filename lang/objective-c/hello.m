#import <Foundation/Foundation.h>
#import <stdio.h>

@interface Number: NSObject
{
@public
    int number;
}

- (void)printNum;
@end

@implementation Number: NSObject

- (void)printNum
{
    NSLog(@"Number:%d\n", number);
}

@end

int main(int argc, const char* argv[])
{
    printf("hello world\n");
    Number *myNumber = [Number new];
    myNumber->number = 6;
    [myNumber printNum];
    return 0;
}
