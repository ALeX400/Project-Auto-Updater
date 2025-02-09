#if defined _reno_const_included
    #endinput
#endif

#define _reno_const_included

#if !defined VIP_MAX_CLASSES
const VIP_MAX_CLASSES = 3
#endif
// const INVALID_INDEX = -1

/**
 * Default string length as 32.
 */
const LEN_DEFAULT_STRING = 32

/**
 * Maximum buffer length of server mode.
 */
const LEN_SERVER_MODE = 10

/**
 * Maximum buffer length of chat prefix.
 */
const LEN_CHAT_PREFIX = 16

/**
 * Maximum buffer length of chat message.
 */
const LEN_CHAT_MESSAGE = 192

/**
 * Maximum buffer length of chat protection keyword.
 */
const LEN_PUBLIC_KEYWORD = 20

/**
 * Maximum buffer length of a regex pattern.
 */
const RENO_REGEX_PATTERN_LENGTH = 256