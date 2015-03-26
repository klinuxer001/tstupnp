#include <stdio.h>
#include <stdarg.h>
#include "tvmanager_tvctl.h"

#define LOGFILE "tvmanager_tvctl.log"

FILE *open_log_file()
{
	FILE *log = fopen(LOGFILE, "a");
	return (log == NULL)?stdout:log;
}

void close_log_file(FILE *log)
{
	fclose(log);
}

void wrlog(const char *fmt, ...)
{
	FILE *file = g_tvctl.log;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(file, fmt, ap);
	fflush(file);
	va_end(ap);
}

void dump_flags()
{
#if UPNP_HAVE_DEBUG
	wrlog("UPNP_HAVE_DEBUG \t= yes\n");
#else
	wrlog("UPNP_HAVE_DEBUG \t= no\n");
#endif
	
#if UPNP_HAVE_CLIENT
	wrlog("UPNP_HAVE_CLIENT\t= yes\n");
#else
	wrlog("UPNP_HAVE_CLIENT\t= no\n");
#endif
	
#if UPNP_HAVE_DEVICE
	wrlog("UPNP_HAVE_DEVICE\t= yes\n");
#else
	wrlog("UPNP_HAVE_DEVICE\t= no\n");
#endif
	
#if UPNP_HAVE_WEBSERVER
	wrlog("UPNP_HAVE_WEBSERVER\t= yes\n");
#else
	wrlog("UPNP_HAVE_WEBSERVER\t= no\n");
#endif

#if UPNP_HAVE_TOOLS
	wrlog("UPNP_HAVE_TOOLS \t= yes\n");
#else
	wrlog("UPNP_HAVE_TOOLS \t= no\n");
#endif
}

