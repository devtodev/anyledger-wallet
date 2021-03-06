#ifndef _WEB3_RPC_H_
#define _WEB3_RPC_H_
/* system includes */
/* local includes */
#include "helpers/uint256.h"
#include "eth/address.h"


#ifdef  __cplusplus
extern "C" {
#endif

int web3_eth_blockNumber(uint64_t *out);
int web3_eth_getBalance(const address_t *address, uint256_t *out);
int web3_eth_getTransactionCount(const address_t *address, uint256_t *out);
int web3_eth_sendRawTransaction(const uint8_t *data, size_t data_len);
#ifdef __cplusplus
}
#endif

#endif /* _WEB3_RPC_H_ */

