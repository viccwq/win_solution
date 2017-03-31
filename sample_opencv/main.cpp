#include <string>
using namespace std;
extern int sift_feature();
extern int harris_feature();
extern int adaptive_thresh( int, string file_name);

int main()
{
//     sift_feature();
//    harris_feature();
    adaptive_thresh( 1, "adaptive_test_4.bmp");
    return 0;
}