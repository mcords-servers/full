#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define max(x,y) (x>y?x:y)
#define min(x,y) (x<y?x:y)
#define dif(x,y) (max(x,y)<=0?abs(x+y):max(x,y)-min(x,y))
#define round(x) (x>0?x+1.0-fmod(x,1.0):x-fmod(x,1.0))
// #define chunk_moved(a,b) dif(round(a/16.0),round(b/16.0))>0
#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)


#define chunk_moved(a, b) ((((a) >= 0 ? (a) / 16 : ((a) - 15) / 16)) != ((b) >= 0 ? (b) / 16 : ((b) - 15) / 16))

#define chunk(a,b) (int)floor(a/16.0)!=((int)floor(b/16.0))
// int chunk_moved(int a, int b) {
//     int v1 = round(a/16.0);
//     int v2 = round(b/16.0);
//     int v3 = dif(v1,v2)>0;

//     LOG("%d, %d, %d", v1,v2,v3);
//     return v3;
// }

#define None 0

#define TAG_Test(buf, val) (val==None)?LOG("a"):None

int main() {
    TAG_Test(0,0);
    TAG_Test(0,1);
    return 0;
}