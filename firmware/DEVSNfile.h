/* 
 * File:   DEVSNfile.h
 * Author: saura
 *
 * Created on 22 August, 2022, 2:56 PM
 */

#ifndef DEVSNFILE_H
#define	DEVSNFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    const long int DEVSN0LIST[13] = {
        
        0x002e0054,
        0x00360036,
        0x002e004b,
        0x002e0035,
        0x002c0033,
        0x002d0031,
        0x0030003d,
        0x002e004c, //V7

        0x002f0036, 
        0x002f003c,
        0x002f004b,
        0x002f0034,
        0x002c002c,//V6
    };
    
    const long int DEVSN1LIST[13] = {

        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,
        0x50424600,//V6
                
    };
    
#ifdef	__cplusplus
}
#endif

#endif	/* DEVSNFILE_H */

