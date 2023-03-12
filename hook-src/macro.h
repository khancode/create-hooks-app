/**
 * These are helper macros for writing hooks, all of them are optional as is including hookmacro.h at all
 */

#include <stdint.h>
#include "hookapi.h"
#include "sfcodes.h"

#ifndef HOOKMACROS_INCLUDED
#define HOOKMACROS_INCLUDED 1


#ifdef NDEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

#define TRACEVAR(v) if (DEBUG) trace_num((uint32_t)(#v), (uint32_t)(sizeof(#v) - 1), (int64_t)v);
#define TRACEHEX(v) if (DEBUG) trace((uint32_t)(#v), (uint32_t)(sizeof(#v) - 1), (uint32_t)(v), (uint32_t)(sizeof(v)), 1);
#define TRACEXFL(v) if (DEBUG) trace_float((uint32_t)(#v), (uint32_t)(sizeof(#v) - 1), (int64_t)v);
#define TRACESTR(v) if (DEBUG) trace((uint32_t)(#v), (uint32_t)(sizeof(#v) - 1), (uint32_t)(v), sizeof(v), 0);

// hook developers should use this guard macro, simply GUARD(<maximum iterations>)
#define GUARD(maxiter) _g((1ULL << 31U) + __LINE__, (maxiter)+1)
#define GUARDM(maxiter, n) _g(( (1ULL << 31U) + (__LINE__ << 16) + n), (maxiter)+1)

#define SBUF(str) (uint32_t)(str), sizeof(str)

#define REQUIRE(cond, str)\
{\
    if (!(cond))\
        rollback(SBUF(str), __LINE__);\
}

// make a report buffer as a c-string
// provide a name for a buffer to declare (buf)
// provide a static string
// provide an integer to print after the string
#define RBUF(buf, out_len, str, num)\
unsigned char buf[sizeof(str) + 21];\
int out_len = 0;\
{\
    int i = 0;\
    for (; GUARDM(sizeof(str),1),i < sizeof(str); ++i)\
        (buf)[i] = str[i];\
    if ((buf)[sizeof(str)-1] == 0) i--;\
    if ((num) < 0) (buf)[i++] = '-';\
    uint64_t unsigned_num = (uint64_t)( (num) < 0 ? (num) * -1 : (num) );\
    uint64_t j = 10000000000000000000ULL;\
    int start = 1;\
    for (; GUARDM(20,2), unsigned_num > 0 && j > 0; j /= 10)\
    {\
        unsigned char digit = ( unsigned_num / j ) % 10;\
        if (digit == 0 && start)\
            continue;\
        start = 0;\
        (buf)[i++] = '0' + digit;\
    }\
    (buf)[i] = '\0';\
    out_len = i;\
}

#define RBUF2(buff, out_len, str, num, str2, num2)\
unsigned char buff[sizeof(str) + sizeof(str2) + 42];\
int out_len = 0;\
{\
    unsigned char* __buf__ = buff;\
    int i = 0;\
    for (; GUARDM(sizeof(str),1),i < sizeof(str); ++i)\
        (__buf__)[i] = str[i];\
    if ((__buf__)[sizeof(str)-1] == 0) i--;\
    if ((num) < 0) (__buf__)[i++] = '-';\
    uint64_t unsigned_num = (uint64_t)( (num) < 0 ? (num) * -1 : (num) );\
    uint64_t j = 10000000000000000000ULL;\
    int start = 1;\
    for (; GUARDM(20,2), unsigned_num > 0 && j > 0; j /= 10)\
    {\
        unsigned char digit = ( unsigned_num / j ) % 10;\
        if (digit == 0 && start)\
            continue;\
        start = 0;\
        (__buf__)[i++] = '0' + digit;\
    }\
    __buf__ += i;\
    out_len += i;\
    i = 0;\
    for (; GUARDM(sizeof(str2),3),i < sizeof(str2); ++i)\
        (__buf__)[i] = str2[i];\
    if ((__buf__)[sizeof(str2)-1] == 0) i--;\
    if ((num2) < 0) (__buf__)[i++] = '-';\
    unsigned_num = (uint64_t)( (num2) < 0 ? (num2) * -1 : (num2) );\
    j = 10000000000000000000ULL;\
    start = 1;\
    for (; GUARDM(20,4), unsigned_num > 0 && j > 0; j /= 10)\
    {\
        unsigned char digit = ( unsigned_num / j ) % 10;\
        if (digit == 0 && start)\
            continue;\
        start = 0;\
        (__buf__)[i++] = '0' + digit;\
    }\
    (__buf__)[i] = '\0';\
    out_len += i;\
}

