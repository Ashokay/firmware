
#ifndef ROTOR_DQ_H
#define	ROTOR_DQ_H


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    
void rotordqf_init(float, float, int); 
//initialize harmonic oscillator cos0, sin0, rotation direction
	
void rotordqf_ho(float, float, float *, float *); 
//w, dt, cosRDQ, sinRDQ 	
	
void rotordqf_park(float, float, float, float*, float*); //abc to dq
// u, v, w, *d, *q
	
void rotordqf_clark(float, float, float*, float*, float*); //dq to abc
//d, q, *u, *v, *w

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

