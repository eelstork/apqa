
#import "invert_matrix4x4.h"
#import <stdbool.h>

float inv4x4(int i,int j,const float* m){

	int o = 2+(j-i);
	
	i += 4+o;
	j += 4-o;
	
	#define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
	
	float inv =
	 + e(+1,-1)*e(+0,+0)*e(-1,+1)
	 + e(+1,+1)*e(+0,-1)*e(-1,+0)
	 + e(-1,-1)*e(+1,+0)*e(+0,+1)
	 - e(-1,-1)*e(+0,+0)*e(+1,+1)
	 - e(-1,+1)*e(+0,-1)*e(+1,+0)
	 - e(+1,-1)*e(-1,+0)*e(+0,+1);
	
	return (o%2)?inv : -inv;
	
	#undef e
	
}

bool inverseMatrix4x4(const float *m, float *out)
{
	
    float inv[16];
	
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			inv[j*4+i] = inv4x4(i,j,m);
	
	double D = 0;
	
	for(int k=0;k<4;k++) D += m[k] * inv[k*4];
	
    if (D == 0) return false;
	
    D = 1.0 / D;
	
    for (int i = 0; i < 16; i++)
        out[i] = inv[i] * D;
	
    return true;
	
}

#ifdef faster
float invf(int i,int j,const float* m){
	
	int o = j-i;
	
	i += o;
	j -= o;
#define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
	
	float inv =
	+ e(7,1) * e(6,2) * e(5,3)
	+ e(7,3) * e(6,1) * e(5,2)
	+ e(5,1) * e(7,2) * e(6,3)
	- e(5,1) * e(6,2) * e(7,3)
	- e(5,3) * e(6,1) * e(7,2)
	- e(7,1) * e(5,2) * e(6,3);
	
	return (o%2)?inv : -inv;
	
#undef e
	
}
#endif

// some extra stuff ----------------------------------

double det4x4(const float* m)
{
	
    float inv[16];
	
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			
			inv[j*4+i] = inv4x4(i,j,m);
			
		}
	}
	
	double D = 0; for(int k=0;k<4;k++){
		D += m[k] * inv[k*4];
	}
	
    return D;
	
}

// ---------------------------------------------------

double det4x4_mesa(const float* m)
{
    float inv[16];
	double det;
	
    inv[0] =
	m[5]  * m[10] * m[15]
	- m[5]  * m[11] * m[14]
	- m[9]  * m[6]  * m[15]
	+ m[9]  * m[7]  * m[14]
	+ m[13] * m[6]  * m[11]
	- m[13] * m[7]  * m[10];
	
    inv[4] =
	 + m[4]  * m[11] * m[14]
	 + m[8]  * m[6]  * m[15]
	 + m[12] * m[7]  * m[10]
 	 - m[8]  * m[7]  * m[14]
 	 - m[12] * m[6]  * m[11]
	 - m[4]  * m[10] * m[15];
	
    inv[8] =
	m[4]  * m[9] * m[15] -
	m[4]  * m[11] * m[13] -
	m[8]  * m[5] * m[15] +
	m[8]  * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];
	
    inv[12] =
	- m[4]  * m[9] * m[14]
	+ m[4]  * m[10] * m[13]
	+ m[8]  * m[5] * m[14]
	- m[8]  * m[6] * m[13]
	- m[12] * m[5] * m[10]
	+ m[12] * m[6] * m[9];
	
    inv[1] =
    + m[1]  * m[11] * m[14]
	+ m[9]  * m[2] * m[15]
	+ m[13] * m[3] * m[10]
	- m[9]  * m[3] * m[14]
	- m[13] * m[2] * m[11]
	- m[1]  * m[10] * m[15];
	
    inv[5] = m[0]  * m[10] * m[15] -
	m[0]  * m[11] * m[14] -
	m[8]  * m[2] * m[15] +
	m[8]  * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];
	
    inv[9] = -m[0]  * m[9] * m[15] +
	m[0]  * m[11] * m[13] +
	m[8]  * m[1] * m[15] -
	m[8]  * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];
	
    inv[13] = m[0]  * m[9] * m[14] -
	m[0]  * m[10] * m[13] -
	m[8]  * m[1] * m[14] +
	m[8]  * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];
	
    inv[2] = m[1]  * m[6] * m[15] -
	m[1]  * m[7] * m[14] -
	m[5]  * m[2] * m[15] +
	m[5]  * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];
	
    inv[6] = -m[0]  * m[6] * m[15] +
	m[0]  * m[7] * m[14] +
	m[4]  * m[2] * m[15] -
	m[4]  * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];
	
    inv[10] = m[0]  * m[5] * m[15] -
	m[0]  * m[7] * m[13] -
	m[4]  * m[1] * m[15] +
	m[4]  * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];
	
    inv[14] = -m[0]  * m[5] * m[14] +
	m[0]  * m[6] * m[13] +
	m[4]  * m[1] * m[14] -
	m[4]  * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];
	
    inv[3] = -m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];
	
    inv[7] = m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];
	
    inv[11] = -m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];
	
    inv[15] = m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];
	
	//rintf("inv[0] (mesa) %.3f\n",inv[0]);
	//rintf("inv[4] (mesa) %.3f\n",inv[4]);
	//rintf("inv[8] (mesa) %.3f\n",inv[8]);
	//rintf("inv[12] (mesa) %.3f\n",inv[12]);
	
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	
	return det;
	
}

