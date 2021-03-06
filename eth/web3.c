/**
* @brief 
* @file web3.c
* @author J.H. 
* @date 2018-10-19
*/

/* system includes */
#include <string.h>
#include <stdio.h> // required for zephyr - includes vsnprintf() declaration
#include <stdarg.h>
#include <assert.h>

/* local includes */
#include "web3.h"
#include "helpers/hextobin.h"

const char *JSONRPC_HEADER = "{\"jsonrpc\":\"2.0\",\"id\":%d,\"method\":\"%s\",\"params\":[";
const char *JSONRPC_TERMINATOR = "]}";

static int __web3_printbuf(web3_ctx_t *web3, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    size_t size = vsnprintf((char*)web3->buf + web3->buf_used, web3->buf_size - web3->buf_used, fmt, ap);
    va_end(ap);
    if(size >= web3->buf_size) {
        return -1;
    }
    web3->buf_used += size;

    return 0;
}

#if 0
// will be used later
static int __web3_printhex(web3_ctx_t *web3, uint64_t val)
{
    return __web3_printbuf(web3, "\"%x\"", val);
}
#endif

// print binary data as a hex string to the buffer
static int __web3_printdata(web3_ctx_t *web3, const uint8_t *buf, size_t buf_size)
{
    if(buf_size >= (web3->buf_size - web3->buf_used)) { return -1; }
    int ret = __web3_printbuf(web3, "\"0x");

    if(ret < 0) { return ret; }
    ret = bintohex_nonull(
            buf, buf_size,
            (char*)web3->buf + web3->buf_used, web3->buf_size - web3->buf_used
            );
    if(ret < 0) { return -1; }
    web3->buf_used += ret;

    ret = __web3_printbuf(web3, "\"");
    if(ret < 0) { return ret; }

    return 0;
}

static int __web3_printaddr(web3_ctx_t *web3, const address_t *addr)
{
    int ret = __web3_printbuf(web3, "\"0x");
    if(ret < 0) { return ret; }
    int size = bintohex_nonull(
            (uint8_t*)addr, sizeof(address_t),
            (char*)web3->buf + web3->buf_used, web3->buf_size - web3->buf_used
    );
    if(size < 0) { return -1; }
    if(size < 40) { return -1; } // length of an ascii ethereum address
    web3->buf_used += size;

    ret = __web3_printbuf(web3, "\"");
    if(ret < 0) { return ret; }

    return 0;
}

static void __web3_resetbuf(web3_ctx_t *ctx)
{
    assert(ctx != NULL);
    assert(ctx->buf != NULL);
    assert(ctx->buf_size > 0);
    memset(ctx->buf, 0, ctx->buf_size);
    ctx->buf_used = 0;
}

void web3_init(web3_ctx_t *web3, uint8_t *buf, size_t buf_size)
{
    web3->buf = buf;
    web3->buf_size = buf_size;
    web3->req_id = 0;
    __web3_resetbuf(web3);
}

#define WEB3_PREAMBLE()\
    __web3_resetbuf(web3);\
    if(__web3_printbuf(web3, JSONRPC_HEADER, web3->req_id, (__func__)) < 0) {\
        return -1;\
    }

#define WEB3_TERMINATOR()\
    if(__web3_printbuf(web3, JSONRPC_TERMINATOR) < 0) {\
        return -1;\
    }

//
// returns: '{"jsonrpc":"2.0","method":"eth_getTransactionCount","params":["@addr","latest"],"id":1}'
//
int eth_getTransactionCount(web3_ctx_t *web3, const address_t *addr)
{
    WEB3_PREAMBLE()
    if(__web3_printaddr(web3, addr) < 0) {
        return -1;
    }
    if(__web3_printbuf(web3, ",\"latest\"") < 0) {
        return -1;
    }

    WEB3_TERMINATOR()

    return 0;
}

//
// returns: '{"jsonrpc":"2.0","method":"eth_blockNumber","params":[],"id":1}'
//
int eth_blockNumber(web3_ctx_t *web3)
{
    WEB3_PREAMBLE()
    WEB3_TERMINATOR()

    return 0;
}

//
// returns: '{"jsonrpc":"2.0","method":"eth_blockNumber","params":[@data],"id":1}'
//
int eth_sendRawTransaction(web3_ctx_t *web3, const uint8_t *data, size_t data_len)
{
    WEB3_PREAMBLE()

    if(__web3_printdata(web3, data, data_len) < 0) {
        return -1;
    }

    WEB3_TERMINATOR()
    return 0;
}

int eth_getBalance(web3_ctx_t *web3, const address_t *addr)
{
    WEB3_PREAMBLE()
    if(__web3_printaddr(web3, addr) < 0) {
        return -1;
    }
    if(__web3_printbuf(web3, ",\"latest\"") < 0) {
        return -1;
    }

    WEB3_TERMINATOR()

    return 0;
}
