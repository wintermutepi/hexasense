#ifndef HYT271_H
#define HYT271_H 1

typedef enum {
  HYT271_ERROR_NONE = 0,
  HYT271_BUS_ERROR,
} HYT271_ERROR_t;


HYT271_ERROR_t hyt271_get_measurements(
    double* humidity, double* temperature);

#endif /* HYT271_H */

