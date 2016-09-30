#include "comm.hpp"


static int write_data_to_file(char *file_name, char *data, int data_len)
{
	FILE *fp = fopen(file_name, "w+");
	if (fp==0) { printf("can't open file\n"); return 0;}
	fseek(fp, 0, SEEK_END);
	fwrite(data, data_len, 1, fp);
	fclose(fp);
	return 0;
}

#define MAX_LENTH 32

static int read_data_from_file_ascii(char *file_name, int **data, int *data_len)
{
	FILE *fp = fopen(file_name, "r");
	*data = (int *)malloc(1024 * 1024 * 32 * sizeof(int));

	char buffer[MAX_LENTH * 2] = {0};
	int read_data;
	int read_line = 0;
	while(fgets(buffer, MAX_LENTH, fp) != NULL)
	{
		if (buffer[0] == '0' && buffer[1] == 'X')
			sscanf(buffer, "0X%x", &read_data);
		if (buffer[0] == '0' && buffer[1] == 'x')
			sscanf(buffer, "0x%x", &read_data);
		else
			read_data = atoi(buffer);
		*(*data + read_line) = read_data;
//		printf("No.%d:%d\n", read_line, read_data);
		read_line++;
		memset(buffer, 0x0, MAX_LENTH * 2);
	}
	*data_len = read_line;
	
/*

	size_t string_size, read_size;
	char *buffer = NULL;
	//seek the last byte of the file  
	fseek(fp, 0 ,SEEK_END);  
	//offset from the first to the last byte, or in other words, filesize  
	string_size = ftell (fp);  
	//go back to the start of the file  
	rewind(fp);  

	//allocate a string that can hold it all  
	buffer = (char*) malloc (sizeof(char) * (string_size + 1) );  
	//read it all in one operation  
	read_size = fread(buffer,sizeof(char),string_size,fp);  
	//fread doesnt set it so put a \0 in the last position  
	//and buffer is now officialy a string  
	buffer[string_size+1] = '\0';  

	if (string_size != read_size) {  
		//something went wrong, throw away the memory and set  
		//the buffer to NULL  
		free(buffer);  
		buffer = NULL;  
	}  
*/

	fclose(fp);
	return 0;
}


int dat_to_image_16u(char *file_name, CvSize size)
{
	int *data = NULL, data_temp;
	int data_len;
	read_data_from_file_ascii(file_name, &data, &data_len);
	int index = 0;
	IplImage *p_image = cvCreateImage(size, IPL_DEPTH_16U, 1);
	cvSetZero(p_image);
	int i, j;
	unsigned short *img_data;
	for (i = 0; i < p_image->height; i++)
	{
		img_data = (unsigned short *)(p_image->imageData + i * p_image->widthStep);
		for (j = 0; j < p_image->width; j++)
		{
			data_temp = *(data + index);
			index ++;
			*(img_data + j) = (unsigned short)data_temp;
// 			CV_IMAGE_ELEM(p_image, unsigned short, i, j) = (unsigned short)data_temp;
		}
	}
	cvNamedWindow(file_name);
	cvShowImage(file_name, p_image);
	cvWaitKey();
	char fall_name[128] = {0};
	sprintf(fall_name, "%s.bmp", file_name);
	cvSaveImage(fall_name, p_image);
	cvDestroyWindow(file_name);
	cvReleaseImage(&p_image);

	if (NULL != data)
	{
		free(data);
		data = NULL;
	}
	return 0;
}


int dat_to_image_8u(char *file_name, CvSize size)
{
	int *data = NULL, data_temp;
	int data_len;
	read_data_from_file_ascii(file_name, &data, &data_len);
	int index = 0;
	IplImage *p_image = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvSetZero(p_image);
	int i, j;
	unsigned char *img_data;
	for (i = 0; i < p_image->height; i++)
	{
		img_data = (unsigned char *)(p_image->imageData + i * p_image->widthStep);
		for (j = 0; j < p_image->width; j++)
		{
			data_temp = *(data + index);
			index ++;
			*(img_data + j) = (unsigned char)data_temp;
// 			CV_IMAGE_ELEM(p_image, unsigned short, i, j) = (unsigned short)data_temp;
		}
	}
	cvNamedWindow(file_name);
	cvShowImage(file_name, p_image);
	cvWaitKey();
	char fall_name[128] = {0};
	sprintf(fall_name, "%s.bmp", file_name);
	cvSaveImage(fall_name, p_image);
	cvDestroyWindow(file_name);
	cvReleaseImage(&p_image);

	if (NULL != data)
	{
		free(data);
		data = NULL;
	}
	return 0;
}

