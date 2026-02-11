#include <stdio.h>

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "ccngen/trav.h"
#include "ccngen/trav_data.h"
#include "palm/str.h"

void CAinit() { return; }
void CAfini() { return; }

/**
 * @fn CAprogram
 */
node_st *CAprogram(node_st *node) {
  TRAVchildren(node);

  struct data_ca *data = DATA_CA_GET();
  PROGRAM_OCCURENCECOUNTARITHMETICOPERATIONS(node) = data->occurenceCountAri;

  return node;
}

static bool isArithmeticOp(enum BinOpType op) {
  return op == BO_add || op == BO_sub || op == BO_mul || op == BO_div ||
         op == BO_mod;
}

/**
 * @fn CAbinop
 */
node_st *CAbinop(node_st *node) {
  struct data_ca *data = DATA_CA_GET();
  if (isArithmeticOp(BINOP_OP(node))) {
    data->occurenceCountAri++;
  }
  TRAVchildren(node);
  return node;
}
