#include "pch.h"

#include "Common/CWinsockError.h"

UwpEngine::CWinsockError::CWinsockError()
{
	m_errors = new CLinkList();

	CWinsockErrorItem* errorItem = new CWinsockErrorItem(6, "WSA_INVALID_HANDLE", "Specified event object handle is invalid.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(87, "WSA_INVALID_PARAMETER", "One or more parameters are invalid.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(995, "WSA_OPERATION_ABORTED", "Overlapped operation aborted.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(996, "WSA_IO_INCOMPLETE", "Overlapped I/O event object not in signaled state.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(997, "WSA_IO_PENDING", "Overlapped operations will complete later.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10004, "WSAEINTR", "Interrupted function call.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10009, "WSAEBADF", "File handle is not valid.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10013, "WSAEACCES", "Permission denied.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10014, "WSAEFAULT", "Bad address.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10022, "WSAEINVA", "Invalid argument.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10024, "WSAEMFILE", "Too many open files.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10035, "WSAEWOULDBLOCK", "Resource temporarily unavailable.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10036, "WSAEINPROGRESS", "Operation now in progress.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10037, "WSAEALREADY", "Operation already in progress.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10038, "WSAENOTSOCK", "Socket operation on nonsocket.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10039, "WSAEDESTADDRREQ", "Destination address required.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10040, "WSAEMSGSIZE", "Message too long.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10041, "WSAEPROTOTYPE", "Protocol wrong type for socket.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10042, "WSAENOPROTOOPT", "Bad protocol option.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10043, "WSAEPROTONOSUPPORT", "Protocol not supported.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10044, "WSAESOCKTNOSUPPORT", "Socket type not supported.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10045, "WSAEOPNOTSUPP", "Operation not supported.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10046, "WSAEPFNOSUPPORT", "Protocol family not supported.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10047, "WSAEAFNOSUPPORT", "Address family not supported by protocol family.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10048, "WSAEADDRINUSE", "Address already in use.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10049, "WSAEADDRNOTAVAI", "Cannot assign requested address.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10050, "WSAENETDOWN", "Network is down.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10051, "WSAENETUNREACH", "Network is unreachable.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10052, "WSAENETRESET", "Network dropped connection on reset.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10053, "WSAECONNABORTED", "Software caused connection abort.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10054, "WSAECONNRESET", "Connection reset by peer.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10055, "WSAENOBUFS", "No buffer space available.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10056, "WSAEISCONN", "Socket is already connected.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10057, "WSAENOTCONN", "Socket is not connected.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10058, "WSAESHUTDOWN", "Cannot send after socket shutdown.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10059, "WSAETOOMANYREFS", "Too many references.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10060, "WSAETIMEDOUT", "Connection timed out.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10061, "WSAECONNREFUSED", "Connection refused.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10062, "WSAELOOP", "Cannot translate name.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10063, "WSAENAMETOOLONG", "Name too long.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10064, "WSAEHOSTDOWN", "Host is down.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10065, "WSAEHOSTUNREACH", "No route to host.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10066, "WSAENOTEMPTY", "Directory not empty.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10067, "WSAEPROCLIM", "Too many processes.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10068, "WSAEUSERS", "User quota exceeded.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10069, "WSAEDQUOT", "Disk quota exceeded.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10070, "WSAESTALE", "Stale file handle reference.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10071, "WSAEREMOTE", "Item is remote.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10091, "WSASYSNOTREADY", "Network subsystem is unavailable.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10092, "WSAVERNOTSUPPORTED", "Winsock.dll version out of range.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10093, "WSANOTINITIALISED", "Successful WSAStartup not yet performed.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10101, "WSAEDISCON", "Graceful shutdown in progress.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10102, "WSAENOMORE", "No more results.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10103, "WSAECANCELLED", "Call has been canceled.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10104, "WSAEINVALIDPROCTABLE", "Procedure call table is invalid.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10105, "WSAEINVALIDPROVIDER", "Service provider is invalid.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10106, "WSAEPROVIDERFAILEDINIT", "Service provider failed to initialize.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10107, "WSASYSCALLFAILURE", "System call failure.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10108, "WSASERVICE_NOT_FOUND", "Service not found.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10109, "WSATYPE_NOT_FOUND", "Class type not found.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10110, "WSA_E_NO_MORE", "No more results.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10111, "WSA_E_CANCELLED", "Call was canceled.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(10112, "WSAEREFUSED", "Database query was refused.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11001, "WSAHOST_NOT_FOUND", "Host not found.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11002, "WSATRY_AGAIN", "Nonauthoritative host not found.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11003, "WSANO_RECOVERY", "This is a nonrecoverable error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11004, "WSANO_DATA", "Valid name, no data record of requested type.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11005, "WSA_QOS_RECEIVERS", "QoS receivers.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11006, "WSA_QOS_SENDERS", "QoS senders.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11007, "WSA_QOS_NO_SENDERS", "No QoS senders.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11008, "WSA_QOS_NO_RECEIVERS", "QoS no receivers.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11009, "WSA_QOS_REQUEST_CONFIRMED", "QoS request confirmed.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11010, "WSA_QOS_ADMISSION_FAILURE", "QoS admission error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11011, "WSA_QOS_POLICY_FAILURE", "QoS policy failure.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11012, "WSA_QOS_BAD_STYLE", "QoS bad style.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11013, "WSA_QOS_BAD_OBJECT", "QoS bad object.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11014, "WSA_QOS_TRAFFIC_CTRL_ERROR", "QoS traffic control error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11015, "WSA_QOS_GENERIC_ERROR", "QoS generic error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11016, "WSA_QOS_ESERVICETYPE", "QoS service type error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11017, "WSA_QOS_EFLOWSPEC", "QoS flowspec error.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11018, "WSA_QOS_EPROVSPECBUF", "Invalid QoS provider buffer.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11019, "WSA_QOS_EFILTERSTYLE", "Invalid QoS filter style.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11020, "WSA_QOS_EFILTERTYPE", "Invalid QoS filter type.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11021, "WSA_QOS_EFILTERCOUNT", "Incorrect QoS filter count.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11022, "WSA_QOS_EOBJLENGTH", "Invalid QoS object length.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11023, "WSA_QOS_EFLOWCOUNT", "Incorrect QoS flow count.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11024, "WSA_QOS_EUNKOWNPSOBJ", "Unrecognized QoS object.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11025, "WSA_QOS_EPOLICYOBJ", "Invalid QoS policy object.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11026, "WSA_QOS_EFLOWDESC", "Invalid QoS flow descriptor.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11027, "WSA_QOS_EPSFLOWSPEC", "Invalid QoS provider - specific flowspec.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11028, "WSA_QOS_EPSFILTERSPEC", "Invalid QoS provider - specific filterspec.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11029, "WSA_QOS_ESDMODEOBJ", "Invalid QoS shape discard mode object.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11030, "WSA_QOS_ESHAPERATEOBJ", "Invalid QoS shaping rate object.");
	AddItem(errorItem);

	errorItem = new CWinsockErrorItem(11031, "WSA_QOS_RESERVED_PETYPE", "Reserved policy QoS element type.");
	AddItem(errorItem);
}

UwpEngine::CWinsockError::~CWinsockError()
{
}

UwpEngine::CWinsockErrorItem* UwpEngine::CWinsockError::GetError(UINT number)
{
	CLinkListNode* error = m_errors->Search(number);

	if (error)
	{
		return (CWinsockErrorItem*)error->m_element;
	}

	return nullptr;
}

void UwpEngine::CWinsockError::AddItem(CWinsockErrorItem* errorItem)
{
	m_errors->Add(errorItem, errorItem->m_nbr);
}