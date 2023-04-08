#ifndef __QEP_H
#define __QEP_H

#include "qevent.h"

typedef uint8_t QState;  /* ״̬����ֵ��״̬��״̬����������ֵ */
typedef QState(*QStateHandler) (void *me, QEvent const *e); /* ����һ������ָ�� */
typedef struct QFsmTag {
    QStateHandler state; /* ״̬��������ǰ�״̬��Ҳ���Ǿ����õ��� me->state */
} QFsm;  /* ƽ��״̬��FSM���ݽṹ */

#define QFsm_ctor(me_, initial_)  \
    ((me_)->state = (initial_))
void QFsm_init(QFsm *me, QEvent const *e);
void QFsm_dispatch(QFsm *me, QEvent const *e);

#define Q_RET_HANDLED ((QState)0)
#define Q_RET_IGNORED ((QState)1)
#define Q_RET_TRAN ((QState)2)

#define Q_HANDLED() (Q_RET_HANDLED)  /* �Ѵ����¼� */
#define Q_IGNORED() (Q_RET_IGNORED)  /* �����¼� */
#define Q_TRAN(target_)  \
    (((QFsm *)me)->state = (QStateHandler)(target_), Q_RET_TRAN)  /* ״̬ת�� */


/* ״̬����ȫ���ź�    һ�������еĶ��״̬������һ���ź�ö��  */
enum QReservedSignals {
    Q_ENTRY_SIG = 1,
    Q_EXIT_SIG,
    Q_INIT_SIG,
    Q_USER_SIG,  /* �������û��Զ����ź�  Q_USER_SIGռһ���յ�λ�� */
		
		
		MID_SIG,
		LW_SIG,
		BW_SIG,
		FW_SIG,
		RW_SIG,
		TR_SIG,
		TL_SIG,
		TM_SIG,
		Take_Site_SIG,
		Catch_SIG,
		Put1_SIG,
		Put2_SIG,
		Return_SIG,
		GetCode_SIG,
		Find_Bridge_SIG,
		Find_Put_SIG,
		Find_put2_SIG,
		Get_num_SIG,
		Put_bridge_SIG,
		Put_end_SIG,
		Put_end2_SIG,
};

#endif
