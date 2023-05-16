const { SHA256 } = require('crypto-js')
const { encode } = require('ripple-binary-codec')

const { client } = require('./xrplClient')


async function fee_rpc(tx_blob) {
  const request = {
    command: 'fee',
    tx_blob,
  }
  return await client.request(request)
}

async function getTransactionFee(transaction) {
  const copyTx = JSON.parse(JSON.stringify(transaction))
  copyTx.Fee = `0`
  copyTx.SigningPubKey = ``
  
  const preparedTx = await client.autofill(copyTx)

  const tx_blob = encode(preparedTx)

  const { result } = await fee_rpc(tx_blob)

  return result.drops.base_fee
}

async function prepareTransactionV3(transaction) {
  transaction.NetworkID = 21338
  transaction.Fee = await getTransactionFee(transaction)
}

function deriveHookNamespace(hookNamespaceSeed) {
  return SHA256(hookNamespaceSeed).toString().toUpperCase()
}

module.exports = { deriveHookNamespace, prepareTransactionV3 }
