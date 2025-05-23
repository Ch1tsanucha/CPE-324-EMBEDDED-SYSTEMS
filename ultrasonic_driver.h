#ifndef ULTRASONIC_DRIVER_H
#define ULTRASONIC_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

void setup_ultrasonic();
float measure_distance();

#ifdef __cplusplus
}
#endif

#endif /* ULTRASONIC_DRIVER_H */
