#include "fft.cpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>

using namespace std;

//input and output images 24bits, without stuffing bits

int picfft() {
	
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	
	FILE *pic = fopen("./h.bmp", "rb");
	FILE *picout = fopen("./spectrum.bmp", "wb");
	
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, pic);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, pic);
	
	int height = infoheader.biHeight;
	int width = infoheader.biWidth;
	int picsize = height * width;
	
	unsigned char img[height*width*3];
	
	for (int i = 0; i < height*width*3; i++) {
		img[i] = 0;
	}
	
	//read input images
	
	unsigned char input_img[height*width*3];
	fread(&input_img, height*width*3, 1, pic);
	
	//main cavans
	unsigned char blue_img[height*width];
	
	int one;
	double done;
	for (int i = 0; i < height*width; i++) {
		blue_img[i] = input_img[i * 3];
	}
	
	//set power index, eg 2^9 = 512
	//int ipt_power_index = 9;
	
	complex cur_height[height];
	complex cur_width[width];
	complex *cur_height_ind;
	complex *cur_width_ind;
	int logh = 8;
	int logw = 8;
	
	//fourier on height demention
	complex tpp;
	
	for (int www = 0; www < width; www++) {
		for (int i = 0; i < height; i++) {
			one = www + i;
			done = (double) one;
			done = pow(-1, done);
			
			cur_height[i].real = (double) blue_img[www * height + i];
			cur_height[i].real = done * cur_height[i].real;
			cur_height[i].img = 0;
		}
		
		cur_height_ind = trs(cur_height, logh, height);
		
		for (int i = 0; i < height; i++) {
			
			//blue_img[www * height + i] = (int) cur_height[i].real;
			tpp = *cur_height_ind;
			blue_img[www * height + i] = (int) tpp.real / height;
			cur_height_ind++;
		}
		
	}
	
	//fourier on width demention
	for (int hhh = 0; hhh < height; hhh++) {
		for (int i = 0; i < width; i++) {
			one = hhh + i;
			done = (double) one;
			done = pow(-1, done);
			
			cur_width[i].real = (double) blue_img[i * width + hhh];
			cur_width[i].real = done * cur_width[i].real;
			cur_width[i].img = 0;
		}
		
		cur_width_ind = trs(cur_width, logw, width);
		
		for(int i = 0; i < width; i++) {
			//blue_img[hhh * width + i] = (int) cur_width[i].real;
			tpp = *cur_width_ind;
			blue_img[i * width + hhh] = (int) tpp.real / width;
			//if (hhh = 56) {
			//	printf("out = %d\n", blue_img[i * width + hhh]);
			//}
			//blue_img[i * width + hhh] = abs(blue_img[i * width + hhh]);
			cur_width_ind++;
		}
	}
	
	//put out the blue image to a white bmp
	for (int i = 0; i < height*width; i++) {
		img[i * 3 + 0] = blue_img[i];
		img[i * 3 + 1] = blue_img[i];
		img[i * 3 + 2] = blue_img[i];
	}
	
	
	
	
	//save the spectrum
	
	//infoheader.biBitCount = 24;
	
	fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, picout);
	fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, picout);
	fwrite(img, picsize, 3, picout);
	
	fclose(pic);
	fclose(picout);
	
	return 0;
	
}

int main() {
	picfft();
}