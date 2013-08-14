#include "iostream"
class SockClient {
  public:
    SockClient(const char *host, const char *service,
               const char *protocal);
    int read(void *buf, int len);
    int write(void *buf, int len);
  private:
    int s;
};
