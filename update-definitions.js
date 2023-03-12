const fs = require('fs')

const { client, connectClient, disconnectClient } = require('./client/util/xrpl-client')

const DEFINITIONS_JSON_PATH = './node_modules/ripple-binary-codec/dist/enums/definitions.json'


async function server_definitions() {
  const request = {
    command: 'server_definitions',
  }
  return await client.request(request)
}

async function run() {
  await connectClient()
  console.log('client connected!')

  const response = await server_definitions()
  console.log('\n1. fetched definitions:')
  console.log(response)

  await fs.writeFileSync(
    DEFINITIONS_JSON_PATH,
    JSON.stringify(response.result, null, 2)
  )
  console.log(`\n2. Wrote to ${DEFINITIONS_JSON_PATH}`)

  await disconnectClient()
  console.log('client disconnected!')
}

run()
