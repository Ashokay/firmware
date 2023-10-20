#include "rotor_dqf.h" 
#include "pmsm_controller.h"
#include "math.h"
#include "device.h"
#include "DEVSNfile.h"
#include "stdlib.h"
#include "default_ESC_config.h"

	#define TDUTY_SATURATION 0.97

    static int DEVSN_valid = 0;
	
	static float dt = 0.00004;
	static float wSensor = 0;
	static float cosSensor = 0, sinSensor = 0;
	static float cosRDQ = 0, sinRDQ = 0;
	
	static float wSlimited = 0;
	static float du = 0, dv = 0, dw = 0;
	static float Ed = 0, Eq = 0;	
	static float dduty = 0, qduty = 0;	
	static float Id = 0, Iq = 0;
	static float R, L, PHI;
	
	static float cosPH, sinPH, cosNH, sinNH;
	static float kp_sensorless_control = 2, ki_sensorless_control = 400;
	static float dwSless_saturation;
	static float fwPL = 0, fwNL = 0;
	
	static float dduty_integrator = 0, qduty_integrator = 0; 
	static float dduty_saturation = 1.0, qduty_saturation = 1.0;
    
    static int possible_fault = 0;

    #define KP_ICON_MIN 0.00005           
    #define KI_ICON_MIN 0.05

    #define KP_ICON_MAX 0.001           
    #define KI_ICON_MAX 0.5  

    #define ICON_WMAX 3000.0   

void pmsm_init(float DT, float Rph, float Lph, float PHIph, float wMR, float cosMR, float sinMR, int dir_rot)
{
	wSensor = wMR;
	wSlimited = wSensor;
	cosRDQ = cosMR; 
    sinRDQ = sinMR; 

	dt = DT; 
	R = Rph;
	L = Lph;
	PHI = PHIph;
	
	fwPL = wSensor;
	fwNL = wSensor;
	
	dduty_integrator = 0;
	qduty_integrator = 0; 
    
    dduty = 0;
    qduty = 0;
    
    du = 0;
    dv = 0;
    dw = 0;
            
}

void PMSM_control_variables(float kp_sc, float ki_sc, float w_sat, float hysteresis_angle)
{
	cosPH = cos(-hysteresis_angle * 3.14159265/180);
    cosNH = cosPH;      
    
    sinPH = sin(-hysteresis_angle * 3.14159265/180);
    sinNH = -sinPH;     
	
	kp_sensorless_control = kp_sc;
	ki_sensorless_control = ki_sc;	
	
	dwSless_saturation = w_sat;		
}

void current_control_variables(float kp_icon, float ki_icon, float dutysat)
{
    float Wabs = 0;
    Wabs = Wabs;

	
	dduty_saturation = 0;
    qduty_saturation = 0;
}

void adaptive_current_control()
{
    float Wabs = 0;
    Wabs = Wabs;

}

void pmsm_model(float Vdc) 
{
    float Vu, Vv, Vw;
    float Vd, Vq;
  
    Vu = du * Vdc * 0.5;
    Vv = dv * Vdc * 0.5;
    Vw = dw * Vdc * 0.5;
        
    rotordqf_park(Vu, Vv, Vw, &Vd, &Vq);
                 
}


void sensorless_control()
{
    Nop();

    
}


void sensor_control()
{
	Nop();
}


void current_control(float Idref, float Iqref, float iVdc)
{
     dduty = 0;
     qduty = 0;    
}