#define CLEARBUF(b)\
{\
    for (int x = 0; GUARD(sizeof(b)), x < sizeof(b); ++x)\
        b[x] = 0;\
}

// returns an in64_t, negative if error, non-negative if valid drops
#define AMOUNT_TO_DROPS(amount_buffer)\
     (((amount_buffer)[0] >> 7) ? -2 : (\
     ((((uint64_t)((amount_buffer)[0])) & 0xb00111111) << 56) +\
      (((uint64_t)((amount_buffer)[1])) << 48) +\
      (((uint64_t)((amount_buffer)[2])) << 40) +\
      (((uint64_t)((amount_buffer)[3])) << 32) +\
      (((uint64_t)((amount_buffer)[4])) << 24) +\
      (((uint64_t)((amount_buffer)[5])) << 16) +\
      (((uint64_t)((amount_buffer)[6])) <<  8) +\
      (((uint64_t)((amount_buffer)[7])))))

#define SUB_OFFSET(x) ((int32_t)(x >> 32))
#define SUB_LENGTH(x) ((int32_t)(x & 0xFFFFFFFFULL))

// when using this macro buf1len may be dynamic but buf2len must be static
// provide n >= 1 to indicate how many times the macro will be hit on the line of code
// e.g. if it is in a loop that loops 10 times n = 10

#define BUFFER_EQUAL_GUARD(output, buf1, buf1len, buf2, buf2len, n)\
{\
    output = ((buf1len) == (buf2len) ? 1 : 0);\
    for (int x = 0; GUARDM( (buf2len) * (n), 1 ), output && x < (buf2len);\
         ++x)\
        output = (buf1)[x] == (buf2)[x];\
}

#define BUFFER_SWAP(x,y)\
{\
    uint8_t* __z__ = x;\
    x = y;\
    y = __z__;\
}

#define ACCOUNT_COMPARE(compare_result, buf1, buf2)\
{\
    compare_result = 0;\
    for (int i = 0; GUARD(20), i < 20; ++i)\
    {\
        if (buf1[i] > buf2[i])\
        {\
            compare_result = 1;\
            break;\
        }\
        else if (buf1[i] < buf2[i])\
        {\
            compare_result = -1;\
            break;\
        }\
    }\
}

#define BUFFER_EQUAL_STR_GUARD(output, buf1, buf1len, str, n)\
    BUFFER_EQUAL_GUARD(output, buf1, buf1len, str, (sizeof(str)-1), n)

#define BUFFER_EQUAL_STR(output, buf1, buf1len, str)\
    BUFFER_EQUAL_GUARD(output, buf1, buf1len, str, (sizeof(str)-1), 1)

#define BUFFER_EQUAL(output, buf1, buf2, compare_len)\
    BUFFER_EQUAL_GUARD(output, buf1, compare_len, buf2, compare_len, 1)

#define UINT16_TO_BUF(buf_raw, i)\
{\
    unsigned char* __buf__ = (unsigned char*)buf_raw;\
    __buf__[0] = (((uint64_t)i) >> 8) & 0xFFUL;\
    __buf__[1] = (((uint64_t)i) >> 0) & 0xFFUL;\
}

