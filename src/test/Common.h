/*
 * Common.h
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_TEST_COMMON_H_
#define SRC_TEST_COMMON_H_
#include "ImPduBase.h"


#define PROMPT		"im-client> "
#define PROMPTION fprintf(stderr, "%s", PROMPT);

typedef void (*packet_callback_t)(CImPdu* pPdu);



#endif /* SRC_TEST_COMMON_H_ */
