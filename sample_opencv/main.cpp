#include "comm_def_col.h"
using namespace std;
extern int sift_feature();
extern int harris_feature();
extern int adaptive_thresh( int, string file_name);
extern int ffilldemmo();
extern int geom_trans(const string file_name);
extern void vic_resize();
extern int tutorial_dft();

void lib_test_DFT()
{
    int a[12] = {1, 2, 3, 4, 5, 6, 20, 40, 0, -2, -5, 10};
    int b[12] = {2, 2, 3, 3, 4, 8 ,23, 12, 8, 12, -3, 0};

    double r_out[12] = {0.0};
    double i_out[12] = {0.0};

    double r_out_2[12] = {0.0};
    double i_out_2[12] = {0.0};
    
    DFT<int>(a, b, r_out, i_out, 12);
    IDFT<double>(r_out, i_out, r_out_2, i_out_2, 12);

    int count = 0;
    for (int i = 0; i < 12; i++)
    {        
        if (abs(a[i] - r_out_2[i]) > 0.001)
        {
            count++;
        }
        if (abs(b[i] - i_out_2[i]) > 0.001)
        {
            count++;
        }
    }
    cout<<count<<endl;
    return;
}

int main()
{
//  sift_feature();
//  harris_feature();
//     adaptive_thresh( 1, "adaptive_test_4.bmp");
//     ffilldemmo();
//     geom_trans("./affine.bmp");
//     geom_trans("");
//    vic_resize();
//    tutorial_dft();
    lib_test_DFT();
    system("pause");
    return 0;
}