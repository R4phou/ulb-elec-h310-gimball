/*******************************************************************************
* File Name: Mux.c
* Version 1.80
*
*  Description:
*    This file contains all functions required for the analog multiplexer
*    AMux User Module.
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "Mux.h"

static uint8 Mux_lastChannel = Mux_NULL_CHANNEL;


/*******************************************************************************
* Function Name: Mux_Start
********************************************************************************
* Summary:
*  Disconnect all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Mux_Start(void) 
{
    uint8 chan;

    for(chan = 0u; chan < Mux_CHANNELS ; chan++)
    {
#if (Mux_MUXTYPE == Mux_MUX_SINGLE)
        Mux_Unset(chan);
#else
        Mux_CYAMUXSIDE_A_Unset(chan);
        Mux_CYAMUXSIDE_B_Unset(chan);
#endif
    }

    Mux_lastChannel = Mux_NULL_CHANNEL;
}


#if (!Mux_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_Select
********************************************************************************
* Summary:
*  This functions first disconnects all channels then connects the given
*  channel.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Mux_Select(uint8 channel) 
{
    Mux_DisconnectAll();        /* Disconnect all previous connections */
    Mux_Connect(channel);       /* Make the given selection */
    Mux_lastChannel = channel;  /* Update last channel */
}
#endif


/*******************************************************************************
* Function Name: Mux_FastSelect
********************************************************************************
* Summary:
*  This function first disconnects the last connection made with FastSelect or
*  Select, then connects the given channel. The FastSelect function is similar
*  to the Select function, except it is faster since it only disconnects the
*  last channel selected rather than all channels.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Mux_FastSelect(uint8 channel) 
{
    /* Disconnect the last valid channel */
    if( Mux_lastChannel != Mux_NULL_CHANNEL)
    {
        Mux_Disconnect(Mux_lastChannel);
    }

    /* Make the new channel connection */
#if (Mux_MUXTYPE == Mux_MUX_SINGLE)
    Mux_Set(channel);
#else
    Mux_CYAMUXSIDE_A_Set(channel);
    Mux_CYAMUXSIDE_B_Set(channel);
#endif


    Mux_lastChannel = channel;   /* Update last channel */
}


#if (Mux_MUXTYPE == Mux_MUX_DIFF)
#if (!Mux_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_Connect
********************************************************************************
* Summary:
*  This function connects the given channel without affecting other connections.
*
* Parameters:
*  channel:  The channel to connect to the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Mux_Connect(uint8 channel) 
{
    Mux_CYAMUXSIDE_A_Set(channel);
    Mux_CYAMUXSIDE_B_Set(channel);
}
#endif

/*******************************************************************************
* Function Name: Mux_Disconnect
********************************************************************************
* Summary:
*  This function disconnects the given channel from the common or output
*  terminal without affecting other connections.
*
* Parameters:
*  channel:  The channel to disconnect from the common terminal.
*
* Return:
*  void
*
*******************************************************************************/
void Mux_Disconnect(uint8 channel) 
{
    Mux_CYAMUXSIDE_A_Unset(channel);
    Mux_CYAMUXSIDE_B_Unset(channel);
}
#endif

#if (Mux_ATMOSTONE)
/*******************************************************************************
* Function Name: Mux_DisconnectAll
********************************************************************************
* Summary:
*  This function disconnects all channels.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void Mux_DisconnectAll(void) 
{
    if(Mux_lastChannel != Mux_NULL_CHANNEL) 
    {
        Mux_Disconnect(Mux_lastChannel);
        Mux_lastChannel = Mux_NULL_CHANNEL;
    }
}
#endif

/* [] END OF FILE */
