#include "mbed.h"
#include "LSM6DSLSensor.h"

#define PI 3.141592654

static DevI2C devI2c(PB_11,PB_10);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address

float computeAngle(int x, int y, int z){
    float fx = (float) x;                       //Cast int x to float fx
    float fy = (float) y;                       //Cast int y to float fy
    float fz = (float) z;                       //Cast int z to float fz
    float pitch = 0;
    float res = 0;
    
    pitch = atan(-fx/(sqrt((fy*fy)+(fz*fz))));  //All rotations are counter-clockwise with respect to the axis, hence -fx
    res = pitch * (180/PI);                     //Convert from radians to degress
    return res;                                 //Will return a degree value between -90deg to +90deg.
}

/* Simple main function */
int main() {
    uint8_t id;
    int32_t axes[3];
    float res=0;
    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while(1) {
        acc_gyro.get_x_axes(axes);
        res = computeAngle(axes[0], axes[1], axes[2]);
        printf("LSM6DSL:        X=%6d,      Y=%6d,      Z=%6d,      Pitch=%3.2f deg.\r\n", axes[0], axes[1], axes[2], res);
        thread_sleep_for(2000);
    }
}