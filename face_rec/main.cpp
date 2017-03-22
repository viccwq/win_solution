#include "comm_def_col.h"

//The Database of Faces
//http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html
//reference
//http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_tutorial.html

extern int eigenfaces(const char *argv1, const char *argv2);
extern int lbp_hist(const char *argv1);
extern int fill_demo( int argc, char** argv );
int main( int argc, char** argv )
{
	eigenfaces("./at.txt", "./output");
//	lbp_hist("./at.txt");
//	fill_demo(argc, argv);
	system("pause");
	return 0;
}