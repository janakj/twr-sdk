#ifndef _BC_RTC_H
#define _BC_RTC_H

#include <stm32l0xx.h>
#include "bc_common.h"

//! @addtogroup bc_rtc bc_rtc
//! @brief Driver for real-time clock
//! @{

//! @brief Initialize real-time clock

extern int _bc_rtc_writable_semaphore;

void bc_rtc_init(void);

//! @brief RTC date and time structure
typedef struct
{
    uint8_t seconds;     //!< Seconds parameter, from 00 to 59
	uint16_t subseconds; //!< Subsecond downcounter. When it reaches zero, it's reload value is the same as @ref RTC_SYNC_PREDIV
	uint8_t minutes;     //!< Minutes parameter, from 00 to 59
	uint8_t hours;       //!< Hours parameter, 24Hour mode, 00 to 23
	uint8_t week_day;         //!< Day in a week, from 1 to 7
	uint8_t date;        //!< Date in a month, 1 to 31
	uint8_t month;       //!< Month in a year, 1 to 12
	uint16_t year;        //!< Year parameter, 2000 to 2099
	uint32_t timestamp;  //!< Seconds from 01.01.1970 00:00:00
} bc_rtc_t;

//! @brief Get date and time from RTC
//! @param[in] rtc Pointer to the RTC date and time structure

void bc_rtc_get_date_time(bc_rtc_t* rtc);

//! @brief Set gate and time to RTC
//! @param[in] rtc Pointer to the RTC date and time structure
//! @return true On success
//! @return false On failure

bool bc_rtc_set_date_time(bc_rtc_t* rtc);

//! @brief Convert RTC to timestamp
//! @param[in] rtc Pointer to the RTC date and time structure
//! @return unix timestamp

uint32_t bc_rtc_rtc_to_timestamp(bc_rtc_t *rtc);

//! @brief Enable RTC write protection
//
// This function supports nested invocations. If bc_rtc_enable_write has been
// called repeatedly, calling this function repeatedly will only lock the RTC
// again after all calls to bc_rtc_enable_write have been unrolled.

static inline void bc_rtc_disable_write()
{
	if (--_bc_rtc_writable_semaphore <= 0) {
		_bc_rtc_writable_semaphore = 0;
		RTC->WPR = 0xff;
	}
}

//! @brief Disable RTC write protection

static inline void bc_rtc_enable_write()
{
	++_bc_rtc_writable_semaphore;
	RTC->WPR = 0xca;
	RTC->WPR = 0x53;
}

//! @brief Enable or disable RTC initialization mode
//! @param[in] state Enable when true, disable when false

void bc_rtc_set_init(bool state);

//! @}

#endif // _BC_RTC_H
