/*******************************************************************************
Implementation file

***************************************************
****************************/
#include "Global.h"

//HHD MasterHandle = hdInitDevice("MasterHandle");   // Master #1 initilization

//HHD Master2 = hdInitDevice("master2");   // Master #2 initilization

//HHD SlaveHandle = hdInitDevice("slave");       // SlaveHandle initilization




//----------------------------Device Asynchronous Callback----------------------

HDCallbackCode HDCALLBACK Start_teleoperation(void *pUserData)

{
	if (omni_cnt <3000)   //delay of 4 sec
	{
		omni_cnt++;
	}

	else

	{
		
		master_omni();	
	//	fixed_dominance();

	
	}


	return HD_CALLBACK_CONTINUE;
}


/*******************************************************************************
Functions
*******************************************************************************/

void omni_init(void)

{
	MasterHandle = hdInitDevice(MASTER_DEVICE_NAME);
	hdEnable(HD_FORCE_OUTPUT);

#ifdef SLAVE_DEVICE_NAME
	SlaveHandle = hdInitDevice(SLAVE_DEVICE_NAME);
	hdEnable(HD_FORCE_OUTPUT);
#endif

	hdStartScheduler();
	//hdMakeCurrentDevice(Master);

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<=2; j++)
		
		{
			force[j][i] = 0.0;
			position[j][i] = 0.0;
			angles[j][i] = 0.0;
			init_position[j][i] = 0.0;
			init_angles[j][i] = 0.0;
		}
		
		mst.prev[i] = 0.0;
		mst.position[i] = 0.0;
		mst.force[i] = 0.0;
		mst.disp[i] = 0.0;
		mst.after_auth[i] = 0.0;
		mst.angle[i] = 0.0;

		slv.prev[i] = 0.0;
		slv.position[i] = 0.0;
		slv.force[i] = 0.0;
		slv.disp[i] = 0.0;
		slv.after_auth[i] = 0.0;
		slv.angle[i] = 0.0;

		slv_p.prev[i] = 0.0;
		slv_p.position[i] = 0.0;
		slv_p.force[i] = 0.0;
		slv_p.disp[i] = 0.0;
		slv_p.after_auth[i] = 0.0;
		slv_p.angle[i] = 0.0;

		simul.prev[i] = 0.0;
		simul.position[i] = 0.0;
		simul.force[i] = 0.0;
		simul.disp[i] = 0.0;
		simul.after_auth[i] = 0.0;

		order_pos[i] = 0;

		diff_e1[i] = 0.0;   prev_e1[i] = 0.0;	
		diff_e2[i] = 0.0;	prev_e2[i] = 0.0;
		
		n[i] = 0.0;
	}

	alpha[0] = 0.5;	alpha[1] = 0.5;	alpha[2] = 0.5;  // Dominance Value

	energy_user1[0] = 0.0;	energy_user1[1] = 0.0;	energy_user1[2] = 0.0;
	energy_user2[0] = 0.0;	energy_user2[1] = 0.0;	energy_user2[2] = 0.0;

	omni_cnt = 0;
	
	Phantom_Started = false;
	
}

/*******************************************************************************
Call Backs
*******************************************************************************/
HDCallbackCode HDCALLBACK DeviceCalibrate(void *pUserData)
{
	hdBeginFrame(MasterHandle);
	hdGetDoublev(HD_CURRENT_POSITION, init_position[0]);
	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, init_angles[0]);
	hdEndFrame(hdGetCurrentDevice());
	
	hdBeginFrame(SlaveHandle);
	hdGetDoublev(HD_CURRENT_POSITION, init_position[2]);
	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, init_angles[2]);
	hdEndFrame(hdGetCurrentDevice());
	
	return HD_CALLBACK_DONE;
}


//----------------------------------------------------------------------------

void omni_calibarate(void)
{
	if (!Phantom_Started) 
	{
		hdScheduleSynchronous(DeviceCalibrate, position, HD_MIN_SCHEDULER_PRIORITY);
	}
}

//----------------------------------------------------------------------------

void omni_start(void)
{
	if (!Phantom_Started)
	{
		Phantom_Started = true;
		hdScheduleAsynchronous(Start_teleoperation, position, HD_DEFAULT_SCHEDULER_PRIORITY);
	}
}

//----------------------------------------------------------------------------

void close (void)
{
	// For cleanup, unschedule our callbacks and stop the servo loop.
	hdStopScheduler();
	//hdUnschedule(teleoperation);
	hdDisableDevice(MasterHandle);
	hdDisableDevice(SlaveHandle);

	file.close(); //close data.txt file
	debug.close(); //close debug.txt file
}
//----------------------------------------------------------------------------

inline void master_omni()   
{
	
	hdBeginFrame(MasterHandle);

	hdGetDoublev(HD_CURRENT_POSITION, mst.position);

	hdBeginFrame(SlaveHandle);

	hdGetDoublev(HD_CURRENT_POSITION, slv.position);


	if (!switch_b)
	{
		
		slv.force[0] =  Omni_Saturation(K_simul*(mst.position[0] - slv.position[0]));
		cout<<"Slave force 1: " <<slv.force[0]<<endl;
		//Sleep(1);
		slv.force[1] =  Omni_Saturation(K_simul*(mst.position[1] - slv.position[1]));
		cout<<"Slave force 2: " <<slv.force[1]<<endl;
		//Sleep(1);
		slv.force[2] =  Omni_Saturation(K_simul*(mst.position[2] - slv.position[2]));
		cout<<"Slave force 3: " <<slv.force[2]<<endl;
		//Sleep(1);
		
		//file << mst.position[0] << "\t" << mst.position[1] << "\t" << mst.position[2];                     //write to data.txt file
	    file << "\t" << slv.position[0] << "\t" << slv.position[1] << "\t" << slv.position[2] << endl; 

		//sprintf(publish, "%0.5f %0.5f %0.5f", slv.position[0] , slv.position[1] , slv.position[2]);
		//zmq_send(publisher, publish, 50, 0);
		
		
	}

	hdMakeCurrentDevice(SlaveHandle);
	hdSetDoublev(HD_CURRENT_FORCE, slv.force);
	slv.force[0] *= -1.0;
	slv.force[1] *= -1.0;
	slv.force[2] *= -1.0;
	hdMakeCurrentDevice(MasterHandle);
	hdSetDoublev(HD_CURRENT_FORCE, slv.force);

	hdEndFrame(SlaveHandle);
	hdEndFrame(MasterHandle);


 }
//----------------------------------------------------------------------------
inline void fixed_dominance()
{
	

}

//----------------------------------------------------------------------------
double Omni_Saturation(double force)
{
	if (force<-LIMIT_FORCE)

		force = -LIMIT_FORCE;

	if (force>LIMIT_FORCE)

		force = LIMIT_FORCE;

	return force;
}

/*****************************************************************************/