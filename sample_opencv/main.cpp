#include <string>
using namespace std;
extern int sift_feature();
extern int harris_feature();
extern int adaptive_thresh( int, string file_name);
extern int ffilldemmo();
extern int geom_trans(const string file_name);

int main()
{
//  sift_feature();
//  harris_feature();
//     adaptive_thresh( 1, "adaptive_test_4.bmp");
//     ffilldemmo();
//     geom_trans("./affine.bmp");
    geom_trans("");
    return 0;
}