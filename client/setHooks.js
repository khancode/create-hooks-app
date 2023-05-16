const fs = require('fs')
const path = require('path')

const { Wallet } = require('xrpl')
const { deriveHookNamespace, prepareTransactionV3 } = require('./util/transaction')

const config = require('../config.json')
const { client, connectClient, disconnectClient } = require('./util/xrplClient')
const { calculateHookOn } = require('./util/calculateHookOn')


const hsfOVERRIDE = 1

function createHooksPayload(config) {
  const result = []

  const { HOOKS, HOOK_NAMESPACE_SEED } = config
  const HookNamespace = deriveHookNamespace(HOOK_NAMESPACE_SEED)
  for (const hook of HOOKS) {
    const { HOOK_C_FILENAME, HookOn } = hook
    const wasm = fs.readFileSync(
      path.resolve(__dirname, `../build/${HOOK_C_FILENAME}.wasm`)
    )

    result.push({
      Hook: {
        CreateCode: wasm.toString(`hex`).toUpperCase(),
        HookOn: calculateHookOn(HookOn),
        Flags: hsfOVERRIDE,
        HookNamespace,
        HookApiVersion: 0,
      },
    })
  }

  return result
}

async function run() {
  await connectClient()

  const HOOK_ACCOUNT = Wallet.fromSeed(config.HOOK_ACCOUNT.seed)
  const Hooks = createHooksPayload(config)

  const tx = {
    TransactionType: `SetHook`,
    Account: HOOK_ACCOUNT.address,
    Hooks,
  }

  await prepareTransactionV3(tx)

  console.log(`1. Transaction to submit (before autofill):`)
  console.log(JSON.stringify(tx, null, 2))
  console.log(`\n2. Submitting transaction...`)

  const result = await client.submitAndWait(tx, {
    autofill: true,
    wallet: HOOK_ACCOUNT,
  })

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
