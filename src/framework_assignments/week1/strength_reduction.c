
/**
 * @file
 *
 * This file contains the code for the OptSubstraction traversal.
 * The traversal has the uid: SR
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/enum.h"
#include "ccngen/trav.h"
#include "global/globals.h"
#include "palm/str.h"

/**
 * @fn SRbinop
 */
node_st *SRbinop(node_st *node) {
  TRAVleft(node);
  TRAVright(node);

  if (BINOP_OP(node) != BO_mul) { // not a mult operation
    return node;
  }

  // Find the literal value of the 2, if none, we are done aswell.
  node_st *num_side = NULL, *expr_side = NULL;
  if (NODE_TYPE(BINOP_LEFT(node)) == NT_NUM) {
    num_side = BINOP_LEFT(node);
    expr_side = BINOP_RIGHT(node);
  } else if (NODE_TYPE(BINOP_RIGHT(node)) == NT_NUM) {
    num_side = BINOP_RIGHT(node);
    expr_side = BINOP_LEFT(node);
  } else {
    return node; // Both sides are variable
  }

  int val = NUM_VAL(num_side);

  if (val < 2 || val > global.max_factor) {
    return node;
  }

  node_st *result = CCNcopy(expr_side);
  for (int i = 1; i < val; i++) {
    result = ASTbinop(result, CCNcopy(expr_side), BO_add);
  }

  BINOP_LEFT(node) = NULL;
  BINOP_RIGHT(node) = NULL;
  CCNfree(node);

  return result;
}
