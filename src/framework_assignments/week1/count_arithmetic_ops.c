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
  PROGRAM_COUNTADD(node) = data->countAdd;
  PROGRAM_COUNTSUB(node) = data->countSub;
  PROGRAM_COUNTMUL(node) = data->countMul;
  PROGRAM_COUNTDIV(node) = data->countDiv;
  PROGRAM_COUNTMOD(node) = data->countMod;
  PROGRAM_OCCURENCECOUNTARITHMETICOPERATIONS(node) =
      data->countAdd + data->countSub + data->countMul + data->countDiv +
      data->countMod;

  return node;
}

/**
 * @fn CAbinop
 */
node_st *CAbinop(node_st *node) {
  struct data_ca *data = DATA_CA_GET();
  switch (BINOP_OP(node)) {
  case BO_add:
    data->countAdd++;
    break;
  case BO_sub:
    data->countSub++;
    break;
  case BO_mul:
    data->countMul++;
    break;
  case BO_div:
    data->countDiv++;
    break;
  case BO_mod:
    data->countMod++;
    break;
  default:
    break;
  }
  TRAVchildren(node);
  return node;
}
