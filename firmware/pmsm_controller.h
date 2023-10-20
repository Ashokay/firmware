
#ifndef PMSM_CONTROLLER_H
#define	PMSM_CONTROLLER_H


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void DEVSN_verification(void);

void pmsm_init(float, float, float, float, float, float, float, int);
//DT, Rph, Lph, PHIph, wMR, cosMR, sinMR 

void PMSM_control_variables(float, float, float, float);
//kp_sc, ki_sc, w saturation, angle_margin

void current_control_variables(float, float, float);
//kp_icon, ki_icon, dqduty saturation

void adaptive_current_control(void);

void pmsm_model(float); 
//Vdc

void sensorless_control(void);

void sensor_control(void);

void current_control(float, float, float);
//Idref, Iqref

void PMSM_operation(float, float, float, float, float, float, float, float, float);
//wMR, cosMr, sinMR, Vdc, Iu, Iv, Iw, Idref, Iqref, 

void PMSM_condition( float *, float *, float *, float *, float *, float *, float *, float *, float *);
//*du, *dv, *dw, dd, dq, Id, Iq, Ed, Eq

void PMSM_angle_hold(float, float, float, float, float, float, float, float *, float *, float *);
//cosMr, sinMR, Iu, Iv, Iw, Idref, Iqref, *du, *dv, *dw

int PMSM_fault_check(void); // 2 special faults

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
