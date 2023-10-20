#include "rotor_dqf.h"	
#include "device.h"

	#define MHO 0.816496610641479

    #define COS0 0.816496610641479
    #define SIN0 0
    #define COS120 -0.500000059604644
    #define SIN120 0.866025388240814
    #define COS240 -0.500000059604644
    #define SIN240 -0.866025388240814

	static float ho_cos0 = 0.816496610641479, ho_sin0 = 0;
	static float ho_cos120 = 0, ho_sin120 = 0;
	static float ho_cos240 = 0, ho_sin240 = 0;	
    
    static float c120 = COS120;
    static float s120 = SIN120;
    
    static float c240 = COS240;
    static float s240 = SIN240;


void rotordqf_init(float cos0, float sin0, int rotation_direction) //Initialize frame based on sensor
{
    if(rotation_direction == 0)
    {
        c120 = COS120;
        s120 = SIN120;        
        c240 = COS240;
        s240 = SIN240;
    }
    else
    {
        c120 = COS240;
        s120 = SIN240;        
        c240 = COS120;
        s240 = SIN120;        
    }
    
    ho_cos0 = cos0;
    ho_sin0 = sin0;

    ho_cos120 = ho_cos0 * c120 + ho_sin0 * s120; 
    ho_cos240 = ho_cos0 * c240 + ho_sin0 * s240;

    ho_sin120 = ho_sin0 * c120 - ho_cos0 * s120;
    ho_sin240 = ho_sin0 * c240 - ho_cos0 * s240;
}


void rotordqf_ho(float Ho_w, float dt, float *cosRDQ, float *sinRDQ)
{
    
    
    
	ho_cos0 = *cosRDQ;
    ho_sin0 = *sinRDQ;  
              
    ho_cos120 = ho_cos0 * c120 + ho_sin0 * s120;
    ho_cos240 = ho_cos0 * c240 + ho_sin0 * s240;

    ho_sin120 = ho_sin0 * c120 - ho_cos0 * s120;
    ho_sin240 = ho_sin0 * c240 - ho_cos0 * s240;
	  
}

void rotordqf_park(float r, float y, float b, float *d, float *q)
{
    float PT11, PT12, PT13, PT21, PT22, PT23;
    
    PT11 = ho_cos0 * r;
    PT12 = ho_cos120 * y;
    PT13 = ho_cos240 * b;
    
    PT21 = -ho_sin0 * r;
    PT22 = -ho_sin120 * y;
    PT23 = -ho_sin240 * b;
    
    *d = PT11 + PT12 + PT13;
    *q = PT21 + PT22 + PT23;
}

void rotordqf_clark(float d, float q, float *r, float *y, float *b)
{
    float PT11, PT12, PT21, PT22, PT31, PT32;
    
    PT11 = ho_cos0 * d;
    PT12 = -ho_sin0 * q;
    
    PT21 = ho_cos120 * d;
    PT22 = -ho_sin120 * q;
    
    PT31 = ho_cos240 * d;
    PT32 = -ho_sin240 * q;
    
    *r = PT11 + PT12 ;
    *y = PT21 + PT22;
    *b = PT31 + PT32;   
}