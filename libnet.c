#include "evfunclib.h"
#include "aesocket.h"
#include "event.h"
#include "channel.h"
#include "acceptor.h"
#include "libnet.h"


libnet_t g_libnet = {0};

void* evnet_createchannel(unsigned int ip, unsigned short port)
{
    return (void*)upstream_channel(ip, port);
}

void evnet_closechannel(void* c, int errcode)
{
    channel_close((channel_t*)c, errcode);
}

int evnet_channelbind(void* c, pfn_msg_handler handler, unsigned int timeouts, void *pUser)
{
    return channel_bind((channel_t*)c, handler, timeouts, pUser);
}

void* evnet_channeluser(void* c)
{
    return channel_user((channel_t*)c);
}

void evnet_channelsend(void* c, char *data, int size)
{
    channel_send((channel_t*)c, data, size);
}

unsigned int evnet_channelip(void* c)
{
    return channel_ip((channel_t*)c);
}

void* evnet_createacceptor(unsigned short port, pfn_msg_handler handler, void *pUsr)
{
    return acceptor_create(port, handler, pUsr);
}

void evnet_destroyacceptor(void* acceptor)
{
    acceptor_destroy((acceptor_t*)acceptor);
}

int evnet_acceptorstart(void* acceptor)
{
    return acceptor_start((acceptor_t*)acceptor);
}

void evnet_acceptorstop(void* acceptor)
{
    acceptor_stop((acceptor_t*)acceptor);
}

unsigned int evnet_hostbyname(char *name)
{
    return aehostbyname(name);
}

int evnet_init()
{
    aesocketinit();
    g_libnet.channelHead = NULL;
    g_libnet.evLoop = aeCreateEventLoop(MAX_CNN);
    if(!g_libnet.evLoop){
        evnet_uint();
        return (-1);
    }
    return 0;
}

void evnet_uint()
{
    if(g_libnet.evLoop){
        aeDeleteEventLoop(g_libnet.evLoop);
        g_libnet.evLoop = NULL;
    }
    aesocketuint();
}

void evnet_loop(unsigned int loops)
{
    if(NULL != g_libnet.evLoop){
        aeProcessEvents(g_libnet.evLoop);
    }    
    channel_crond(loops);
}
