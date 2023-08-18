#include "header.h"

void golden_sequence(cn_STREAM &cnData, out_STREAM &outData)
{
    #pragma HLS INTERFACE axis port=outData
	#pragma HLS INTERFACE axis port=cnData
	#pragma HLS INTERFACE ap_ctrl_none port=return

	datau64b cnTemp = cnData.read();
	datau11b E = cnTemp.range(29,19);
	datau16b N_cell_id = cnTemp.range(47,32);
	datau16b RNTI = cnTemp.range(63,48);

	datau31b x1,x2;
	datau31b x1temp,x2temp;
	x1 = 1;
	x2 = (RNTI*(1<<16)) + + N_cell_id;
	x2 %= (1<<31);
	datau1b new_val1, new_val2;
	datau108b temp = 0;
	datau5b burst = 0;
	datau5b e = 0;
	output_data_axi output;

	for(int i=1;i<=60;i++){
#pragma HLS LOOP_TRIPCOUNT min=60 max=60
#pragma HLS PIPELINE
		x1temp = x2temp = 0;
		if(i==60){
			for(int j=0;j<7;j++){
#pragma HLS LOOP_TRIPCOUNT min=7 max=7
#pragma HLS PIPELINE
				x1temp[j] = x1[j]^x1[j+3];
				x2temp[j] = x2[j]^x2[j+1]^x2[j+2]^x2[j+3];
			}
			x1 = (x1>>7)+(x1temp<<24);
			x2 = (x2>>7)+(x2temp<<24);
		}else{
			for(int j=0;j<27;j++){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=27 max=27
				x1temp[j] = x1[j]^x1[j+3];
				x2temp[j] = x2[j]^x2[j+1]^x2[j+2]^x2[j+3];
			}
			x1 = (x1>>27)+(x1temp<<4);
			x2 = (x2>>27)+(x2temp<<4);
		}
	}


		burst = E/108;
		for(e=0;e<16;e++){
#pragma HLS LOOP_TRIPCOUNT min=1 max=16
#pragma HLS PIPELINE
			if(e==burst)break;
			for(int a=0;a<4;a++){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=4 max=4
				x1temp = x2temp = 0;
				for(int b=0;b<27;b++){
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=27 max=27
					x1temp[b] = x1[b]^x1[b+3];
					x2temp[b] = x2[b]^x2[b+1]^x2[b+2]^x2[b+3];
					temp[107-b-a*27] = x1[b]^x2[b];
				}
				x1 = (x1>>27)+(x1temp<<4);
				x2 = (x2>>27)+(x2temp<<4);
			}
			output.data = temp;
			output.last = (e==burst-1);
			outData.write(output);
			temp = 0;
		}
}


