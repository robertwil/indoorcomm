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
#include "data_connection.h"
#include "bus_component/bus_handler.h"
#include "Odometrie.h"

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
	FILE *fp = NULL;
	/* Create a channel to the NET kernel. */
	if((skfd = iw_sockets_open()) < 0)
	{
		perror("socket");
		return -1;
	}

	if (strcmp(argv[1], "Vermessung") == 0) {
		//FIXME
		if ( (fp = fopen("test.txt", "w+")) == NULL){
			perror("open failed");
			printf("fail\n");
		}else{
			char *test = {"HALLO\n"};
			fwrite(test,sizeof(test),1,fp);
			fflush(fp);
			fprintf(fp,"%s\n","Test");
			fflush(fp);
			fprintf(fp,"%d,%d\n",1,2);
			fflush(fp);
		}
		int i;
		for (i = 0; i < 20; i++) {
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
			ListRow_t listRow;
			//listRow = malloc(sizeof(ListRow_t) * sizeof(retScanData[0].mac));
			int j;
			printf("Vor for\n");
			for (j = 0; j < sizeof(retScanData)/sizeof(retScanData[0]); j++) {
				//listRow.AdressAndPower[j].macadress[0] = retScanData[j].mac;
				strncpy(listRow.AdressAndPower[j].macadress,retScanData[j].mac,sizeof(retScanData[j].mac));
				listRow.AdressAndPower[j].signalpower = retScanData[j].SSID;
			}
			printf("Nach for\n");
			listRow.x = 0;
			listRow.y = 0;
			printf("Vor fputs\n");
			//fputs( createRow(listRow),fp);
			//fprintf(fp,"%d,%d\n",9,10);
			//fflush(fp);
			int h;
			for (i = 0; i < (sizeof(listRow.AdressAndPower)/
				sizeof(listRow.AdressAndPower[0])); i++) {
				fprintf(fp,"%s ",listRow.AdressAndPower[i].macadress);
				fprintf(fp,"%d,",listRow.AdressAndPower[i].signalpower);

			}
			fprintf(fp,"%d,%d\n",1,2);
			fflush(fp);
			//createRow(listRow,fp);
			//fputs("Test",fp);
			printf("Nach fputs\n");
			//fflush(fp);
			usleep(1000000);
		}
		/* Close the socket. */
		fclose(fp);
	} else if (strcmp(argv[1], "Navigation") == 0){
		fp = fopen("test.txt", "r");
		PositionList_t positionList = readALL(fp);
		int i;
		for (i = 0; i < 30; i++) {
			scanData_t retscanData[10];
			AdressAndPower_t adressPower[10];
			int j;
			for ( j = 0; j < 10; j++) {
				//adressPower[j].macadress = retscanData[j].mac;
				strncpy(adressPower[j].macadress,retscanData[j].mac,sizeof(retscanData[j].mac));
				adressPower[j].signalpower = retscanData[j].SSID;
			}
			Position_t position = findPositionOfSignal(adressPower, positionList);
			char xpos[sizeof(position.x)];
			memcpy(xpos, &position.x, sizeof(position.y));
			char ypos[sizeof(position.y)];
			memcpy(xpos, &position.y, sizeof(position.y));
			printf("Current Position:");
			printf(xpos);
			printf(", ");
			printf(ypos);
		}
	} else {
		printf("No such mode\n");
	}
	iw_sockets_close(skfd);
	return 0;
}


