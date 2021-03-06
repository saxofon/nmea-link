#ifndef __SER_BRIDGE_H__
#define __SER_BRIDGE_H__

#include <ip_addr.h>
#include <c_types.h>
#include <espconn.h>

#define MAX_TCP_CONN 4 
#define MAX_CONN (MAX_TCP_CONN+3) // 4 TCP, 1 UDP, 1 Serial, 1 web console
#define SER_BRIDGE_TIMEOUT 300 // 300 seconds = 5 minutes

// Send buffer size
#define MAX_TXBUFFER (2*1460)

typedef struct serbridgeConnData {
    struct espconn *conn;
    uint16         txbufferlen;   // length of data in txbuffer
    char           *txbuffer;     // buffer for the data to send
    char           *sentbuffer;   // buffer sent, awaiting callback to get freed
    uint32_t       txoverflow_at; // when the transmitter started to overflow
    bool           readytosend;   // true, if txbuffer can be sent by espconn_sent
    void (*send)(void*, const char*, uint16);
    void* recv_conn;
    void (*recv)(void*, const char*, uint16);
} serbridgeConnData;

// port1 is transparent&programming, second port is programming only
void ICACHE_FLASH_ATTR serbridgeInit(int port);
void ICACHE_FLASH_ATTR serbridgeInitPins(void);
void ICACHE_FLASH_ATTR serbridgeUartCb(char *buf, short len);
void ICACHE_FLASH_ATTR serbridgeReset();

// send to all clients
int ICACHE_FLASH_ATTR
mux_write(void* vcd, uint8_t* buf, size_t len);

int ICACHE_FLASH_ATTR
allocateConnectionRecord(
    espconn *send_conn, void (*send)(void*, const char*, uint16),
    void *recv_conn, void (*recv)(void*, const char*, uint16)
);

void* ICACHE_FLASH_ATTR new_mux_src();

#endif /* __SER_BRIDGE_H__ */
