#ifndef __MOCA_RPC_CLIENT_H__
#define __MOCA_RPC_CLIENT_H__ 1
#include "RPC.h"

BEGIN_MOCA_RPC_NAMESPACE

class RPCClientImpl;
class RPCClient;

typedef void *RPCOpaqueData;
typedef void (*EventListener)(const RPCClient *channel, int32_t eventType, RPCOpaqueData eventData, RPCOpaqueData userData);

enum {
  EVENT_TYPE_CONNECTED = 1 << 0,
  EVENT_TYPE_ESTABLISHED = 1 << 1,
  EVENT_TYPE_DISCONNECTED = 1 << 2,
  EVENT_TYPE_REQUEST = 1 << 3,
  EVENT_TYPE_RESPONSE = 1 << 4,
  EVENT_TYPE_PAYLOAD = 1 << 5,
  EVENT_TYPE_ERROR = 1 << 6,
  EVENT_TYPE_LOG = 1 << 7,
  EVENT_TYPE_ALL = 0xFFFFFFFF,
};

struct ErrorEventData
{
  int32_t code;
  const char *message;
};

struct PacketEventData
{
  int64_t id;
  int32_t code;
  int32_t payloadSize;
  const KeyValuePairs<StringLite, StringLite> *headers;
};

typedef PacketEventData RequestEventData;
typedef PacketEventData ResponseEventData;

struct PayloadEventData
{
  int64_t id;
  int32_t size;
  int32_t reserved:31;
  int32_t commit:1;
  const char *payload;
};

class RPCClient
{
private:
  RPCClientImpl *impl_;
private:
  RPCClient();
  ~RPCClient();

  static void defaultLogger(LogLevel level, const char *func, const char *file, uint32_t line, const char *fmt, ...);

public:
  enum {
    CONNECT_FLAG_SSL = 1,
    CONNECT_FLAG_DEBUG = 2,
  };

  enum {
    LOOP_FLAG_ONE_SHOT = 1,
    LOOP_FLAG_NONBLOCK = 2,
  };

public:
  static LogLevel logLevel(LogLevel level);
  static RPCClient *create(int64_t timeout = 0x7FFFFFFFFFFFFFFFL, int64_t keepalive = 0x7FFFFFFFFFFFFFFFL, int32_t flags = 0, RPCLogger logger = defaultLogger);
  int32_t addListener(EventListener listener, RPCOpaqueData userData = NULL, int32_t eventMask = 0xFFFFFFFF);
  int32_t removeListener(EventListener listener);
  int32_t connect(const char *address);
  int32_t close();
  int32_t localAddress(StringLite *localAddress, uint16_t *port = NULL) const;
  int32_t remoteAddress(StringLite *remoteAddress, uint16_t *port = NULL) const;
  int32_t localId(StringLite *localId) const;
  int32_t remoteId(StringLite *remoteId) const;
  int32_t loop(int32_t flags = 0);
  int32_t breakLoop();
  int32_t keepalive();
  int32_t response(int64_t id, int32_t code, const KeyValuePairs<StringLite, StringLite> *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const;
  int32_t request(int64_t *id, int32_t code, const KeyValuePairs<StringLite, StringLite> *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const;
  int32_t request(int32_t code, const KeyValuePairs<StringLite, StringLite> *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const
  {
    int64_t discarded;
    return request(&discarded, code, headers, payload, payloadSize);
  }
  void addRef();
  bool release();
#ifdef MOCA_RPC_HAS_STL
  int32_t response(int64_t id, int32_t code, const KeyValueMap *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const;
  int32_t request(int64_t *id, int32_t code, const KeyValueMap *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const;
  int32_t request(int32_t code, const KeyValueMap *headers = NULL, const void *payload = NULL, size_t payloadSize = 0) const
  {
    int64_t discarded;
    return request(&discarded, code, headers, payload, payloadSize);
  }
#endif
};

END_MOCA_RPC_NAMESPACE

#endif