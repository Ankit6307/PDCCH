#include "header.h"

void SUB_BLOCK_INT(STREAM_T &inData, hls::stream<datau64b> &cnData,STREAM_T &outData)
{
#pragma HLS interface ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=inData
#pragma HLS INTERFACE axis port=outData
#pragma HLS INTERFACE axis port=cnData
    data_axi input[4];//input array,each cell can store 128 bits and max size is 512
    data_axi outdata[4];
    datau64b cn;// variable to get the configuration parameters
    int j,N,k,kmax,A,a,t;
    int n=0;
    int  l=0;
    int  m=0;
    cn=cnData.read();// Read operation of configuration parameters
    N=cn.range(18,8);// Getting the N value from higher layer, ie from configuration parameters
   ap_uint<16> var[32];//var is an array of size 32 and each cell stores 4bit,8bits or 16bits depending on the input size 128,256 or 512 respectiveley.
//Depending on N value parameter A is calculated as N/32 and kmax as 128/A as our top function SUB_BLOCK_INT can have max input and output size of 128 bits
if(N==512)
	    {
	    	A=16;
	       kmax=8;
	    }
	    else if(N==256)
	    {
	    	A=8;
	    	kmax=16;

	    }
	    else if(N==128)
	    {

	    	A=4;
	    	kmax=32;
	    }
	a=N/128;//a defines the number of bursts of data, here 1 burst=128 bits
    for(t=0;t<a;t++)
    {
//#pragma HLS loop_tripcount min=1 max=4//loop trip count pragma defined to explicitly mention the min and max times this loop runs
      input[t]=inData.read();//Read the input 'a' number of times

    for(j=127;j>0;j=j-A)
    	{
#pragma HLS loop_tripcount min=1 max=4//loop trip count pragma defined to explicitly mention the min and max times this loop runs

    		var[P[n]]=input[t].data.range(j,j-A+1);//store 'A' bits at a time from MSB of input data to var array at location defined by SBI interleaver pattern P[n]
    		n++;

        }
    }

    m=127;
    for(t=0;t<a;t++)
    {
      for(k=0;k<kmax;k++)
    {

    	outdata[t].data.range(m,m-A+1)=var[l];//Store the interleaved bits sequentially from var array to output
    	l++;
    	m=m-A;

     outdata[t].last=0;

    }
      if(inData.empty()&& t==a-1)
                {
              	  outdata[t].last=1;//Tlast is asserted on the last transaction of the block
                }
     m=127;

    outData.write(outdata[t]);

    }

}