#define UINT16_FROM_BUF(buf)\
    (((uint64_t)((buf)[0]) <<  8) +\
     ((uint64_t)((buf)[1]) <<  0))

#define UINT32_TO_BUF(buf_raw, i)\
{\
    unsigned char* __buf__ = (unsigned char*)buf_raw;\
    __buf__[0] = (((uint64_t)i) >> 24) & 0xFFUL;\
    __buf__[1] = (((uint64_t)i) >> 16) & 0xFFUL;\
    __buf__[2] = (((uint64_t)i) >>  8) & 0xFFUL;\
    __buf__[3] = (((uint64_t)i) >>  0) & 0xFFUL;\
}


#define UINT32_FROM_BUF(buf)\
    (((uint64_t)((buf)[0]) << 24) +\
     ((uint64_t)((buf)[1]) << 16) +\
     ((uint64_t)((buf)[2]) <<  8) +\
     ((uint64_t)((buf)[3]) <<  0))

#define UINT64_TO_BUF(buf_raw, i)\
{\
    unsigned char* __buf__ = (unsigned char*)buf_raw;\
    __buf__[0] = (((uint64_t)i) >> 56) & 0xFFUL;\
    __buf__[1] = (((uint64_t)i) >> 48) & 0xFFUL;\
    __buf__[2] = (((uint64_t)i) >> 40) & 0xFFUL;\
    __buf__[3] = (((uint64_t)i) >> 32) & 0xFFUL;\
    __buf__[4] = (((uint64_t)i) >> 24) & 0xFFUL;\
    __buf__[5] = (((uint64_t)i) >> 16) & 0xFFUL;\
    __buf__[6] = (((uint64_t)i) >>  8) & 0xFFUL;\
    __buf__[7] = (((uint64_t)i) >>  0) & 0xFFUL;\
}


#define UINT64_FROM_BUF(buf)\
    (((uint64_t)((buf)[0]) << 56) +\
     ((uint64_t)((buf)[1]) << 48) +\
     ((uint64_t)((buf)[2]) << 40) +\
     ((uint64_t)((buf)[3]) << 32) +\
     ((uint64_t)((buf)[4]) << 24) +\
     ((uint64_t)((buf)[5]) << 16) +\
     ((uint64_t)((buf)[6]) <<  8) +\
     ((uint64_t)((buf)[7]) <<  0))


#define INT64_FROM_BUF(buf)\
   ((((uint64_t)((buf)[0] & 0x7FU) << 56) +\
     ((uint64_t)((buf)[1]) << 48) +\
     ((uint64_t)((buf)[2]) << 40) +\
     ((uint64_t)((buf)[3]) << 32) +\
     ((uint64_t)((buf)[4]) << 24) +\
     ((uint64_t)((buf)[5]) << 16) +\
     ((uint64_t)((buf)[6]) <<  8) +\
     ((uint64_t)((buf)[7]) <<  0)) * (buf[0] & 0x80U ? -1 : 1))

#define INT64_TO_BUF(buf_raw, i)\
{\
    unsigned char* __buf__ = (unsigned char*)buf_raw;\
    __buf__[0] = (((uint64_t)i) >> 56) & 0x7FUL;\
    __buf__[1] = (((uint64_t)i) >> 48) & 0xFFUL;\
    __buf__[2] = (((uint64_t)i) >> 40) & 0xFFUL;\
    __buf__[3] = (((uint64_t)i) >> 32) & 0xFFUL;\
    __buf__[4] = (((uint64_t)i) >> 24) & 0xFFUL;\
    __buf__[5] = (((uint64_t)i) >> 16) & 0xFFUL;\
    __buf__[6] = (((uint64_t)i) >>  8) & 0xFFUL;\
    __buf__[7] = (((uint64_t)i) >>  0) & 0xFFUL;\
    if (i < 0) __buf__[0] |= 0x80U;\
}

