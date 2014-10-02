/*
 *  netchat.h
 *  netchat
 *
 */
using namespace std;

static const unsigned int SERVER_PORT = 8101;
static const unsigned int CLIENT_CHAT_PORT = 8098;
static const unsigned int CLIENT_FILE_XFER_PORT = 8100;

static const char SERVER_GREETING[] = "NETCHAT. HELLO.";

static const int MAX_SIZE_HANDSHAKE = 255;

static const char BUDDY_INTERNAL_DELIMITER = '#';
static const char BUDDY_LIST_INTERNAL_DELIMITER = '$';

static const int SERVER_SLEEP = 2; // in seconds
static const int CLIENT_RETRY_DELAY = 2; // in seconds

static const int MAX_WAIT_FOR_ACK = 15; // Number of packets to examine for ACK before retrying 
static const int MAX_RETRIES = 2; // Maximum number of retries for sending message before giving up.

static const char OP_PREAMBLE = 'p';
static const char OP_MSG = 'm';
static const char OP_ACK = 'a';

static const unsigned int FILE_RW_CHUNK = 32768; // 2^15 bytes written or read from the buffer at a time
static const char PATH_SEPARATOR = '/';


