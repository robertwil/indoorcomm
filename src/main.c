/*
 * rpitestmain.c
 *
 *  Created on: 22.04.2017
 *      Author: robert
 */


#include <stdio.h>
#include <stdlib.h>
//#include "wireless.h"
#include "iwlib.h"
#include "wlanscan.h"

/******************************* MAIN ********************************/

/*------------------------------------------------------------------*/
/*
 * The main !
 */
int
main(int	argc,
     char **	argv)
{
  int skfd;			/* generic raw socket desc.	*/
  char *dev;			/* device name			*/
  char *cmd;			/* command			*/
  char **args;			/* Command arguments */
  int count;			/* Number of arguments */

  /* Create a channel to the NET kernel. */
  if((skfd = iw_sockets_open()) < 0)
    {
      perror("socket");
      return -1;
    }
//FIXME
  dev = "wlan0";
  cmd = "scan";
  args = argv +3 ;
  count = argc -3;

  scanData_t retScanData[20];
  /* do the actual work */
  if (dev){


    (print_scanning_info)(skfd, dev, args, count,retScanData);

  	int i;
  	printf("Found Networks: \n");
  	for(i =0; i<=10;i++){
  		printf("ESSID: %s\n",retScanData[i].essid);
  		printf("SCAN: %s,%d\n\n",retScanData[i].mac,retScanData[i].SSID);
  	}
  }
  else{
	  iw_enum_devices(skfd, print_scanning_info, args, count);
	  printf("ELSE!");
  }



  /* Close the socket. */
  iw_sockets_close(skfd);

  return 0;
}


