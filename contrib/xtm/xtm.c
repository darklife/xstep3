/*

   XTM - Xstep Tabajara Monitorator
   Copyright (C) 2003 by Paulo Henrique Rodrigues Pinheiro

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Please read the COPYING and README file!!!

*/

#include "/usr/local/include/xstep.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

long
  mem_total,
  swap_total;
int
  espera;
char
  cpuidle[30],
  memavailreal[30],
  laload[30],
  swap[30],
  servidor[80],
  grupo[50];

long pega_snmp(char *servidor, char *grupo, char *variavel)
{
	char buffer[1000];
	struct snmp_session session, *ss;
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;

	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;

	int status;

	init_snmp("snmpapp");
	    
	snmp_sess_init( &session );
	session.peername=servidor;

	session.version = SNMP_VERSION_1;
	session.community=grupo;
	session.community_len = strlen(session.community);

	ss = snmp_open(&session);

	if (!ss) {
		return (EOF);
	}

	pdu = snmp_pdu_create(SNMP_MSG_GET);
 
	get_node(variavel,anOID,&anOID_len);

	snmp_add_null_var(pdu, anOID, anOID_len);
	status = snmp_synch_response(ss, pdu, &response);

	if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
		snprint_variable(
		  buffer,
		  1000,
		  response->variables->name,
		  response->variables->name_length,
		  response->variables
		);
		strcpy(buffer,strrchr(buffer,' '));
	}
	else {
		if (status == STAT_SUCCESS) {
			snmp_close(ss);
		}

		return (EOF);
	}

	if (response) {
		snmp_free_pdu(response);
	}

	snmp_close(ss);

	return ((long)(atof(buffer)*1000));
	return (atoi(buffer));
}

void trabalha(struct xtree *t)
{
	static time_t antes=30000;
	time_t agora;

	if(!animate) {
		return;
	}

	if(0==antes) {
		time(&antes);
	}

	time(&agora);

	if(difftime(agora,antes)>espera) {
		long new_cpuidle;
		double new_memavailreal,new_laload,new_swap;

		new_cpuidle=
		  100
		  -
		  (
		    pega_snmp(servidor,grupo,"ssCpuIdle.0")
		    /
		    1000
		  );

		new_memavailreal=
		  (
		    1.0
		    -
		    (
		      (double)
		      pega_snmp(servidor,grupo,"memAvailReal.0")
		      /
		      mem_total
		    )
		  )
		  *
		  100;

		new_swap=
		  (
		    1.0
		    -
		    (
		      (double)
		      pega_snmp(servidor,grupo,"memAvailSwap.0")
		      /
		      swap_total
		    )
		  )
		  *
		  100;

		new_laload=
		  (double)
		  pega_snmp(servidor,grupo,"laLoad.1")
		  /
		  1000;

		antes=agora;
		broadcast++;

		sprintf(
		  cpuidle      ,
		  "CPU=%ld%%",
		  new_cpuidle
		);

		sprintf(
		  memavailreal ,
		  "Memória=%5.2f%%",
		  new_memavailreal
		);

		sprintf(
		  swap,
		  "Swap=%5.2f%%",
		  new_swap
		);

		sprintf(
		  laload,
		  "LoadAverage=%5.2f",
		  new_laload
		);
	}
}

void xmain(int n,char *p[])
{
	struct xtree *win;

	strcpy(servidor,p[1]);
	strcpy(grupo   ,p[2]);
	espera=atoi(p[3]);

	mem_total=pega_snmp(servidor,grupo,"memTotalReal.0");
	swap_total=pega_snmp(servidor,grupo,"memTotalSwap.0");

	strcpy(cpuidle      ,"");
	strcpy(memavailreal ,"");
	strcpy(swap         ,"");
	strcpy(laload       ,"");

	win=window_create(0,0,750,35,"xtm - Xstep Tabajara Monitorator");
        win->broadcast=trabalha;

	label_create(8+0*130,8,120,20,servidor    ,white,up);
	label_create(8+1*130,8,120,20,cpuidle     ,black,center);
	label_create(8+2*130,8,120,20,memavailreal,black,center);
	label_create(8+3*130,8,120,20,swap        ,black,center);
	label_create(8+4*130,8,120,20,laload      ,black,center);
	
	button_create(8+5*130,8,72,20,"cai fora",window_close);

	animate=1;
}

