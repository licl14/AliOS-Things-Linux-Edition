//#ifdef CONFIG_MATH

//#include <os_compat.h>
#include <math.h>   // Configuration header
#include "fdlibm.h"

#if 0
static const double huge = 1.0e300;
#endif

double floor(double x)
{
	int i0,i1,j0;
	unsigned i,j;
	i0 =  CYG_LIBM_HI(x);
	i1 =  CYG_LIBM_LO(x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;
	if(j0<20) {
		if(j0<0) {  /* raise inexact if x != 0 */
			if(huge+x>0.0) {/* return 0*sign(x) if |x|<1 */
				if(i0>=0) {i0=i1=0;} 
				else if(((i0&0x7fffffff)|i1)!=0)
					{ i0=0xbff00000;i1=0;}
			}
		} else {
			i = (0x000fffff)>>j0;
			if(((i0&i)|i1)==0) return x; /* x is integral */
			if(huge+x>0.0) {        /* raise inexact flag */
				if(i0<0) i0 += (0x00100000)>>j0;
				i0 &= (~i); i1=0;
			}
		}
	} else if (j0>51) {
		if(j0==0x400) return x+x;   /* inf or NaN */
		else return x;              /* x is integral */
	} else {
		i = ((unsigned)(0xffffffff))>>(j0-20);
		if((i1&i)==0) return x;     /* x is integral */
		if(huge+x>0.0) {            /* raise inexact flag */
			if(i0<0) {
				if(j0==20) i0+=1; 
				else {
					j = i1+(1<<(52-j0));
					if(j<(unsigned)i1) i0 +=1 ;     /* got a carry */
					i1=j;
				}
			}
			i1 &= (~i);
		}
	}
	CYG_LIBM_HI(x) = i0;
	CYG_LIBM_LO(x) = i1;
	return x;
}

//#endif // CONFIG_MATH
