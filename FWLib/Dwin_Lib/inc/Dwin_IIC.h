#ifndef	_Dwin_IIC_h_
#define	_Dwin_IIC_h_

extern	void i2cstart(void);
extern	void i2cstop(void);
extern	void mack(void);
extern	void mnak(void);
extern	void cack(void);
extern	void i2cbw(unsigned char dat);
extern	unsigned char i2cbr(void);


#endif

