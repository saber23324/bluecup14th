#include "qep.h"


extern QEvent const QEP_reservedEvt_[100];

/**
 * @brief QFsm_init ��ʼ��FSM
 *
 * @param me
 * @param e
 */
void QFsm_init(QFsm *me, QEvent const *e)
{
    (*me->state)(me, e); /* ִ����˵ĳ�ʼת�� */
    (void)(*me->state)(me , &QEP_reservedEvt_[Q_ENTRY_SIG]);  /* ִ�н��붯�� */
}
