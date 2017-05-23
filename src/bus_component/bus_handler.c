
/***********************************************************************/
/*************************** standard includes *************************/
/***********************************************************************/
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>




/***********************************************************************/
/************************** application includes ***********************/
/***********************************************************************/
#include "bus_handler.h"

//#include "../measurement_component/measurements.h"

/***********************************************************************/
/********************************* defines *****************************/
/***********************************************************************/

#define DEVICE "/dev/spidev1.0"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/*************************************************************************/
/********************************* enum **********************************/
/*************************************************************************/

/***********************************************************************/
/******************************** typedefs *****************************/
/***********************************************************************/

/***********************************************************************/
/****************************** private variables *********************/
/***********************************************************************/
/* instance of the singleton object */

static bus_handler_t *mSelf;
static int bus_active;






/***********************************************************************/
/*************************** private prototypes ************************/
/***********************************************************************/

/***********************************************************************/
/*************** Class implementation /public methodes *****************/
/***********************************************************************/
bool bus_init(bus_handler_t *object)
{




	mSelf = object;
	bus_active =0;

    int ret=0;
  //  int i;
    mSelf->mModeWrite = 1;
    mSelf->mModeWrite = SPI_MODE_1;
    mSelf->mModeRead = 0;
    mSelf->mModeRead = SPI_MODE_0;
    mSelf->mBits = 8;
    mSelf->mSpeed = 500000;
    mSelf->mDelay = 0;
    mSelf->mTransfer.tx_buf = (unsigned long)&(mSelf->mTx);
    mSelf->mTransfer.rx_buf = (unsigned long)&(mSelf->mRx);
    mSelf->mTransfer.len = 3;
    mSelf->mTransfer.delay_usecs = mSelf->mDelay;
    mSelf->mTransfer.speed_hz = mSelf->mSpeed;
    mSelf->mTransfer.bits_per_word = mSelf->mBits;
    mSelf->mTransfer.cs_change = false;
    mSelf->mBus = open(DEVICE, O_RDWR);


    if (mSelf->mBus < 0)
    {
        //fprintf(stderr, "Can't open spi bus to the spocs!\n");
    }

    if (mSelf->mBus >= 0)
    {

        // Set SPI mode
        ret = ioctl(mSelf->mBus, SPI_IOC_WR_MODE, &(mSelf->mModeWrite));
        if (ret == -1)
            //fprintf(stderr, "can't set spi mode\n");


        // Set the amount of bits transfered at once
        ret = ioctl(mSelf->mBus, SPI_IOC_WR_BITS_PER_WORD, &(mSelf->mBits));
        if (ret == -1)
            //fprintf(stderr, "can't set bits per word\n");


        // Set the speed of the spi
        ret = ioctl(mSelf->mBus, SPI_IOC_WR_MAX_SPEED_HZ, &(mSelf->mSpeed));
        if (ret == -1)
            //fprintf(stderr, "can't set max speed hz\n");


        if ((ret = ioctl(mSelf->mBus ,SPI_IOC_RD_MODE, &mSelf->mModeRead)) == -1) {
    		perror("READ Mode2 failed");
    	}

    	if ((ret = ioctl(mSelf->mBus, SPI_IOC_RD_BITS_PER_WORD, &mSelf->mBits)) == -1) {
    		perror("READ Bits2 failed");
    	}

    	if ((ret = ioctl(mSelf->mBus, SPI_IOC_RD_MAX_SPEED_HZ, &mSelf->mSpeed)) == -1) {
    		perror("READ Speed2 failed");
    	}

    }

	return true;
}

bool bus_shutdown ()
{

	int result = close(mSelf->mBus);
	printf ("close:%d", result);



	return true;
}
/*
void bus_send_action(uint16_t data,uint8_t tx0)
{
	bus_lock();

	//fprintf(stderr,"Started bus");
//    int ret=0;
    mSelf->mTx[0] = tx0;
    mSelf->mTx[1] = (uint8_t) (data >> 4);
    mSelf->mTx[2] = (uint8_t) ((data & 0x0F) << 4);

    mSelf->mTransfer.len =3;

    //fprintf(stderr,"mTx : %x %x %x",mSelf->mTx[0],mSelf->mTx[1],mSelf->mTx[2]);

    if (mSelf->mBus >= 0)
    {
    	gpio_setValue(DAC_CS,HIGH);
    	gpio_setValue(DAC_CS,LOW);
        ioctl(mSelf->mBus, SPI_IOC_MESSAGE(1), &(mSelf->mTransfer));
        gpio_setValue(DAC_CS,HIGH);
        bus_unlock();
//        if (ret < 1)
            //fprintf(stderr, "can't send spi message \n");
    }
//	usleep (1000);

}

uint16_t bus_readout_adc(adc_multiplex_t mode){
	bus_lock();
	int ret = 0;

	switch(mode){
	case MEPE_Vout:
		mSelf->mTx[0]= 0b10000011;
		break;
	case HIGHCURRENT:
		mSelf->mTx[0]= 0b11100011;
		break;
	case LOWCURRENT_ULC:
		mSelf->mTx[0]= 0b11110011;
		break;
	default:
		//fprintf(stderr,"ERROR\n");
		break;
	}

	mSelf->mTx[1]= 0b10000011; //See TI ADS1018 datasheet p.20
	mSelf->mTransfer.len =2;

	gpio_setValue(ADC_CS,HIGH);
	gpio_setValue(ADC_CS,LOW);
	if ((ret = ioctl(mSelf->mBus, SPI_IOC_MESSAGE(1), &mSelf->mTransfer)) == -1) {
		perror("Send Message2 failed");
	}
	gpio_setValue(ADC_CS,HIGH);
	bus_unlock();
//	//fprintf(stderr,"Rx : %x  Rx[1] : %x  Rx[2] : %x\n",mSelf->mRx[0], mSelf->mRx[1],mSelf->mRx[2]);
	return (((uint16_t)mSelf->mRx[0] << 4) | ((uint16_t)mSelf->mRx[1] >> 4));
}


*/

/***********************************************************************/
/*************** Class implementation / protected methodes *************/
/***********************************************************************/

/***********************************************************************/
/*************** Class implementation / private methodes ***************/
/***********************************************************************/


/***********************************************************************/
/************************ Tasks of the class ***************************/
/***********************************************************************/

