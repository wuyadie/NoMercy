#pragma once
#include <cstdint>
#include <memory>

#define NOMERCY_CLIENT_MAX_MESSAGE_SIZE		0x100000
#define NOMERCY_MESSAGE_CRYPT_KEY			0x18

typedef struct _NM_CLIENT_MESSAGE_BODY
{
	uint8_t		pMagic[4];
	uint8_t		pContext[NOMERCY_CLIENT_MAX_MESSAGE_SIZE];
	uint32_t	ulContextSize;
	uint32_t	ulContextSum;
	uint32_t	ulContextPureSum;
	bool		bCrypted;
	uint8_t		pCryptKey;
	bool *		pbCompleted;
} SNMClientMsgBody, *PNMClientMsgBody;


typedef struct _NM_CLIENT_TEST_MESSAGE_CONTEXT
{
	char szMessage[255];
} SNMTestMsgCtx, *PNMTestMsgCtx;

typedef struct _NM_CLIENT_REDIRECT_POINTER_MESSAGE_CONTEXT
{
	void * lpFuncPointer;
} SNMRedirectPtrMsgCtx, *PNMRedirectPtrMsgCtx;

typedef struct _NM_CLIENT_SIGNAL_MESSAGE_CONTEXT
{
	int32_t iSignalCode;
} SNMSignalMsgCtx, *PNMSignalMsgCtx;

typedef struct _NM_CLIENT_GAME_CODE_CONTEXT
{
	int32_t iGameCode;
} SNMGameCodeMsgCtx, *PNMGameCodeMsgCtx;


enum ENMSignalIDs
{
	NM_SIG_ID_NULL			= 0,
	NM_SIG_SCR_PROT_ON,
	NM_SIG_SCR_PROT_OFF,
	NM_SIG_HB_V1_SETUP,
	NM_SIG_HB_V2_SETUP,
	NM_SIG_FRM_TICK,
	NM_SIG_PTR_RDT_OK
};

enum NM_MESSAGE_CODES
{
	NM_MSG_NULL				= 0,
	NM_GAME_LAUNCH_ERROR,
	NM_SERVICE_LAUNCH_ERROR,
	NM_DRIVER_LAUNCH_ERROR,
	NM_NETWORK_ERROR,
	NM_GAME_STARTED,
	NM_PROTECTION_COMPLETE
};

enum NM_DATA_CODES
{
	NM_DATA_NULL,

	// Test
	NM_DATA_SEND_TEST_MESSAGE = 13371337,
	NM_DATA_RECV_TEST_MESSAGE = NM_DATA_SEND_TEST_MESSAGE,

	// Pointer redirections
	NM_DATA_SEND_TRACEERROR = 1000,
	NM_DATA_SEND_NET_SEND_PROXY,
	NM_DATA_SEND_MAPPED_FILE_EXIST,
	NM_DATA_SEND_VID,
	NM_DATA_SEND_PHASE,
	NM_DATA_SEND_PLAYER_NAME,
	NM_DATA_SEND_MAPPED_FILE_HASH,
	NM_DATA_SEND_NET_SEND_SEQ,

	// Signals
	NM_DATA_SEND_SCREEN_PROTECTION_ON_SIGNAL = 2000,
	NM_DATA_SEND_SCREEN_PROTECTION_OFF_SIGNAL,

	NM_DATA_SEND_HEARTBEAT_V1_SETUP_SIGNAL,
	NM_DATA_SEND_HEARTBEAT_V2_SETUP_SIGNAL,

	NM_DATA_SEND_TICK_SIGNAL,

	NM_DATA_SEND_POINTER_REDIRECTION_COMPLETED_SIGNAL,

	// Utils
	NM_DATA_SEND_GAME_CODE = 3000,
	NM_DATA_SEND_PARENT_PROC_INFO,
	NM_DATA_SEND_USER_ID,
	NM_DATA_CHECK_FILE_SUM,
	NM_DATA_CHECK_API_HOOK,
	NM_DATA_PROTECT_FUNCTION,
	NM_DATA_PROTECT_MODULE,
	NM_DATA_SEED_PACKET_CRYPT_KEY,
	NM_DATA_ENCRYPT_CLIENT_PACKET,
	NM_DATA_DECRYPT_SERVER_PACKET,
	NM_DATA_PRINT_NOMERCY_MESSAGE,
	NM_DATA_SEND_GAME_NETWORK_INFORMATIONS,
	NM_DATA_ON_CLIENT_PACKET_RECEIVED, 
	NM_DATA_SEND_CLIENT_PACKET,

	// Recv
	NM_DATA_RECV_VERSION = 4000,	
	NM_DATA_RECV_IS_INITIALIZED,
	NM_DATA_RECV_SUSPECIOUS_EVENT
};

typedef void (__stdcall* TNMCallback)(NM_MESSAGE_CODES Code, const char* c_szMessage, const void* lpParam);

class CNM_SDK
{
	public:
		CNM_SDK() = default;
		~CNM_SDK() = default;

		// Initilization
		bool InitNoMercy(const char* c_szLicenseCode);

		// Finalization
		bool ReleaseNoMercy();

		// Notification
		bool OnGameTick();

		// Message handlers
		bool RegisterNoMercyMessageHandler(TNMCallback lpMessageHandler);
		bool SendDataToNoMercy(NM_DATA_CODES Code, const void* c_lpMessage);

		// Message wrappers
		bool SendTestMessage(bool bCrypted);

	protected:
		// Crypt nomercy message context
		bool SecureNoMercyMessage(const void* c_lpMessage, uint32_t ulSize, uint8_t pKey, bool bCrypt); // bCrypt == true; Encrypt, Otherwise Decrypt

		// Get checksum of nomercy message context
		bool GetChecksumNoMercyMessage(const void* c_lpMessage, uint32_t ulSize, uint32_t * pulChecksum);

		// Build nomercy message
		std::unique_ptr <SNMClientMsgBody> BuildMessage(const void* c_lpMessage, uint32_t ulSize, bool bCrypt = false, uint8_t pCryptKey = 0x0, std::uint32_t * pulErrorStep = nullptr);
};