#define ttPAYMENT 0
#define ttESCROW_CREATE 1
#define ttESCROW_FINISH 2
#define ttACCOUNT_SET 3
#define ttESCROW_CANCEL 4
#define ttREGULAR_KEY_SET 5
#define ttOFFER_CREATE 7
#define ttOFFER_CANCEL 8
#define ttTICKET_CREATE 10
#define ttSIGNER_LIST_SET 12
#define ttPAYCHAN_CREATE 13
#define ttPAYCHAN_FUND 14
#define ttPAYCHAN_CLAIM 15
#define ttCHECK_CREATE 16
#define ttCHECK_CASH 17
#define ttCHECK_CANCEL 18
#define ttDEPOSIT_PREAUTH 19
#define ttTRUST_SET 20
#define ttACCOUNT_DELETE 21
#define ttHOOK_SET 22
#define ttNFTOKEN_MINT 25
#define ttNFTOKEN_BURN 26
#define ttNFTOKEN_CREATE_OFFER 27
#define ttNFTOKEN_CANCEL_OFFER 28
#define ttNFTOKEN_ACCEPT_OFFER 29
#define ttURITOKEN_MINT 45
#define ttURITOKEN_BURN 46
#define ttURITOKEN_BUY 47
#define ttURITOKEN_CREATE_SELL_OFFER 48
#define ttURITOKEN_CANCEL_SELL_OFFER 49
#define ttCLAIM_REWARD 98
#define ttINVOKE 99
#define ttAMENDMENT 100
#define ttFEE 101
#define ttUNL_MODIFY 102
#define ttEMIT_FAILURE 103

#define tfCANONICAL 0x80000000UL

#define atACCOUNT 1U
#define atOWNER 2U
#define atDESTINATION 3U
#define atISSUER 4U
#define atAUTHORIZE 5U
#define atUNAUTHORIZE 6U
#define atTARGET 7U
#define atREGULARKEY 8U
#define atPSEUDOCALLBACK 9U

#define amAMOUNT 1U
#define amBALANCE 2U
#define amLIMITAMOUNT 3U
#define amTAKERPAYS 4U
#define amTAKERGETS 5U
#define amLOWLIMIT 6U
#define amHIGHLIMIT 7U
#define amFEE 8U
#define amSENDMAX 9U
#define amDELIVERMIN 10U
#define amMINIMUMOFFER 16U
#define amRIPPLEESCROW 17U
#define amDELIVEREDAMOUNT 18U

/**
 * RH NOTE -- PAY ATTENTION
 *
 * ALL 'ENCODE' MACROS INCREMENT BUF_OUT
 * THIS IS TO MAKE CHAINING EASY
 * BUF_OUT IS A SACRIFICIAL POINTER
 *
 * 'ENCODE' MACROS WITH CONSTANTS HAVE
 * ALIASING TO ASSIST YOU WITH ORDER
 * _TYPECODE_FIELDCODE_ENCODE_MACRO
 * TO PRODUCE A SERIALIZED OBJECT
 * IN CANONICAL FORMAT YOU MUST ORDER
 * FIRST BY TYPE CODE THEN BY FIELD CODE
 *
 * ALL 'PREPARE' MACROS PRESERVE POINTERS
 *
 **/


#define ENCODE_TL_SIZE 49
#define ENCODE_TL(buf_out, tlamt, amount_type)\
{\
        uint8_t __uat__ = amount_type; \
        buf_out[0] = 0x60U +(__uat__ & 0x0FU ); \
        for (int i = 1; GUARDM(48, 1), i < 49; ++i)\
            buf_out[i] = tlamt[i-1];\
        buf_out += ENCODE_TL_SIZE;\
}
#define _06_XX_ENCODE_TL(buf_out, drops, amount_type )\
    ENCODE_TL(buf_out, drops, amount_type );
