#ifndef __QFSM_USER_H
#define __QFSM_USER_H

#include "qep.h"
#include "qevent.h"
#include "main.h"

typedef struct CarTag {
    uint8_t timeout; /* number of seconds till explosion */
    uint8_t code;    /* Ŀǰ��������� */
    uint8_t defuse;  /* ������Ҫ������ */

} Car;


#endif
