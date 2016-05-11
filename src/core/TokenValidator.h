/*
 * TokenValidator.h
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_CORE_TOKENVALIDATOR_H_
#define SRC_CORE_TOKENVALIDATOR_H_

#include "Util.h"

/**
 * return 0 if generate token successful
 */
int genToken(unsigned int uid, time_t time_offset, char* md5_str_buf);

bool IsTokenValid(uint32_t user_id, const char* token);

#endif /* SRC_CORE_TOKENVALIDATOR_H_ */
