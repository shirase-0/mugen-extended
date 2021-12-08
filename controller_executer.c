#include "mu_headers.h"

MU_Controller_Executer *controller_executer_init()
{
	MU_Controller_Executer *exec = (MU_Controller_Executer*) malloc(sizeof(MU_Controller_Executer));
	init_func_table(exec);

	return exec;
}

void init_func_table(MU_Controller_Executer *exec)
{
	exec->p_func_table[0] = &null_controller;
    exec->p_func_table[1] = NULL;
    exec->p_func_table[2] = NULL;
    exec->p_func_table[3] = NULL;
    exec->p_func_table[4] = NULL;
    exec->p_func_table[5] = NULL;
    exec->p_func_table[6] = NULL;
    exec->p_func_table[7] = NULL;
    exec->p_func_table[8] = NULL;
    exec->p_func_table[9] = NULL;
    exec->p_func_table[10] = NULL;
    exec->p_func_table[11] = NULL;
    exec->p_func_table[12] = NULL;
    exec->p_func_table[13] = NULL;
    exec->p_func_table[14] = NULL;
    exec->p_func_table[15] = &change_state;
    exec->p_func_table[16] = NULL;
    exec->p_func_table[17] = NULL;
    exec->p_func_table[18] = NULL;
    exec->p_func_table[19] = NULL;
    exec->p_func_table[20] = NULL;
    exec->p_func_table[21] = NULL;
    exec->p_func_table[22] = NULL;
    exec->p_func_table[23] = NULL;
    exec->p_func_table[24] = NULL;
    exec->p_func_table[25] = NULL;
    exec->p_func_table[26] = NULL;
    exec->p_func_table[27] = NULL;
    exec->p_func_table[28] = NULL;
    exec->p_func_table[29] = NULL;
    exec->p_func_table[30] = NULL;
    exec->p_func_table[31] = NULL;
    exec->p_func_table[32] = NULL;
    exec->p_func_table[33] = NULL;
    exec->p_func_table[34] = NULL;
    exec->p_func_table[35] = NULL;
    exec->p_func_table[36] = NULL;
    exec->p_func_table[37] = NULL;
    exec->p_func_table[38] = NULL;
    exec->p_func_table[39] = NULL;
    exec->p_func_table[40] = NULL;
    exec->p_func_table[41] = NULL;
    exec->p_func_table[42] = NULL;
    exec->p_func_table[43] = NULL;
    exec->p_func_table[44] = NULL;
    exec->p_func_table[45] = NULL;
    exec->p_func_table[46] = NULL;
    exec->p_func_table[47] = NULL;
    exec->p_func_table[48] = NULL;
    exec->p_func_table[49] = NULL;
    exec->p_func_table[50] = NULL;
    exec->p_func_table[51] = NULL;
    exec->p_func_table[52] = NULL;
    exec->p_func_table[53] = NULL;
    exec->p_func_table[54] = NULL;
    exec->p_func_table[55] = NULL;
    exec->p_func_table[56] = NULL;
    exec->p_func_table[57] = NULL;
    exec->p_func_table[58] = NULL;
    exec->p_func_table[59] = NULL;
    exec->p_func_table[60] = NULL;
    exec->p_func_table[61] = NULL;
    exec->p_func_table[62] = NULL;
    exec->p_func_table[63] = NULL;
    exec->p_func_table[64] = NULL;
    exec->p_func_table[65] = NULL;
    exec->p_func_table[66] = NULL;
    exec->p_func_table[67] = NULL;
    exec->p_func_table[68] = NULL;
    exec->p_func_table[69] = NULL;
    exec->p_func_table[70] = NULL;
    exec->p_func_table[71] = NULL;
    exec->p_func_table[72] = NULL;
    exec->p_func_table[73] = NULL;
    exec->p_func_table[74] = NULL;
    exec->p_func_table[75] = NULL;
    exec->p_func_table[76] = NULL;
    exec->p_func_table[77] = NULL;
    exec->p_func_table[78] = NULL;
    exec->p_func_table[79] = NULL;
    exec->p_func_table[80] = &varset;
    exec->p_func_table[81] = NULL;
    exec->p_func_table[82] = NULL;
    exec->p_func_table[83] = &velset;
    exec->p_func_table[84] = NULL;
    exec->p_func_table[85] = NULL;
    exec->p_func_table[86] = NULL;
    exec->p_func_table[87] = NULL;
}

void mu_execute_controller(int n_controller)
{
	// I'd uncomment the debug part if I knew what TNULL is
	// #ifdef DEBUG
	//     PrintMessage("Execute controller %i",nController-TNULL);
	// #endif
	//  //   (this->*pFuncTable[nController-TNULL])();
}

void null_controller()
{

}

void change_state()
{
	/* int nStateToChange;
    //Get the Statenumber to wich it will be changed
    nStateToChange=(int)((CPlayer*)lpPlayer)->GetParamValue(PA_VALUE);
    ((CPlayer*)lpPlayer)->ChangeState(nStateToChange);*/
}

void change_anim()
{
	// TODO
}

void velset()
{
	/*    float x=((CPlayer*)lpPlayer)->GetParamValue(PA_XVALUE);
	    float y=((CPlayer*)lpPlayer)->GetParamValue(PA_YVALUE);
	#ifdef DEBUG
	    PrintMessage("Velset X=%f,Y=%f",x,y);
	#endif

	    if(x!=NOPARAM)
	       ((CPlayer*)lpPlayer)->PlRtInfo.xvel=x;
	       
	    if(y!=NOPARAM)
	        ((CPlayer*)lpPlayer)->PlRtInfo.yvel=y;
	*/
}

void varset()
{
	/*float fVarValue=0;
    int index=0;
    
    //check which var to set
    
    //VAR
    fVarValue=((CPlayer*)lpPlayer)->GetParamValue(PA_VAR);
    if(fVarValue!=NOPARAM)
    {
        index=(int)((CPlayer*)lpPlayer)->m_VM.m_Stack.Pop().Value;
       ((CPlayer*)lpPlayer)->SetIVar(index,fVarValue);
    
    }

    //FVAR
    fVarValue=((CPlayer*)lpPlayer)->GetParamValue(PA_FVAR);
    if(fVarValue!=NOPARAM)
    {
        index=(int)((CPlayer*)lpPlayer)->m_VM.m_Stack.Pop().Value;
       ((CPlayer*)lpPlayer)->SetFVar(index,fVarValue);
    
    }
    
    //SYSVAR
    fVarValue=((CPlayer*)lpPlayer)->GetParamValue(PA_SYSVAR);
    if(fVarValue!=NOPARAM)
    {
        index=(int)((CPlayer*)lpPlayer)->m_VM.m_Stack.Pop().Value;
       ((CPlayer*)lpPlayer)->SetSysVar(index,fVarValue);
    
    }
    
   //SYSFVAR
    fVarValue=((CPlayer*)lpPlayer)->GetParamValue(PA_SYSFVAR);
    if(fVarValue!=NOPARAM)
    {
        index=(int)((CPlayer*)lpPlayer)->m_VM.m_Stack.Pop().Value;
       ((CPlayer*)lpPlayer)->SetSysFVar(index,fVarValue);
    
    }

    
        */
}