#define ENCODE_TL_AMOUNT(buf_out, drops )\
    ENCODE_TL(buf_out, drops, amAMOUNT );
#define _06_01_ENCODE_TL_AMOUNT(buf_out, drops )\
    ENCODE_TL_AMOUNT(buf_out, drops );


// Encode drops to serialization format
// consumes 9 bytes
#define ENCODE_DROPS_SIZE 9
#define ENCODE_DROPS(buf_out, drops, amount_type ) \
    {\
        uint8_t __uat__ = amount_type; \
        uint64_t __udrops__ = drops; \
        buf_out[0] = 0x60U +(__uat__ & 0x0FU ); \
        buf_out[1] = 0b01000000 + (( __udrops__ >> 56 ) & 0b00111111 ); \
        buf_out[2] = (__udrops__ >> 48) & 0xFFU; \
        buf_out[3] = (__udrops__ >> 40) & 0xFFU; \
        buf_out[4] = (__udrops__ >> 32) & 0xFFU; \
        buf_out[5] = (__udrops__ >> 24) & 0xFFU; \
        buf_out[6] = (__udrops__ >> 16) & 0xFFU; \
        buf_out[7] = (__udrops__ >>  8) & 0xFFU; \
        buf_out[8] = (__udrops__ >>  0) & 0xFFU; \
        buf_out += ENCODE_DROPS_SIZE; \
    }

#define _06_XX_ENCODE_DROPS(buf_out, drops, amount_type )\
    ENCODE_DROPS(buf_out, drops, amount_type );

#define ENCODE_DROPS_AMOUNT(buf_out, drops )\
    ENCODE_DROPS(buf_out, drops, amAMOUNT );
#define _06_01_ENCODE_DROPS_AMOUNT(buf_out, drops )\
    ENCODE_DROPS_AMOUNT(buf_out, drops );

#define ENCODE_DROPS_FEE(buf_out, drops )\
    ENCODE_DROPS(buf_out, drops, amFEE );
#define _06_08_ENCODE_DROPS_FEE(buf_out, drops )\
    ENCODE_DROPS_FEE(buf_out, drops );

#define ENCODE_TT_SIZE 3
#define ENCODE_TT(buf_out, tt )\
    {\
        uint8_t __utt__ = tt;\
        buf_out[0] = 0x12U;\
        buf_out[1] =(__utt__ >> 8 ) & 0xFFU;\
        buf_out[2] =(__utt__ >> 0 ) & 0xFFU;\
        buf_out += ENCODE_TT_SIZE; \
    }
#define _01_02_ENCODE_TT(buf_out, tt)\
    ENCODE_TT(buf_out, tt);


