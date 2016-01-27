#ifdef __WAND__
target[name[filterstep.h] type[include]]
#endif

#ifndef FILTERSTEP_H
#define FILTERSTEP_H

/**A simle first order IIR filter.
*/
inline double filterstep(double x,double y,double r)
	{return x*(1-r) + y*r;}

#endif
