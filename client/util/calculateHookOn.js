// Taken & modified from https://github.com/XRPLF/xrpl-hooks-ide/blob/82c06cbb12bd320cd93e50c8fbaf09d6cd6f2410/utils/hookOnCalculator.ts

const tts = {
  ttPAYMENT: 0,
  ttESCROW_CREATE: 1,
  ttESCROW_FINISH: 2,
  ttACCOUNT_SET: 3,
  ttESCROW_CANCEL: 4,
  ttREGULAR_KEY_SET: 5,
  ttOFFER_CREATE: 7,
  ttOFFER_CANCEL: 8,
  ttTICKET_CREATE: 10,
  ttSIGNER_LIST_SET: 12,
  ttPAYCHAN_CREATE: 13,
  ttPAYCHAN_FUND: 14,
  ttPAYCHAN_CLAIM: 15,
  ttCHECK_CREATE: 16,
  ttCHECK_CASH: 17,
  ttCHECK_CANCEL: 18,
  ttDEPOSIT_PREAUTH: 19,
  ttTRUST_SET: 20,
  ttACCOUNT_DELETE: 21,
  ttHOOK_SET: 22,
  ttNFTOKEN_MINT: 25,
  ttNFTOKEN_BURN: 26,
  ttNFTOKEN_CREATE_OFFER: 27,
  ttNFTOKEN_CANCEL_OFFER: 28,
  ttNFTOKEN_ACCEPT_OFFER: 29,
  ttINVOKE: 99,
}

//Create a map that maps the transaction type to the bit position
const ttMap = new Map()
ttMap.set('Payment', tts.ttPAYMENT)
ttMap.set('EscrowCreate', tts.ttESCROW_CREATE)
ttMap.set('EscrowFinish', tts.ttESCROW_FINISH)
ttMap.set('AccountSet', tts.ttACCOUNT_SET)
ttMap.set('EscrowCancel', tts.ttESCROW_CANCEL)
ttMap.set('SetRegularKey', tts.ttREGULAR_KEY_SET)
ttMap.set('OfferCreate', tts.ttOFFER_CREATE)
ttMap.set('OfferCancel', tts.ttOFFER_CANCEL)
ttMap.set('TicketCreate', tts.ttTICKET_CREATE)
ttMap.set('SignerListSet', tts.ttSIGNER_LIST_SET)
ttMap.set('PaymentChannelCreate', tts.ttPAYCHAN_CREATE)
ttMap.set('PaymentChannelFund', tts.ttPAYCHAN_FUND)
ttMap.set('PaymentChannelClaim', tts.ttPAYCHAN_CLAIM)
ttMap.set('CheckCreate', tts.ttCHECK_CREATE)
ttMap.set('CheckCash', tts.ttCHECK_CASH)
ttMap.set('CheckCancel', tts.ttCHECK_CANCEL)
ttMap.set('DepositPreauth', tts.ttDEPOSIT_PREAUTH)
ttMap.set('TrustSet', tts.ttTRUST_SET)
ttMap.set('AccountDelete', tts.ttACCOUNT_DELETE)
ttMap.set('HookSet', tts.ttHOOK_SET)
ttMap.set('NFTokenMint', tts.ttNFTOKEN_MINT)
ttMap.set('NFTokenBurn', tts.ttNFTOKEN_BURN)
ttMap.set('NFTokenCreateOffer', tts.ttNFTOKEN_CREATE_OFFER)
ttMap.set('NFTokenCancelOffer', tts.ttNFTOKEN_CANCEL_OFFER)
ttMap.set('NFTokenAcceptOffer', tts.ttNFTOKEN_ACCEPT_OFFER)
ttMap.set('Invoke', tts.ttINVOKE)

const calculateHookOn = (arr) => {
  let s = '0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffbfffff'
  arr.forEach((transactionType) => {
    if (!ttMap.has(transactionType)) {
      throw new Error(`Invalid transaction type ${transactionType}`)
    }
    const ttNum = ttMap.get(transactionType)
    let v = BigInt(s)
    v ^= BigInt(1) << BigInt(ttNum)
    s = '0x' + v.toString(16)
  })
  s = s.replace('0x', '')
  s = s.padStart(64, '0')
  return s.toUpperCase()
}

module.exports = { calculateHookOn }
