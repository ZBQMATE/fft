#include <math.h>
#include "complex.cpp"
#include "complex.h"

using namespace std;

double pi = 3.1415926535897932;

//input data array should contain 2 to the n integrals

//ipt_length = 2 to (ipt_power_index)
complex* trs(complex ipts[], int ipt_power_index, int ipt_length) {
	
	//INVERSE ORDER, USING RADER ALGORITHM
	
	int a, b, c = 0;
	complex temp;
	
	complex ipt[ipt_length];
	
	for (int kkk = 0; kkk < ipt_length; kkk++) {
		ipt[kkk].real = ipts[kkk].real;
		ipt[kkk].img = ipts[kkk].img;
	}
	
	for (a = 0; a < ipt_length - 1; a++) {
		
		if (a < b) {
			temp.real = ipt[a].real;
			temp.img = ipt[a].img;
			ipt[a].real = ipt[b].real;
			ipt[a].img = ipt[b].img;
			ipt[b].real = temp.real;
			ipt[b].img = temp.img;
		}
		
		c = ipt_length / 2;
		
		while (c > 1 & c <= b) {
			b = b - c;
			c = c / 2;
		}
		
		b = b + c;
		
	}
	
	
	//fast fourier transfer
	
	complex para;
	complex update_para;
	complex tempb;
	
	for (int lyr = 1; lyr <= ipt_power_index; lyr++) {
		
		double pitemp = (double) lyr - 1;
		int gap = (int) pow(2, pitemp);
		
		para.real = 1;
		para.img = 0;
		
		update_para.real = cos(pi/gap);
		update_para.img = -sin(pi/gap);
		
		int prsa, prsb;
		for (int sd = 0; sd < gap; sd++) {
		//for (int sd = 0; sd < ipt_length; sd = sd + 2 * gap) {
			
			for (prsa = sd; prsa < ipt_length; prsa = prsa + 2 * gap) {
			//for (prsa = sd; prsa < sd + gap; prsa = prsa + 1) {
				prsb = prsa + gap;
				
				tempb = ipt[prsb].multi(para);
				ipt[prsa] = ipt[prsa].add(tempb);
				ipt[prsb] = ipt[prsa].subtract(tempb);
			}
			para = para.multi(update_para);
		}
	}
	
	/**/
	complex *ipti;
	ipti = ipt;
	return ipti;
}