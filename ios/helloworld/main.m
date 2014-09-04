#import <Foundation/Foundation.h>

int main(int argc, const char *argv[])
{
#if 0
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSLog(@"Hello, Objective-C world!");
    [pool drain];
#else
    @autoreleasepool {
        NSLog(@"Hello, Objective-C world!");
    }
#endif

    return 0;
}
