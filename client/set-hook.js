const fs = require('fs')
const path = require('path')

const addr = require('ripple-address-codec')
const { Wallet } = require('xrpl')
const { prepareTransactionV3 } = require('./util/transaction')

const config = require('../config.json')
const { client, connectClient, disconnectClient } = require('./util/xrpl-client')


const hookFilename = config.HOOK_C_FILENAME
const HOOK_ACCOUNT = Wallet.fromSeed(config.HOOK_ACCOUNT.seed)
const hsfOVERRIDE = 1
const wasm = fs.readFileSync(path.resolve(__dirname, `../build/${hookFilename}.wasm`))
const hookNamespace = addr.codec._sha256(hookFilename).toString(`hex`).toUpperCase()


async function run() {
  await connectClient()

  const tx = {
    TransactionType: `SetHook`,
    Account: HOOK_ACCOUNT.address,
    Hooks: [        
      {                        
        Hook: {                
          CreateCode: wasm.toString(`hex`).toUpperCase(),
          HookOn: `0000000000000000000000000000000000000000000000000000000000000000`,
          Flags: hsfOVERRIDE,
          HookNamespace: hookNamespace,
          HookApiVersion: 0,
        },
      },
    ],
  }

  await prepareTransactionV3(tx)

  console.log(`1. Transaction to submit (before autofill):`)
  console.log(JSON.stringify(tx, null, 2))
  console.log(`\n2. Submitting transaction...`)

  const result = await client.submitAndWait(
    tx, 
    { autofill: true, wallet: HOOK_ACCOUNT }
  )

  console.log(`\n3. Transaction result:`)
  console.log(result)

  const txResult = result.result.meta.TransactionResult
  if (txResult === 'tesSUCCESS') {
    console.log(`\n4. SetHook transaction succeeded!`)
  } else {
    console.log(`\n4. SetHook transaction failed with ${txResult}!`)
  }

  await disconnectClient()
}

run()
