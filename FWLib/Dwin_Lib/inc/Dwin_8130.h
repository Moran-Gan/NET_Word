#ifndef	_Dwin_8130_h_
#define	_Dwin_8130_h_

extern	void rdtime(void);
extern	void Time_Update(void);
extern	void init_rtc(void);
extern	void time_set_init_enter(void);
extern	void time_set_confirm(void);
extern	void	Write_time_set_confirm(uint8_t* time);
extern	void Time_Update_fault(void);
extern	uint8_t getWeekdayByYearday(uint16_t year, uint16_t month, uint16_t days);

#endif