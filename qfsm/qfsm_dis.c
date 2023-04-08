#include "qep.h"


extern QEvent const QEP_reservedEvt_[100];

/**
 * @brief QFsm_dispatch FSM״̬����ǲ����
 *
 * @param me
 * @param e
 */
void QFsm_dispatch(QFsm *me, QEvent const *e)
{
    QStateHandler s = me->state;  /* ���浱ǰ�ĺ���ָ�� */
    QState r = (*s)(me, e); /* ���õ�ǰ״̬����������ȡ�¼� */
    if(r == Q_RET_TRAN) {  /* �ж��Ƿ���״̬ת���¼� */
        (void)(*s)(me, &QEP_reservedEvt_[Q_EXIT_SIG]); /* ִ�е�ǰ״̬�˳����� */
        (void)(*me->state)(me, &QEP_reservedEvt_[Q_ENTRY_SIG]);/* ִ����״̬���붯�� */
    }
}
