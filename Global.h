//Define all variables here

//---------------------------------------------------------------
#include<iostream>
#include<fstream>

#include <HD/hd.h>
#include <HDU/hduVector.h>
#include <HDU/hduError.h>
//#include <opencv/highgui.h>
//#include <opencv/imgproc.hpp>
//#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
//---------------------------------------------------------------
using namespace std;

ofstream file("data.txt");
ofstream debug("debug.txt");
//---------------------------------------------------------------
#define MASTER_DEVICE_NAME  "Omni"
#define SLAVE_DEVICE_NAME  "slave"

#define LIMIT_FORCE	5
#define K_simul		0.1
#define SCALE		1 

//---------------------------------------------------------------
 HHD MasterHandle,SlaveHandle ; // Dual Phantom omni initilization
//HHD Master = hdInitDevice("Master");
//HHD SlaveHandle = hdInitDevice("SlaveHandle");

hduVector3Dd position[3], force[3], angles[3];
hduVector3Dd init_position[3], init_angles[3];
hduVector3Dd order_pos;

//--------------------------------------------------------------
double energy_user1[3], energy_user2[3];
double diff_e1[3], diff_e2[3], prev_e2[3], prev_e1[3];
double alpha[3];
double n[3];

double user1_x = 0;
double user1_y = 0;
double user1_z = 0;

double user2_x = 0;
double user2_y = 0;
double user2_z = 0;


int omni_cnt;

bool Phantom_Started;
bool switch_b = false;
//--------------------------------------------------------------

typedef struct _PACK {
	hduVector3Dd position;
	hduVector3Dd force;
	hduVector3Dd prev;
	hduVector3Dd disp;
	hduVector3Dd after_auth;
	hduVector3Dd angle;
}PACK;

PACK mst, slv, slv_p;
PACK simul;

//------------------Function Declarations----------------------

void omni_init(void);
void omni_calibarate(void);
void omni_start(void);
void close(void);
double Omni_Saturation(double force);
inline void master_omni();    //Master #1
inline void slave_omni();     //Master #2
inline void slave_premium();  //SlaveHandle (Phantom Premium)
inline void fixed_dominance();
//---------------------------------------------------------------
Eigen::Vector3d N(3), v(3), u(3), Up(3);
Eigen::Vector4d P(4), C(4);
Eigen::Vector4d Xd(4);
Eigen::Vector4d Xs(4);
Eigen::Vector4d us1(4);
Eigen::Vector4d us2(4);
Eigen::Vector4d Rd(4);
Eigen::Vector4d combine(4);   //slave position vector


Eigen::Vector4d mf(4), mv(4);
Eigen::Vector4d sf(4), sv(4);

Eigen::Matrix4d R1_inv(4, 4), R2_inv(4, 4);
Eigen::Matrix4d Rotation1(4, 4), Rotation2(4, 4);

/*******************************************************************************
Declare Debugging variables here  

******************************************************************************/