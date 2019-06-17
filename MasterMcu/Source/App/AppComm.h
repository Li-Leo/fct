#ifndef APPCOMM_H
#define APPCOMM_H
#include "common.h"
#include "CtlSpiComm.h"
extern TS_SPI_COMMAND gg_last_cmd_return;
void AppCommHandleCmdReturn(TS_SPI_COMMAND *ret_cmd);
#endif
