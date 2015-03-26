#include "common.h"
#include "upnp.h"
#include "upnptools.h"
#include "tvmanager_tvctl.h"

UpnpClient_Handle ctrlpt_handle = -1;
const char TvDeviceType[] = "urn:schemas-upnp-org:device:tvdevice:1";

/*********** other local function **********/
extern FILE *open_log_file();
extern void close_log_file(FILE *log);
extern void wrlog(const char *fmt, ...);
/*********** other local function *********/

static int tvcp_callback(Upnp_EventType Type, void *Event, void *Cookie)
{
	/*int errCode = 0;*/
	switch ( Type ) {
	/* SSDP Stuff */
	case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
	case UPNP_DISCOVERY_SEARCH_RESULT: {
		struct Upnp_Discovery *d_event = (struct Upnp_Discovery *)Event;
		IXML_Document *DescDoc = NULL;
		int ret;

		if (d_event->ErrCode != UPNP_E_SUCCESS) {
			printf("Error in Discovery Callback -- %d\n",
				d_event->ErrCode);
		}
		ret = UpnpDownloadXmlDoc(d_event->Location, &DescDoc);
		if (ret != UPNP_E_SUCCESS) {
			printf("Can't Download Doc\n");
		} else {
			printf("Parse the iXML doc\n");
			DOMString s = ixmlPrintDocument(DescDoc);
			if((s == NULL) || (s[0] == '\0'))
			{
				printf("Parse ixml doc error!\n");
				return -1;	
			}
			printf("Doc is:\n%s\n", s);
		}
		if (DescDoc) {
			ixmlDocument_free(DescDoc);
		}
		//打印设备列表
		break;
	}
	case UPNP_DISCOVERY_SEARCH_TIMEOUT:
		break;
	default:
		wrlog("Cannot support this type:%d\n", Type);
		return -1;
	}
	return 0;
}

void init_local_control(struct g_tvctl_ctl *tvc)
{
	tvc->log = open_log_file();
}

int init_tvctl(void)
{
	init_local_control(&g_tvctl);
	wrlog("UPNP_VERSION_STRING = %s\n"
				"UPNP_VERSION_MAJOR  = %d\n"
				"UPNP_VERSION_MINOR  = %d\n"	  
				"UPNP_VERSION_PATCH  = %d\n"	  
				"UPNP_VERSION        = %d\n",	
 		UPNP_VERSION_STRING, UPNP_VERSION_MAJOR,
		UPNP_VERSION_MINOR, UPNP_VERSION_PATCH,
		UPNP_VERSION);

	dump_flags(g_tvctl.log);
	//初始化UPNP
	int rc = UpnpInit (NULL, 0);
	if ( UPNP_E_SUCCESS == rc ) {
		const char* ip_address = UpnpGetServerIpAddress();
		unsigned short port    = UpnpGetServerPort();
		
		wrlog("UPnP Initialized OK ip=%s, port=%d\n", 
			(ip_address ? ip_address : "UNKNOWN"), port);
	} else {
		wrlog("Upnp init failed,error: %s\n", UpnpGetErrorMessage (rc));
		return -1;
	}
	//注册设备-控制点
	rc = UpnpRegisterClient(tvcp_callback, &ctrlpt_handle, &ctrlpt_handle);
	if (rc != UPNP_E_SUCCESS)
	{
		wrlog("Failed for register client!err=%s\n", UpnpGetErrorMessage(rc));
		return -1;
	}
	wrlog("Upnp Register Client success!\n");

	return 0;
}

/**
  *@brief	控制点发送广播消息以查询设备
  *@param	tv:输入参数，超时时间
  *@return 	-1: 失败  0：成功
  */
int tvctl_discover(int tv)
{
	int rc = UpnpSearchAsync(ctrlpt_handle, tv, TvDeviceType, NULL);
	if (rc != UPNP_E_SUCCESS)
	{
		wrlog("Failed for search devices!err=%s\n", UpnpGetErrorMessage(rc));
		return -1;
	}
	return 0;
}


int fini_tvctl(void)
{
	(void) UpnpFinish();
	close_log_file(g_tvctl.log);
	return 0;
}

#ifdef MAIN_TEST
int main (int argc, char* argv[])
{
	int rc;
	init_tvctl();

	for(;;) {
		tvctl_discover(5);
		sleep(atoi(argv[1]));
	}
	
	fini_tvctl();
	exit (EXIT_SUCCESS);
}
#endif