#define ENCODE_ACCOUNT_SIZE 22
#define ENCODE_ACCOUNT(buf_out, account_id, account_type)\
    {\
        uint8_t __uat__ = account_type;\
        buf_out[0] = 0x80U + __uat__;\
        buf_out[1] = 0x14U;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(account_id +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(account_id +  8);\
        *(uint32_t*)(buf_out + 18) = *(uint32_t*)(account_id + 16);\
        buf_out += ENCODE_ACCOUNT_SIZE;\
    }
#define _08_XX_ENCODE_ACCOUNT(buf_out, account_id, account_type)\
    ENCODE_ACCOUNT(buf_out, account_id, account_type);

#define ENCODE_ACCOUNT_SRC_SIZE 22
#define ENCODE_ACCOUNT_SRC(buf_out, account_id)\
    ENCODE_ACCOUNT(buf_out, account_id, atACCOUNT);
#define _08_01_ENCODE_ACCOUNT_SRC(buf_out, account_id)\
    ENCODE_ACCOUNT_SRC(buf_out, account_id);

#define ENCODE_ACCOUNT_DST_SIZE 22
#define ENCODE_ACCOUNT_DST(buf_out, account_id)\
    ENCODE_ACCOUNT(buf_out, account_id, atDESTINATION);
#define _08_03_ENCODE_ACCOUNT_DST(buf_out, account_id)\
    ENCODE_ACCOUNT_DST(buf_out, account_id);

#define ENCODE_ACCOUNT_OWNER_SIZE 22
#define ENCODE_ACCOUNT_OWNER(buf_out, account_id) \
    ENCODE_ACCOUNT(buf_out, account_id, atOWNER);
#define _08_02_ENCODE_ACCOUNT_OWNER(buf_out, account_id) \
    ENCODE_ACCOUNT_OWNER(buf_out, account_id);

#define ENCODE_UINT32_COMMON_SIZE 5U
#define ENCODE_UINT32_COMMON(buf_out, i, field)\
    {\
        uint32_t __ui__ = i; \
        uint8_t __uf__ = field; \
        buf_out[0] = 0x20U +(__uf__ & 0x0FU); \
        buf_out[1] =(__ui__ >> 24 ) & 0xFFU; \
        buf_out[2] =(__ui__ >> 16 ) & 0xFFU; \
        buf_out[3] =(__ui__ >>  8 ) & 0xFFU; \
        buf_out[4] =(__ui__ >>  0 ) & 0xFFU; \
        buf_out += ENCODE_UINT32_COMMON_SIZE; \
    }
#define _02_XX_ENCODE_UINT32_COMMON(buf_out, i, field)\
    ENCODE_UINT32_COMMON(buf_out, i, field)\

#define ENCODE_UINT32_UNCOMMON_SIZE 6U
#define ENCODE_UINT32_UNCOMMON(buf_out, i, field)\
    {\
        uint32_t __ui__ = i; \
        uint8_t __uf__ = field; \
        buf_out[0] = 0x20U; \
        buf_out[1] = __uf__; \
        buf_out[2] =(__ui__ >> 24 ) & 0xFFU; \
        buf_out[3] =(__ui__ >> 16 ) & 0xFFU; \
        buf_out[4] =(__ui__ >>  8 ) & 0xFFU; \
        buf_out[5] =(__ui__ >>  0 ) & 0xFFU; \
        buf_out += ENCODE_UINT32_UNCOMMON_SIZE; \
    }
#define _02_XX_ENCODE_UINT32_UNCOMMON(buf_out, i, field)\
    ENCODE_UINT32_UNCOMMON(buf_out, i, field)\

#define ENCODE_LLS_SIZE 6U
#define ENCODE_LLS(buf_out, lls )\
    ENCODE_UINT32_UNCOMMON(buf_out, lls, 0x1B );
#define _02_27_ENCODE_LLS(buf_out, lls )\
    ENCODE_LLS(buf_out, lls );

#define ENCODE_FLS_SIZE 6U
#define ENCODE_FLS(buf_out, fls )\
    ENCODE_UINT32_UNCOMMON(buf_out, fls, 0x1A );
#define _02_26_ENCODE_FLS(buf_out, fls )\
    ENCODE_FLS(buf_out, fls );

#define ENCODE_TAG_SRC_SIZE 5
#define ENCODE_TAG_SRC(buf_out, tag )\
    ENCODE_UINT32_COMMON(buf_out, tag, 0x3U );
#define _02_03_ENCODE_TAG_SRC(buf_out, tag )\
    ENCODE_TAG_SRC(buf_out, tag );

#define ENCODE_TAG_DST_SIZE 5
#define ENCODE_TAG_DST(buf_out, tag )\
    ENCODE_UINT32_COMMON(buf_out, tag, 0xEU );
#define _02_14_ENCODE_TAG_DST(buf_out, tag )\
    ENCODE_TAG_DST(buf_out, tag );

#define ENCODE_SEQUENCE_SIZE 5
#define ENCODE_SEQUENCE(buf_out, sequence )\
    ENCODE_UINT32_COMMON(buf_out, sequence, 0x4U );
#define _02_04_ENCODE_SEQUENCE(buf_out, sequence )\
    ENCODE_SEQUENCE(buf_out, sequence );

#define ENCODE_FLAGS_SIZE 5
#define ENCODE_FLAGS(buf_out, tag )\
    ENCODE_UINT32_COMMON(buf_out, tag, 0x2U );
#define _02_02_ENCODE_FLAGS(buf_out, tag )\
    ENCODE_FLAGS(buf_out, tag );

#define ENCODE_SIGNING_PUBKEY_SIZE 35
#define ENCODE_SIGNING_PUBKEY(buf_out, pkey )\
    {\
        buf_out[0] = 0x73U;\
        buf_out[1] = 0x21U;\
        *(uint64_t*)(buf_out +  2) = *(uint64_t*)(pkey +  0);\
        *(uint64_t*)(buf_out + 10) = *(uint64_t*)(pkey +  8);\
        *(uint64_t*)(buf_out + 18) = *(uint64_t*)(pkey + 16);\
        *(uint64_t*)(buf_out + 26) = *(uint64_t*)(pkey + 24);\
        buf[34] = pkey[32];\
        buf_out += ENCODE_SIGNING_PUBKEY_SIZE;\
    }

#define _07_03_ENCODE_SIGNING_PUBKEY(buf_out, pkey )\
    ENCODE_SIGNING_PUBKEY(buf_out, pkey );

#define ENCODE_SIGNING_PUBKEY_NULL_SIZE 35
#define ENCODE_SIGNING_PUBKEY_NULL(buf_out )\
    {\
        buf_out[0] = 0x73U;\
        buf_out[1] = 0x21U;\
        *(uint64_t*)(buf_out+2) = 0;\
        *(uint64_t*)(buf_out+10) = 0;\
        *(uint64_t*)(buf_out+18) = 0;\
        *(uint64_t*)(buf_out+25) = 0;\
        buf_out += ENCODE_SIGNING_PUBKEY_NULL_SIZE;\
    }

#define _07_03_ENCODE_SIGNING_PUBKEY_NULL(buf_out )\
    ENCODE_SIGNING_PUBKEY_NULL(buf_out );


#ifdef HAS_CALLBACK
#define PREPARE_PAYMENT_SIMPLE_SIZE 270U
#else
#define PREPARE_PAYMENT_SIMPLE_SIZE 248U
#endif

#define PREPARE_PAYMENT_SIMPLE(buf_out_master, drops_amount_raw, to_address, dest_tag_raw, src_tag_raw)\
    {\
        uint8_t* __buf_out__ = buf_out_master;\
        uint8_t __acc__[20];\
        uint64_t __drops_amount__ = (drops_amount_raw);\
        uint32_t __dest_tag__ = (dest_tag_raw);\
        uint32_t __src_tag__ = (src_tag_raw);\
        uint32_t __cls__ = (uint32_t)ledger_seq();\
        hook_account(SBUF(__acc__));\
        _01_02_ENCODE_TT                   (__buf_out__, ttPAYMENT                      );      /* uint16  | size   3 */ \
        _02_02_ENCODE_FLAGS                (__buf_out__, tfCANONICAL                    );      /* uint32  | size   5 */ \
        _02_03_ENCODE_TAG_SRC              (__buf_out__, __src_tag__                    );      /* uint32  | size   5 */ \
        _02_04_ENCODE_SEQUENCE             (__buf_out__, 0                              );      /* uint32  | size   5 */ \
        _02_14_ENCODE_TAG_DST              (__buf_out__, __dest_tag__                   );      /* uint32  | size   5 */ \
        _02_26_ENCODE_FLS                  (__buf_out__, __cls__ + 1                    );      /* uint32  | size   6 */ \
        _02_27_ENCODE_LLS                  (__buf_out__, __cls__ + 5                    );      /* uint32  | size   6 */ \
        _06_01_ENCODE_DROPS_AMOUNT         (__buf_out__, __drops_amount__               );      /* amount  | size   9 */ \
        uint8_t* __fee_ptr__ = __buf_out__;\
        _06_08_ENCODE_DROPS_FEE            (__buf_out__, 0                              );      /* amount  | size   9 */ \
        _07_03_ENCODE_SIGNING_PUBKEY_NULL  (__buf_out__                                 );      /* pk      | size  35 */ \
        _08_01_ENCODE_ACCOUNT_SRC          (__buf_out__, __acc__                        );      /* account | size  22 */ \
        _08_03_ENCODE_ACCOUNT_DST          (__buf_out__, to_address                     );      /* account | size  22 */ \
        int64_t __buf_size__ = PREPARE_PAYMENT_SIMPLE_SIZE - (__buf_out__ - buf_out_master);                             \
        int64_t __edlen__ = etxn_details((uint32_t)__buf_out__, __buf_size__);                  /* emitdet | size 1?? */ \
        int64_t __fee__ = etxn_fee_base(buf_out_master, PREPARE_PAYMENT_SIMPLE_SIZE);                                    \ 
        _06_08_ENCODE_DROPS_FEE            (__fee_ptr__, __fee__                        );                               \
    }

#ifdef HAS_CALLBACK
#define PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE 309
#else
#define PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE 288
#endif
#define PREPARE_PAYMENT_SIMPLE_TRUSTLINE(buf_out_master, tlamt, to_address, dest_tag_raw, src_tag_raw)\
    {\
        uint8_t* __buf_out__ = buf_out_master;\
        uint8_t __acc__[20];\
        uint32_t __dest_tag__ = (dest_tag_raw);\
        uint32_t __src_tag__ = (src_tag_raw);\
        uint32_t __cls__ = (uint32_t)ledger_seq();\
        hook_account(SBUF(__acc__));\
        _01_02_ENCODE_TT                   (__buf_out__, ttPAYMENT                      );      /* uint16  | size   3 */ \
        _02_02_ENCODE_FLAGS                (__buf_out__, tfCANONICAL                    );      /* uint32  | size   5 */ \
        _02_03_ENCODE_TAG_SRC              (__buf_out__, __src_tag__                    );      /* uint32  | size   5 */ \
        _02_04_ENCODE_SEQUENCE             (__buf_out__, 0                              );      /* uint32  | size   5 */ \
        _02_14_ENCODE_TAG_DST              (__buf_out__, __dest_tag__                   );      /* uint32  | size   5 */ \
        _02_26_ENCODE_FLS                  (__buf_out__, __cls__ + 1                    );      /* uint32  | size   6 */ \
        _02_27_ENCODE_LLS                  (__buf_out__, __cls__ + 5                    );      /* uint32  | size   6 */ \
        _06_01_ENCODE_TL_AMOUNT            (__buf_out__, tlamt                          );      /* amount  | size  48 */ \
        uint8_t* __fee_ptr__ = __buf_out__;\
        _06_08_ENCODE_DROPS_FEE            (__buf_out__, 0                              );      /* amount  | size   9 */ \
        _07_03_ENCODE_SIGNING_PUBKEY_NULL  (__buf_out__                                 );      /* pk      | size  35 */ \
        _08_01_ENCODE_ACCOUNT_SRC          (__buf_out__, __acc__                        );      /* account | size  22 */ \
        _08_03_ENCODE_ACCOUNT_DST          (__buf_out__, to_address                     );      /* account | size  22 */ \
        int64_t __buf_size__ = PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE - (__buf_out__ - buf_out_master);                   \
        etxn_details((uint32_t)__buf_out__, __buf_size__);                                      /* emitdet | size 1?? */ \
        int64_t __fee__ = etxn_fee_base(buf_out_master, PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE);                          \ 
        _06_08_ENCODE_DROPS_FEE            (__fee_ptr__, __fee__                        );                               \
    }



#endif