void PMSM_operation(float wMR, float cosMR, float sinMR, float Vdc, float Iu, float Iv, float Iw, float Idref, float Iqref)
{
    float dmax, dmin, d0;
    
	wSensor = wMR;
	cosSensor = cosMR;
	sinSensor = sinMR;
	
	rotordqf_park(Iu, Iv, Iw, &Id, &Iq);				//find Id, Iq
	pmsm_model(Vdc);									//Find Ed, Eq based on prev. duty

    //wSlimited = 314;
    cosRDQ = cosSensor;
    sinRDQ = sinSensor;
    rotordqf_ho(wSlimited, dt, &cosRDQ, &sinRDQ);	//Find next angle
    
	current_control(Idref, Iqref, Vdc);					//Find next Id, Iq, dd, dq	
    
    rotordqf_clark(dduty, qduty, &du, &dv, &dw);		//Find next du, dv, dw
    
    if(du > dv)
    {
        dmax = du;
        dmin = dv;
        if(dw > dmax) dmax = dw;
        else if (dw < dmin) dmin = dw;
    }
    else
    {
        dmax = dv;
        dmin = du;       
        if(dw > dmax) dmax = dw;
        else if (dw < dmin) dmin = dw;
    }    
    
    d0 = (dmax + dmin)/2;
    
    du = du - d0;
    dv = dv - d0;
    dw = dw - d0;	
    
	if(du > TDUTY_SATURATION) du = TDUTY_SATURATION;
    if(du < -TDUTY_SATURATION) du = -TDUTY_SATURATION;
	
    if(dv > TDUTY_SATURATION) dv = TDUTY_SATURATION;
    if(dv < -TDUTY_SATURATION) dv = -TDUTY_SATURATION;
    
    if(dw > TDUTY_SATURATION) dw = TDUTY_SATURATION;
    if(dw < -TDUTY_SATURATION) dw = -TDUTY_SATURATION;
}

void PMSM_condition(float *Adr_du, float *Adr_dv, float *Adr_dw, float * Adr_Dd, float * Adr_Dq, float * Adr_Id_act, float * Adr_Iq_act, float * Adr_Ed_act, float * Adr_Eq_act)
{
	*Adr_du = du;
	*Adr_dv = dv;
	*Adr_dw = dw;		
    
    *Adr_Dd = dduty;
    *Adr_Dq = qduty;
    
    *Adr_Id_act = Id;
    *Adr_Iq_act = Iq;
    
    *Adr_Ed_act = Ed;
    *Adr_Eq_act = Eq;
}

void PMSM_angle_hold(float cosMR, float sinMR, float Iu, float Iv, float Iw, float Idref, float Iqref, float *Adr_du, float *Adr_dv, float *Adr_dw)
{
    cosRDQ = cosMR;
    sinRDQ = sinMR;	
    
	rotordqf_park(Iu, Iv, Iw, &Id, &Iq);			//find Id, Iq  
    
    rotordqf_ho(0, 0, &cosRDQ, &sinRDQ);           //Find next angle
    
	current_control(Idref, Iqref, 48.0);					//Find next Id, Iq, dd, dq	      
	
    rotordqf_clark(dduty, qduty, &du, &dv, &dw);	//Find next du, dv, dw

	if(du > TDUTY_SATURATION) du = TDUTY_SATURATION;
    if(du < -TDUTY_SATURATION) du = -TDUTY_SATURATION;
    
    if(dv > TDUTY_SATURATION) dv = TDUTY_SATURATION;
    if(dv < -TDUTY_SATURATION) dv = -TDUTY_SATURATION;
    
    if(dw > TDUTY_SATURATION) dw = TDUTY_SATURATION;
    if(dw < -TDUTY_SATURATION) dw = -TDUTY_SATURATION;	
	
	*Adr_du = du;
	*Adr_dv = dv;
	*Adr_dw = dw;		
}

int PMSM_fault_check(void)
{
    int fault_id = 0;
    if(fabs(wSensor) > 500)
    {
        if(possible_fault == 1)  {fault_id = 0;}        
        if(possible_fault == 2)  {fault_id = 0;}        
    }
    else possible_fault = 0;
    //fault_id = 0;
    
    if(DEVSN_valid == 0) fault_id = 8;
    
    return fault_id;
}

void DEVSN_verification(void)
{
    volatile long int PICid0 = 0, PICid1 =0;
    volatile int DEVSNlist_size = 0;
    int list_counter = 0;
    
    DEVSNlist_size = sizeof(DEVSN0LIST)/sizeof(DEVSN0LIST[0]);
    
    DEVSN_valid = 1;
    for(list_counter = 0; list_counter < DEVSNlist_size; list_counter++)
    {
        PICid0 = DEVSN0LIST[list_counter];
        PICid1 = DEVSN1LIST[list_counter];
        
        if((PICid0 == DEVSN0) & (PICid1 == DEVSN1))
        {
            DEVSN_valid = 1;
        }
    }    
}