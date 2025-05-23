/* 
 * File:   motor_driver.h
 * Author: BB
 *
 * Created on 20 ??????? 2568, 16:24 ?.
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

void fd(int speed);
void tr(int speed);
void tl(int speed);
void st(void);
void sr(void);
void sl(void);
void bk(void);
void motor_init(void);
int track(void);
void slcheck(void);
void srcheck(void);
void Tableone(void);


#ifdef __cplusplus
}
#endif

#endif /* MOTOR_DRIVER_H */