bool inverseMatrix4x4_mesa(const float* m, float* invOut)
{
    float inv[16], det;
    int i;
	
    inv[0] =
	  m[5]  * m[10] * m[15]
	- m[5]  * m[11] * m[14]
	- m[9]  * m[6]  * m[15]
	+ m[9]  * m[7]  * m[14]
	+ m[13] * m[6]  * m[11]
	- m[13] * m[7]  * m[10];
	
    inv[4] =
   -m[4]  * m[10] * m[15] +
	m[4]  * m[11] * m[14] +
	m[8]  * m[6]  * m[15] -
	m[8]  * m[7]  * m[14] -
	m[12] * m[6]  * m[11] +
	m[12] * m[7]  * m[10];
	
    inv[8] =
	m[4]  * m[9] * m[15] -
	m[4]  * m[11] * m[13] -
	m[8]  * m[5] * m[15] +
	m[8]  * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];
	
    inv[12] =
   - m[4]  * m[9] * m[14]
   + m[4]  * m[10] * m[13]
   + m[8]  * m[5] * m[14]
   - m[8]  * m[6] * m[13]
   - m[12] * m[5] * m[10]
   + m[12] * m[6] * m[9];
	
    inv[1] =
   -m[1]  * m[10] * m[15] +
	m[1]  * m[11] * m[14] +
	m[9]  * m[2] * m[15] -
	m[9]  * m[3] * m[14] -
	m[13] * m[2] * m[11] +
	m[13] * m[3] * m[10];
	
    inv[5] = m[0]  * m[10] * m[15] -
	m[0]  * m[11] * m[14] -
	m[8]  * m[2] * m[15] +
	m[8]  * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];
	
    inv[9] = -m[0]  * m[9] * m[15] +
	m[0]  * m[11] * m[13] +
	m[8]  * m[1] * m[15] -
	m[8]  * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];
	
    inv[13] = m[0]  * m[9] * m[14] -
	m[0]  * m[10] * m[13] -
	m[8]  * m[1] * m[14] +
	m[8]  * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];
	
    inv[2] = m[1]  * m[6] * m[15] -
	m[1]  * m[7] * m[14] -
	m[5]  * m[2] * m[15] +
	m[5]  * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];
	
    inv[6] = -m[0]  * m[6] * m[15] +
	m[0]  * m[7] * m[14] +
	m[4]  * m[2] * m[15] -
	m[4]  * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];
	
    inv[10] = m[0]  * m[5] * m[15] -
	m[0]  * m[7] * m[13] -
	m[4]  * m[1] * m[15] +
	m[4]  * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];
	
    inv[14] = -m[0]  * m[5] * m[14] +
	m[0]  * m[6] * m[13] +
	m[4]  * m[1] * m[14] -
	m[4]  * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];
	
    inv[3] = -m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];
	
    inv[7] = m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];
	
    inv[11] = -m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];
	
    inv[15] = m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];
	
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	
    if (det == 0)
        return false;
	
    det = 1.0 / det;
	
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
	
    return true;
	
}