#define LEN	400
static int generate_h_file(IplImage *p_image, char *file_name)
{
	int i, j;
	if (NULL == p_image)
	{
		return -1;
	}	

	char data_1[LEN] = {0};
	char data[8] = {0};
	//每行字节对齐
	char *data_write = (char *)malloc((p_image->widthStep * p_image->height) * 7 + LEN);
	memset(data_write, 0x0, (p_image->widthStep * p_image->height) * 7  + LEN);
	char *data_write_tmp = data_write;
	char *data_image = NULL;

	sprintf(data_1, "//the image is stored according to the width_step\n"\
		"//the blank area will be filled with 0xff\n"\
		"//(width:%d) widthstep:%d, height:%d\nchar dsp_image[%d]={\n", p_image->width, p_image->widthStep, p_image->height, (p_image->widthStep * p_image->height));
	memcpy(data_write_tmp, data_1, strlen(data_1));
	data_write_tmp += strlen(data_1);

	for (i = 0; i < p_image->height; i++)
	{
		data_image = p_image->imageData + i * p_image->widthStep;
		for (j = 0; j < p_image->widthStep; j++)
		{
			memset(data, 0x0, sizeof(data));
			if (j >= p_image->width)
				sprintf(data, "0x%02x,", 0xff);
			else
				sprintf(data, "0x%02x,", (unsigned char)*(data_image + j));
			memcpy(data_write_tmp, data, strlen(data));
			data_write_tmp += strlen(data);
		}

		if (i < p_image->height - 1)
		{
			memset(data, 0x0, sizeof(data));
			sprintf(data, "\n");
			memcpy(data_write_tmp, data, 1);
			data_write_tmp += 1;
		}
	}
	data_write_tmp -= 1;
	memcpy(data_write_tmp, "};\n", 3);

	write_data_to_file(file_name, data_write, strlen(data_write));
	free(data_write);	
}


int img_to_hex(char *file_name)
{
	char scan_data[16] = {0};
	int channel_flag = 3;

	IplImage *p_image = cvLoadImage(file_name, NULL);
	cvNamedWindow(file_name);
	cvShowImage(file_name, p_image);
	cvWaitKey();
	cvDestroyWindow(file_name);

	IplImage *p_image_gray = cvCreateImage(cvGetSize(p_image), IPL_DEPTH_8U, 1);

	memset(scan_data, 0x0, sizeof(scan_data));
	printf("going to create gray image? y/n:");
	scanf("%s",scan_data);
	if (scan_data[0] == 'y' || scan_data[0] == 'Y')
	{
		if (p_image->nChannels >= 3)
			cvConvertImage(p_image, p_image_gray, CV_RGB2GRAY);
		else
			cvCopy(p_image, p_image_gray);
		channel_flag = 1;
	}

	channel_flag = 1;
	if (1 == channel_flag)
	{

	}
	else
		;
	char fall_name[128] = {0};
	sprintf(fall_name, "%s.h", file_name);
	generate_h_file(p_image, fall_name);

	
	cvReleaseImage(&p_image);
	cvReleaseImage(&p_image_gray);
	return 0;
}





int main(int argc, char *argv[])
{
	int ret  = 0;
	if (argc < 3)
	{
		printf("please specify the image name!");
		return 0;
	}

	ifstream fin(argv[2]);
	if (!fin)
	{
		std::cout << "can not open this file\:"<<argv[1]<< endl;
		return 0;
	}
	fin.close();

	if (!strcmp(argv[1], "-f"))
	{
		CvSize size;
		int depth = 0;
		size.height = 0;
		size.width = 0;
		printf("please input the depth, width, height:");
		scanf("%d,%d,%d", &depth, &size.width, &size.height);
		printf("depth:%d, width:%d, height:%d\n", depth, size.width, size.height);
		if (depth == 8)
			dat_to_image_8u(argv[2], size);
		else
			dat_to_image_16u(argv[2], size);
	}
	else if (!strcmp(argv[1], "-v"))
	{
		img_to_hex(argv[2]);
	}
	else
		return 0;
	return 0;
}